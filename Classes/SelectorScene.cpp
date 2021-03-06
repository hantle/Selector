#include "SelectorScene.h"

#define GRAVITY         1000
#define DRAG_BODYS_TAG  78
#define NUM_BALLS       16
#define NUM_BG_VEC      20
#define BG_RADIUS       280
#define BALL_RADIUS     50
#define HOLE_SIZE_RAD   0.5
#define IMPULSE_MAX     700
#define IMPULSE_MIN     100

USING_NS_CC;

Scene* CSelector::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    auto world = scene->getPhysicsWorld();
    
    // 'layer' is an autorelease object
    auto layer = CSelector::create();
    
    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE); // physics world debugging을 위한 붉은 영역
    //world->setGravity(Point(0, -GRAVITY));
    

    // add layer as a child to scene
    scene->addChild(layer);
    
    CCLOG("loaded SelectorScene");

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CSelector::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    CCLOG("CSelector Width : %.f, Height : %.f", this->getContentSize().width, this->getContentSize().height);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    CCLOG("VisibleSize Width : %.f, Height : %.f", visibleSize.width, visibleSize.height);
    CCLOG("VisibleOrigin x : %.f, y : %.f", visibleOrigin.x, visibleOrigin.y);
    
//    CCLOG("Class size : %.f x %.f", this->getContentSize().width, this->getContentSize().height);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(CSelector::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(visibleOrigin.x + visibleSize.width
                                - closeItem->getContentSize().width/2, visibleOrigin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    // Touch Events (Single) --> Touch를 사용하기 위한 정형화 된 code 이니 외우자!!!
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CSelector::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CSelector::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CSelector::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // Acceleration Event
    Device::setAccelerometerEnabled(true);
    auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(CSelector::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(accListener, this);
    
    this->vInitVariables();
    this->vInitDrawVisibleEdge();
    //this->vInitDrawBarrel();
    this->vInitDrawSprites();
    
    
    return true;
}


void CSelector::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void CSelector::vInitVariables()
{
    
    balls = Node::create();
    balls->retain();
    this->addChild(balls);
}


void CSelector::vInitDrawVisibleEdge()
{
    //glClearColor(81.0/255.0, 192.0/255.0, 201.0/255.0, 1.0);
    auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
//    edgeNode->setAnchorPoint(Vec2(0.5, 0.5));
    edgeNode->setPosition(Point(this->getContentSize().width/2, this->getContentSize().height/2));
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);
}

void CSelector::vInitDrawBarrel()
{
    // Draw Barrel
    Vec2 vec[NUM_BG_VEC];
    float radInterval = (2*M_PI - HOLE_SIZE_RAD) / (NUM_BG_VEC-3);
    
    for (int i = 0; i < NUM_BG_VEC-2; i++)
    {
        float x, y;
        x = BG_RADIUS*cosf((-0.5)*M_PI + HOLE_SIZE_RAD/2 + i*radInterval);
        y = BG_RADIUS*sinf((-0.5)*M_PI + HOLE_SIZE_RAD/2 + i*radInterval);
        vec[i] = Point(x, y);
    }
    vec[NUM_BG_VEC-2] = Vec2(vec[NUM_BG_VEC-3].x,   vec[NUM_BG_VEC-3].y - 100);
    vec[NUM_BG_VEC-1] = Vec2(vec[0].x,              vec[0].y - 100);
    
    
    for (int i = 0; i < NUM_BG_VEC; i++)
    {
        CCLOG("vec[%d].x:%.1f, y:%.1f",i, vec[i].x, vec[i].y);
    }
    auto worldBody = PhysicsBody::createEdgePolygon(vec, NUM_BG_VEC, PHYSICSBODY_MATERIAL_DEFAULT, 20);
//    worldbody->
    auto worldSprite = Sprite::create();
    worldSprite->setPosition(Point(this->getContentSize().width/2, this->getContentSize().height/2));
    worldSprite->setPhysicsBody(worldBody);
    this->addChild(worldSprite);
}

