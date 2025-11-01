#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <jasmine.hpp>
#include <random>
#include <ranges>

using namespace geode::prelude;
using namespace jasmine::button;
using namespace jasmine::nodes;

namespace jasmine::settings {
    std::unordered_map<std::string, SettingV3*>* getSettings() {
        static std::unordered_map<std::string, SettingV3*>* settings = [] {
            auto settings = new std::unordered_map<std::string, SettingV3*>();
            auto mod = Mod::get();
            auto manager = ModSettingsManager::from(mod);
            for (auto& key : mod->getSettingKeys()) {
                settings->emplace(std::move(key), manager->get(key).get());
            }
            return settings;
        }();
        return settings;
    }
}

Hook* jasmine::hook::get(std::map<std::string, std::shared_ptr<Hook>>& hooks, std::string_view name, bool enabled) {
    if (hooks.empty()) return nullptr;

    auto it = hooks.find(std::string(name));
    if (it == hooks.end()) return nullptr;

    auto hook = it->second.get();
    hook->setAutoEnable(enabled);
    return hook;
}

void jasmine::hook::modify(std::map<std::string, std::shared_ptr<Hook>>& hooks, std::string_view key) {
    if (hooks.empty()) return;

    auto value = setting::getValue<bool>(key);
    for (auto& hook : std::views::values(hooks)) {
        hook->setAutoEnable(value);
    }

    new EventListener([hooks](std::shared_ptr<SettingV3> setting) {
        auto value = std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue();
        for (auto& hook : std::views::values(hooks)) {
            toggle(hook.get(), value);
        }
    }, SettingChangedFilterV3(GEODE_MOD_ID, std::string(key)));
}

void jasmine::hook::modify(std::map<std::string, std::shared_ptr<Hook>>& hooks, std::string_view name, std::string_view key) {
    auto hook = get(hooks, name, setting::getValue<bool>(key));
    if (!hook) return;

    new EventListener([hook](std::shared_ptr<SettingV3> setting) {
        auto value = std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue();
        toggle(hook, value);
    }, SettingChangedFilterV3(GEODE_MOD_ID, std::string(key)));
}

void jasmine::hook::toggle(Hook* hook, bool enable) {
    if (!hook) return;

    if (enable) {
        if (auto res = hook->enable(); res.isErr()) {
            log::error("Failed to enable {} hook: {}", hook->getDisplayName(), std::move(res).unwrapErr());
        }
    }
    else {
        if (auto res = hook->disable(); res.isErr()) {
            log::error("Failed to disable {} hook: {}", hook->getDisplayName(), std::move(res).unwrapErr());
        }
    }
}

double jasmine::random::get() {
    return (double)rand() / (double)RAND_MAX;
}

bool jasmine::random::getBool() {
    return getInt(0, 1) == 1;
}

int jasmine::random::getInt(int min, int max) {
    static std::mt19937 rng(std::random_device{}());
    return std::uniform_int_distribution<int>(min, max)(rng);
}

const char* jasmine::search::getKey(GJSearchObject* object) {
    std::string_view key = object->getKey();
    return key.data() + (key.size() > 256uz ? key.size() - 256uz : 0uz);
}

SettingV3* jasmine::setting::getInternal(std::string_view key) {
    auto settings = jasmine::settings::getSettings();
    if (auto it = settings->find(std::string(key)); it != settings->end()) return it->second;
    return nullptr;
}

std::vector<std::string_view> jasmine::string::split(std::string_view str, char delimiter) {
    std::vector<std::string_view> result;
    auto start = 0uz;
    auto end = str.find(delimiter);
    while (end != std::string_view::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));
    return result;
}

std::vector<std::string_view> jasmine::string::split(std::string_view str, std::string_view delimiter) {
    std::vector<std::string_view> result;
    auto start = 0uz;
    auto end = str.find(delimiter);
    while (end != std::string_view::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));
    return result;
}

std::vector<matjson::Value> jasmine::web::getArray(utils::web::WebResponse* response, std::string_view key) {
    if (auto json = response->json()) {
        if (key.data()) {
            if (auto obj = json.unwrap().get(key)) {
                if (auto arr = std::move(obj.unwrap()).asArray()) return std::move(arr).unwrap();
            }
        }
        else {
            if (auto arr = std::move(json).unwrap().asArray()) return std::move(arr).unwrap();
        }
    }
    return {};
}

std::string jasmine::web::getString(utils::web::WebResponse* response) {
    return std::string(std::from_range, response->data());
}

ButtonHooker* ButtonHooker::create(CCMenuItem* button, CCObject* listener, SEL_MenuHandler selector) {
    if (!button) return nullptr;
    auto hooker = new ButtonHooker();
    hooker->m_listener = button->m_pListener;
    hooker->m_selector = button->m_pfnSelector;
    button->setTarget(listener, selector);
    button->setUserObject("hooker"_spr, hooker);
    hooker->autorelease();
    return hooker;
}

void ButtonHooker::call(CCObject* button) {
    if (auto hooker = static_cast<ButtonHooker*>(static_cast<CCNode*>(button)->getUserObject("hooker"_spr))) {
        (hooker->m_listener->*hooker->m_selector)(button);
    }
}

TableNode* TableNode::create(int columns, int rows, float width, float height, std::string_view prefix) {
    auto ret = new TableNode();
    if (ret->init(columns, rows, width, height, prefix)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TableNode::init(int columns, int rows, float width, float height, std::string_view prefix) {
    if (!CCLayer::init()) return false;

    ignoreAnchorPointForPosition(false);
    setContentSize({ width, height });

    m_menus = CCArray::create();
    m_columns = columns;

    for (int i = 0; i < rows; i++) {
        auto menu = CCMenu::create();
        menu->setContentSize({ width, height / rows });
        menu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
        menu->setID(fmt::format("{}-{}", prefix, i + 1));
        addChild(menu);
        m_menus->addObject(menu);
    }

    setLayout(ColumnLayout::create()->setAxisReverse(true));

    return true;
}

void TableNode::addButton(CCMenuItem* button) {
    if (button) static_cast<CCNode*>(m_menus->objectAtIndex(m_buttons++ / m_columns))->addChild(button);
}

void TableNode::updateAllLayouts() {
    for (auto menu : CCArrayExt<CCNode*>(m_menus)) {
        menu->updateLayout();
    }
    updateLayout();
}
