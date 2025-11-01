#pragma once
#include <Geode/utils/cocos.hpp>

namespace jasmine::nodes {
    class TableNode : public cocos2d::CCLayer {
    protected:
        geode::Ref<cocos2d::CCArray> m_menus;
        int m_rows;
        int m_buttons;

        bool init(int columns, int rows, float width, float height, std::string_view prefix);
    public:
        static TableNode* create(int columns, int rows, float width, float height, std::string_view prefix);

        void addButton(cocos2d::CCMenuItem* button);
        void updateAllLayouts();
    };
}