void CSelector::vInitDrawSprites()
{
    // add image to texture cache
    //Texture2D *image[NUM_BALLS];
    std::string imageName[NUM_BALLS];
    imageName[0] = StringUtils::format("res/Byungjoo.png");
    imageName[1] = StringUtils::format("res/Cheonhyuck.png");
    imageName[2] = StringUtils::format("res/Donghyun.png");
    imageName[3] = StringUtils::format("res/Doyoung.png");
    imageName[4] = StringUtils::format("res/Euiyun.png");
    imageName[5] = StringUtils::format("res/Jaehyuk.png");
    imageName[6] = StringUtils::format("res/Jaesung.png");
    imageName[7] = StringUtils::format("res/Jina.png");
    imageName[8] = StringUtils::format("res/Jinwook.png");
    imageName[9] = StringUtils::format("res/Jonghoon.png");
    imageName[10] = StringUtils::format("res/Joowheon.png");
    imageName[11] = StringUtils::format("res/Manjae.png");
    imageName[12] = StringUtils::format("res/Seonggwon.png");
    imageName[13] = StringUtils::format("res/Seongjin.png");
    imageName[14] = StringUtils::format("res/Yonghan.png");
    imageName[15] = StringUtils::format("res/Yurye.png");
    
    
    // make physicsBody
    auto physicsBody = PhysicsBody::create();
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);
   
    // add five dynamic body
    for (int i = 0; i < NUM_BALLS; i++)
    {
        physicsBody = PhysicsBody::createCircle(BALL_RADIUS, PhysicsMaterial(0.3, 0.99, 0.1));
        
        //set the body isn't affected by the physics world's gravitational force
        physicsBody->setTag(DRAG_BODYS_TAG);
        
        // images on sprites
        auto sprite = Sprite::create(imageName[i]);
//        sprite->setScale(1);
        sprite->setPosition(Vec2(this->getContentSize().width/2 + cocos2d::random(-BG_RADIUS/2, BG_RADIUS/2),this->getContentSize().height/2 + cocos2d::random(-BG_RADIUS/2, BG_RADIUS/2)));
        sprite->setPhysicsBody(physicsBody);
        
        balls->addChild(sprite);
        CCLOG("Ball No.%d is created", i);
    }
}

// Touches
bool CSelector::onTouchBegan(Touch *touch, Event *unused_event)
{
    this->selectSpriteForTouch(touch->getLocation());
    //this->onTouchMoved(touch, unused_event);
    return true;
}



void CSelector::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (selSprite)
    {
        Point diff;
        diff = touch->getLocation() - selSprite->getPosition();
        selSprite->getPhysicsBody()->setVelocity(diff*10);

//        auto dragAction = MoveTo::create(0, touch->getLocation());
//        selSprite->getPhysicsBody()->setDynamic(false);
//        selSprite->runAction(dragAction);
//        selSprite->setPosition(touch->getLocation());
    }
//    Touch *pTouch_ = (Touch *)touch->anyObject();
//    touch->
//    Point touchLocation = convertToNodeSpace(touch->getLocation());  // [self convertTouchToNodeSpace:touch];
//    
//    Point oldTouchLocation = touch->getPreviousLocation();   // [touch previousLocationInView:touch.view];
//    oldTouchLocation = Director::sharedDirector()->convertToGL(oldTouchLocation);  // [[CCDirector sharedDirector] convertToGL:oldTouchLocation];
//    oldTouchLocation = [self convertToNodeSpace:oldTouchLocation];
//    
//    Point translation = ccpsub(touchLocation, oldTouchLocation);
//    Point newPos = ccpAdd(mySpriteToMove.position, translation);
//    mySpriteToMove.position = newPos;
    
}

void CSelector::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (selSprite) {
        selSprite->getPhysicsBody()->setDynamic(true);
    }
    else
    {
        for (auto child: balls->getChildren())
        {
            float xDiff, yDiff, ix, iy;
            xDiff = child->getPosition().x - touch->getLocation().x;
            yDiff = child->getPosition().y - touch->getLocation().y;
            if (xDiff < 0) {
                xDiff = -IMPULSE_MAX - xDiff;
                ix = MIN(xDiff, -IMPULSE_MIN);  // abs MIN is applied
                ix = MAX(xDiff, -IMPULSE_MAX);  // abs MAX is applied
            }
            else {
                xDiff = IMPULSE_MAX - xDiff;
                ix = MAX(xDiff, IMPULSE_MIN);  // abs MIN is applied
                ix = MIN(xDiff, IMPULSE_MAX);  // abs MAX is applied
            }
            if (yDiff < 0) {
                yDiff = -IMPULSE_MAX - yDiff;
                iy = MIN(yDiff, -IMPULSE_MIN);  // abs MIN is applied
                iy = MAX(yDiff, -IMPULSE_MAX);  // abs MAX is applied
            }
            else {
                yDiff = IMPULSE_MAX - yDiff;
                iy = MAX(yDiff, IMPULSE_MIN);  // abs MIN is applied
                iy = MIN(yDiff, IMPULSE_MAX);  // abs MAX is applied
            }
            
            
            //        if (ix < 0) ix = 0;
            //        if (iy < 0) iy = 0;
            child->getPhysicsBody()->applyImpulse(Vect(ix,iy) * GRAVITY);
            //CCLOG("Ball Pos.x:%.1f, y:%.1f", child->getPosition().x, child->getPosition().y);
        }
    }
    //CCLOG("Touched x:%f, y:%f",touch->getLocation().x, touch->getLocation().y);
}


void CSelector::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
{
    auto world = this->getScene()->getPhysicsWorld();
    world->setGravity(Point(acc->x, acc->y)*1000);
    
//    CCLOG("Accel. x:%.2f, y:%.2f, z:%.2f", acc->x, acc->y, acc->z);
}

void CSelector::selectSpriteForTouch(cocos2d::Point touchPoint)
{
    Node *newSprite = NULL;
    for (auto child : balls->getChildren()) {
        if ( child->getBoundingBox().containsPoint(touchPoint)){
            newSprite = child;
            break;
        }
    }
    if (newSprite != selSprite){
        selSprite = newSprite;
    }
}







