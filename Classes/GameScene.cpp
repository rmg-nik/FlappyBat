#include "GameScene.h"
#include "GameOverScene.h"
#include "Barrier.h"
#include "CollisionMask.h"

#include "ui/CocosGUI.h"

USING_NS_CC;

namespace
{
    const Size GROUND_SIZE{ 2560, 200 };
    const float MOVE_BY_TIME = 6.0f;
    const float BARRIER_SPAWN_INTERVAL = 2.0f;
    const float GAP_SIZE = 180.0f;
    const int GAP_SIZE_VAR = 50;
    const int GAP_POS_Y_VAR = 300;
    const float GRAVITY = -3.0f * 98.f;
    const float SPEED_UP = 200.0f;
}

Scene* GameScene::createScene()
{    
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0.f, GRAVITY));
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = GameScene::create();
    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{  
    if (!Layer::init())
    {
        return false;
    }

    setupScene();

    return true;
}

void GameScene::setupScene() 
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();    
    Size rootSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

    m_dst_pos_x = -GROUND_SIZE.width / 2;
    m_barrier_src_pos_x = rootSize.width;

    {
        auto node = Node::create();

        auto background_1 = Sprite::create("background.jpg");        
        background_1->setAnchorPoint({ 0.0f, 0.5f });
        background_1->setPosition(Vec2(0.0f, visibleSize.height / 2 + origin.y));
        node->addChild(background_1, -2);

        auto background_2 = Sprite::create("background.jpg");
        background_2->setAnchorPoint({ 0.0f, 0.5f });
        background_2->setPosition(Vec2(background_1->getContentSize().width, visibleSize.height / 2 + origin.y));
        node->addChild(background_2, -2);

        addChild(node, -2);

        auto act = RepeatForever::create(Sequence::create(MoveBy::create(MOVE_BY_TIME * 2.0f, { -rootSize.width, 0 }), MoveBy::create(0.0f, { rootSize.width, 0.0f }), nullptr));
        node->runAction(act);
    }     

    {
        auto sprite_ground = Sprite::create("ground.png");
        sprite_ground->getTexture()->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

        sprite_ground->setContentSize(GROUND_SIZE);
        sprite_ground->setTextureRect(Rect(0, 0, GROUND_SIZE.width, GROUND_SIZE.height));
        sprite_ground->setAnchorPoint({ 0.0f, 0.0f });
        sprite_ground->setPosition(Vec2(0, 0));
        this->addChild(sprite_ground);

        auto act = RepeatForever::create(Sequence::create(MoveBy::create(MOVE_BY_TIME, { m_dst_pos_x, 0 }), MoveTo::create(0.0f, { 0.0f, 0.0f }), nullptr));
        sprite_ground->runAction(act);
    }

    {
        auto sprite_ground = Sprite::create("ground.png");
        sprite_ground->getTexture()->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
        sprite_ground->setFlippedY(true);
        sprite_ground->setContentSize(GROUND_SIZE);
        sprite_ground->setTextureRect(Rect(0, 0, GROUND_SIZE.width, GROUND_SIZE.height));
        sprite_ground->setAnchorPoint({ 0.0f, 1.0f });
        sprite_ground->setPosition(Vec2(0, rootSize.height));
        this->addChild(sprite_ground);

        auto act = RepeatForever::create(Sequence::create(MoveBy::create(MOVE_BY_TIME, { m_dst_pos_x, 0 }), MoveTo::create(0.0f, { 0.0f, rootSize.height }), nullptr));
        sprite_ground->runAction(act);
    }
     
    auto edge_body = PhysicsBody::createEdgeBox(Size(rootSize.width, rootSize.height - 2 * GROUND_SIZE.height));
    edge_body->setCategoryBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER | COLLISION_BITMASK_t::COLLISION_HERO);
    edge_body->setCollisionBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER);
    edge_body->setContactTestBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER | COLLISION_BITMASK_t::COLLISION_HERO);
    setPhysicsBody(edge_body);

    m_hero = Hero::createHero();
    m_hero->setPosition(Vec2(rootSize.width / 2, rootSize.height / 2));
    this->addChild(m_hero);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouch, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);    

    schedule(schedule_selector(GameScene::spawnBarrier), BARRIER_SPAWN_INTERVAL);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    m_score_lbl = Label::createWithTTF("Score: 0", "PixelEmulator.ttf", 52);
    m_score_lbl->enableOutline(Color4B(0, 0, 0, 255), 4);
    m_score_lbl->setAnchorPoint(Vec2(1.0f, 1.0f));
    m_score_lbl->setPosition(Vec2(visibleSize.width + origin.x - 20, visibleSize.height + origin.y - 20));
    addChild(m_score_lbl);
}

