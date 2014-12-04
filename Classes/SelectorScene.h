#ifndef __SELECTOR_SCENE_H__
#define __SELECTOR_SCENE_H__

#include "cocos2d.h"

class CSelector : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // Global variables
    cocos2d::Size visibleSize;
    cocos2d::Vec2 visibleOrigin;
//    cocos2d::Sprite *sprite;
    cocos2d::Node *balls;
    
    
    // Functions
    void vInitVariables();
    void vInitDrawBackground();
    void vInitDrawSprites();
    
    
    //     Touches
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(CSelector);
};

#endif // __SELECTOR_SCENE_H__
