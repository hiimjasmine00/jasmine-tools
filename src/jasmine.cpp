#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/utils/StringMap.hpp>
#include <jasmine.hpp>
#include <ranges>

using namespace geode::prelude;
using namespace jasmine::button;
using namespace jasmine::nodes;

namespace jasmine::settings {
    StringMap<SettingV3*>* getSettings() {
        static StringMap<SettingV3*>* settings = [] {
            auto settings = new StringMap<SettingV3*>();
            auto mod = Mod::get();
            auto manager = ModSettingsManager::from(mod);
            auto keys = mod->getSettingKeys();
            for (auto it = keys.begin(); it != keys.end(); it = keys.erase(it)) {
                settings->emplace(std::move(*it), manager->get(*it).get());
            }
            return settings;
        }();
        return settings;
    }
}

namespace ServerAPIEvents {
    struct Server {
        int id;
        std::string url;
        int priority;
    };

    inline Server getCurrentServer() GEODE_EVENT_EXPORT_CALL_NORES(&getCurrentServer, (), "km7dev.server_api/getCurrentServer");
}

std::string jasmine::gdps::getURL() {
    static_assert(GEODE_COMP_GD_VERSION == 22081, "Incompatible GD version for jasmine::gdps::getURL");

    static std::string url = [] {
        if (Loader::get()->isModLoaded("km7dev.server_api")) {
            auto url = std::move(ServerAPIEvents::getCurrentServer().url);
            if (!url.empty() && url != "NONE_REGISTERED") {
                while (url.ends_with('/')) url = url.substr(0, url.size() - 1);
                return url;
            }
        }

        return std::string(reinterpret_cast<const char*>(base::get() +
            GEODE_WINDOWS(0x558b70)
            GEODE_ARM_MAC(0x77d709)
            GEODE_INTEL_MAC(0x868df0)
            GEODE_ANDROID64(0xeccf90)
            GEODE_ANDROID32(0x96c0db)
            GEODE_IOS(0x6b8cc2)
        ), 0, 34);
    }();
    return url;
}

bool jasmine::gdps::isActive() {
    static bool isActive = getURL().rfind("://www.boomlings.com/database") == std::string::npos;
    return isActive;
}

Hook* jasmine::hook::createInternal(
    uintptr_t address, std::string name, void* detour, const tulip::hook::HandlerMetadata& metadata, Function<void(Hook*)> preClaim
) {
    auto hook = Hook::create(reinterpret_cast<void*>(address), detour, std::move(name), metadata, {});

    if (preClaim) preClaim(hook.get());

    auto res = Mod::get()->claimHook(hook);
    if (res.isOk()) return res.unwrap();
    else {
        if (res.isErr()) log::error("Failed to claim hook {}: {}", name, res.unwrapErr());
        return nullptr;
    }
}

Hook* jasmine::hook::get(StringMap<std::shared_ptr<Hook>>& hooks, std::string_view name, bool enabled) {
    if (hooks.empty()) return nullptr;

    auto it = hooks.find(name);
    if (it == hooks.end()) return nullptr;

    auto hook = it->second.get();
    hook->setAutoEnable(enabled);
    return hook;
}

void jasmine::hook::modify(StringMap<std::shared_ptr<Hook>>& hooks, std::string_view key) {
    if (hooks.empty()) return;

    auto value = setting::getValue<bool>(key);
    for (auto& hook : std::views::values(hooks)) {
        hook->setAutoEnable(value);
    }

    SettingChangedEventV3(GEODE_MOD_ID, std::string(key)).listen([hooks](std::shared_ptr<SettingV3> setting) {
        auto value = std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue();
        for (auto& hook : std::views::values(hooks)) {
            toggle(hook.get(), value);
        }
    }).leak();
}

void jasmine::hook::modify(StringMap<std::shared_ptr<Hook>>& hooks, std::string_view name, std::string_view key) {
    auto hook = get(hooks, name, setting::getValue<bool>(key));
    if (!hook) return;

    SettingChangedEventV3(GEODE_MOD_ID, std::string(key)).listen([hook](std::shared_ptr<SettingV3> setting) {
        toggle(hook, std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue());
    }).leak();
}

void jasmine::hook::toggle(Hook* hook, bool enable) {
    if (!hook) return;

    if (enable) {
        if (auto res = hook->enable(); res.isErr()) {
            log::error("Failed to enable {} hook: {}", hook->getDisplayName(), res.unwrapErr());
        }
    }
    else {
        if (auto res = hook->disable(); res.isErr()) {
            log::error("Failed to disable {} hook: {}", hook->getDisplayName(), res.unwrapErr());
        }
    }
}

int jasmine::level::getDifficulty(GJGameLevel* level) {
    if (!level) return 0;
    if (level->m_demon > 0) return GJGameLevel::demonIconForDifficulty((DemonDifficultyType)level->m_demonDifficulty);
    else if (level->m_autoLevel) return -1;
    else return level->getAverageDifficulty();
}

double jasmine::random::get() {
    return (double)rand() / (double)RAND_MAX;
}

double jasmine::random::get(double min, double max) {
    return get() * (max - min) + min;
}

const char* jasmine::search::getKey(GJSearchObject* object) {
    std::string_view key = object->getKey();
    return key.data() + (key.size() > 256uz ? key.size() - 256uz : 0uz);
}

SettingV3* jasmine::setting::getInternal(std::string_view key) {
    auto settings = jasmine::settings::getSettings();
    if (auto it = settings->find(key); it != settings->end()) return it->second;
    return nullptr;
}

std::vector<matjson::Value> jasmine::web::getArray(const utils::web::WebResponse& response, std::string_view key) {
    if (auto json = response.json()) {
        if (key.data()) {
            if (auto obj = json.unwrap().get(key)) {
                if (auto arr = std::move(obj.unwrap()).asArray()) {
                    return std::move(arr).unwrap();
                }
            }
        }
        else {
            if (auto arr = std::move(json).unwrap().asArray()) {
                return std::move(arr).unwrap();
            }
        }
    }
    return {};
}

std::string jasmine::web::getString(const utils::web::WebResponse& response) {
    auto& data = response.data();
    return std::string(reinterpret_cast<const char*>(data.data()), data.size());
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

    m_menus.reserve(rows);
    m_columns = columns;

    for (int i = 0; i < rows; i++) {
        auto menu = CCMenu::create();
        menu->setContentSize({ width, height / rows });
        menu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
        menu->setID(fmt::format("{}-{}", prefix, i + 1));
        addChild(menu);
        m_menus.push_back(menu);
    }

    setLayout(ColumnLayout::create()->setAxisReverse(true));

    return true;
}

void TableNode::addButton(CCMenuItem* button) {
    if (button) m_menus[m_buttons++ / m_columns]->addChild(button);
}

void TableNode::updateAllLayouts() {
    for (auto menu : m_menus) {
        menu->updateLayout();
    }
    updateLayout();
}
