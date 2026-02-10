#pragma once
#include <cocos2d.h>

namespace jasmine::nodes {
    class TableNode : public cocos2d::CCLayer {
    protected:
        std::vector<cocos2d::CCMenu*> m_menus;
        int m_columns;
        int m_buttons;

        bool init(int columns, int rows, float width, float height, std::string_view prefix);
    public:
        static TableNode* create(int columns, int rows, float width, float height, std::string_view prefix);

        void addButton(cocos2d::CCMenuItem* button);
        void updateAllLayouts();
    };
}
