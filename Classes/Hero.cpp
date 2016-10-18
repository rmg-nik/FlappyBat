#include "Hero.h"
#include "CollisionMask.h"

USING_NS_CC;

Hero* Hero::createHero()
{
    auto hero = Hero::create();

    return hero;
}

bool Hero::init() 
{
    if (!Node::init())
        return false;
    
    setScale(1.5f);

    auto temp_hero = Sprite::create("hero_0.png");
    const auto hero_size = temp_hero->getContentSize();

    const int FRAME_COUNT = 6;
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < FRAME_COUNT; i++)
    {
        sprintf(str, "hero_%d.png", i);
        auto frame = SpriteFrame::create(str, Rect(0, 0, hero_size.width, hero_size.height));
        animFrames.pushBack(frame);
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.04f);
    m_hero_animation = Animate::create(animation);
    m_hero_animation->retain();
    
    m_hero = Sprite::create();
    m_hero->setSpriteFrame(m_hero_animation->getAnimation()->getFrames().at(0)->getSpriteFrame());    

    //-10% of size for assurance
    auto physics_body = PhysicsBody::createCircle(hero_size.height / 2 - 0.1f * hero_size.height);
    physics_body->setCategoryBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER | COLLISION_BITMASK_t::COLLISION_HERO | COLLISION_BITMASK_t::COLLISION_CHECKPOINT);
    physics_body->setCollisionBitmask(COLLISION_BITMASK_t::COLLISION_HERO);
    physics_body->setContactTestBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER | COLLISION_BITMASK_t::COLLISION_HERO | COLLISION_BITMASK_t::COLLISION_CHECKPOINT);
    setPhysicsBody(physics_body);

    addChild(m_hero);
    return true;
}

void Hero::Fly()
{
    if (m_is_dropped)
        return;

    m_hero->stopAllActions();
        
    auto act = Sequence::create(
        m_hero_animation, 
        m_hero_animation->reverse(),
        CallFunc::create([=]()
    {
        m_hero->setSpriteFrame(m_hero_animation->getAnimation()->getFrames().at(0)->getSpriteFrame()); 
    }),
        nullptr);
    m_hero->runAction(act);
}

void Hero::Drop()
{
    m_is_dropped = true;
    m_hero->stopAllActions();
}

Hero::~Hero()
{
    if (m_hero_animation)
        m_hero_animation->release();
}
