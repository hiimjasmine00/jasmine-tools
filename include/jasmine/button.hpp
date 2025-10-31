#pragma once
#include <cocos2d.h>

namespace jasmine::button {
    class ButtonHooker : public cocos2d::CCObject {
        cocos2d::CCObject* m_listener;
        cocos2d::SEL_MenuHandler m_selector;

        static ButtonHooker* create(cocos2d::CCMenuItem* button, cocos2d::CCObject* listener, cocos2d::SEL_MenuHandler selector) {
            if (!button) return nullptr;
            auto hooker = new ButtonHooker();
            hooker->m_listener = button->m_pListener;
            hooker->m_selector = button->m_pfnSelector;
            button->setTarget(listener, selector);
            button->setUserObject(GEODE_MOD_ID "/hooker", hooker);
            hooker->autorelease();
            return hooker;
        }

        static void call(cocos2d::CCObject* button) {
            if (auto hooker = static_cast<ButtonHooker*>(static_cast<cocos2d::CCNode*>(button)->getUserObject(GEODE_MOD_ID "/hooker"))) {
                (hooker->m_listener->*hooker->m_selector)(button);
            }
        }
    };
}