bool GameScene::onTouch(cocos2d::Touch *touch, cocos2d::Event *event)
{
    m_hero->Fly();
    m_hero->getPhysicsBody()->setVelocity(Vec2(0, SPEED_UP));    
    return true;
}

void GameScene::spawnBarrier(float) 
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto barrier = Barrier::createBarrier(GAP_SIZE, GAP_SIZE_VAR, origin.y + visibleSize.height / 2, GAP_POS_Y_VAR);    
    barrier->setPosition(Vec2(m_barrier_src_pos_x + barrier->getBarrierWidth(), 0));
    this->addChild(barrier, -1);

    auto act = Sequence::create(MoveBy::create(MOVE_BY_TIME, { m_dst_pos_x, 0 }), RemoveSelf::create(true), nullptr);
    barrier->runAction(act);
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact & contact)
{
    auto a = contact.getShapeA()->getBody();
    auto b = contact.getShapeB()->getBody();
    const auto amask = a->getCollisionBitmask();
    const auto bmask = b->getCollisionBitmask();

    if (amask == bmask)
        return false;

    if ((amask == COLLISION_BITMASK_t::COLLISION_CHECKPOINT && bmask == COLLISION_BITMASK_t::COLLISION_HERO) ||
        (amask == COLLISION_BITMASK_t::COLLISION_HERO && bmask == COLLISION_BITMASK_t::COLLISION_CHECKPOINT))
    {
        return false;
    }

    if ((amask == COLLISION_BITMASK_t::COLLISION_BARRIER && bmask == COLLISION_BITMASK_t::COLLISION_HERO) ||
        (amask == COLLISION_BITMASK_t::COLLISION_HERO && bmask == COLLISION_BITMASK_t::COLLISION_BARRIER))
    {
        m_hero->Drop();        
        const auto chs = getChildren();
        for (auto&x : chs)
            x->stopAllActions();
        auto scene = GameOverScene::createScene(m_score);
        Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
        return true;
    }

    log("There are shouldn't be any other contact");
    return false;
}

void GameScene::onContactSeparate(cocos2d::PhysicsContact & contact) 
{
    auto a = contact.getShapeA()->getBody();
    auto b = contact.getShapeB()->getBody();
    const auto amask = a->getCollisionBitmask();
    const auto bmask = b->getCollisionBitmask();

    if (amask == bmask)
        return;

    if ((amask == COLLISION_BITMASK_t::COLLISION_CHECKPOINT && bmask == COLLISION_BITMASK_t::COLLISION_HERO) ||
        (amask == COLLISION_BITMASK_t::COLLISION_HERO && bmask == COLLISION_BITMASK_t::COLLISION_CHECKPOINT))
    {
        m_score++;        
        m_score_lbl->setString("Score: " + std::to_string(m_score));
        return;
    }

    log("There are shouldn't be any other separate");
}

void GameScene::switchState()
{    
    if (Director::getInstance()->isPaused())
    {
        Director::getInstance()->resume();
        getScene()->getPhysicsWorld()->setSpeed(1.0f);     
    }
    else
    {
        Director::getInstance()->pause();
        getScene()->getPhysicsWorld()->setSpeed(0);
    }
}
