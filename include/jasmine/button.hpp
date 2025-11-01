#pragma once
#include <cocos2d.h>

namespace jasmine::button {
    class ButtonHooker : public cocos2d::CCObject {
    public:
        cocos2d::CCObject* m_listener;
        cocos2d::SEL_MenuHandler m_selector;

        static ButtonHooker* create(cocos2d::CCMenuItem* button, cocos2d::CCObject* listener, cocos2d::SEL_MenuHandler selector);

        static void call(cocos2d::CCObject* button);
    };
}
