#include "Barrier.h"
#include "CollisionMask.h"

USING_NS_CC;

Barrier * Barrier::createBarrier(float gapSize, int gapVar, float gapPosY, int gapPosYVar)
{
    auto barrier = new Barrier();

    if (!barrier->init(gapSize, gapVar, gapPosY, gapPosYVar))
    {
        delete barrier;
        barrier = nullptr;
    }

    return barrier;
}

float Barrier::getBarrierWidth() const 
{ 
    return m_barrier_width; 
}

bool Barrier::init(float gapSize, int gapVar, float gapPosY, int gapPosYVar)
{
    if (!Node::init())
        return false;

    auto gap_pos_y = gapPosY + rand() % gapPosYVar - gapPosYVar / 2;
    float gap = gapSize + rand() % gapVar;

    {
        auto top = Sprite::create("barrier_top.png");
        top->setAnchorPoint(Vec2(0.5f, 0.0f));
        top->setPosition(0, gap_pos_y + gap / 2);

        auto top_pbody = PhysicsBody::createBox(top->getContentSize());
        top_pbody->setGravityEnable(false);
        top_pbody->setDynamic(false);
        top_pbody->setCategoryBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER | COLLISION_BITMASK_t::COLLISION_HERO);
        top_pbody->setCollisionBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER);
        top_pbody->setContactTestBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER | COLLISION_BITMASK_t::COLLISION_HERO);
        top->setPhysicsBody(top_pbody);

        addChild(top);

        m_barrier_width = top->getContentSize().width;
    }

    {
        auto bottom = Sprite::create("barrier_bottom.png");
        bottom->setAnchorPoint(Vec2(0.5f, 1.0f));
        bottom->setPosition(0, gap_pos_y - gap / 2);

        auto bottom_pbody = PhysicsBody::createBox(bottom->getContentSize());
        bottom_pbody->setGravityEnable(false);
        bottom_pbody->setDynamic(false);
        bottom_pbody->setCategoryBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER | COLLISION_BITMASK_t::COLLISION_HERO);
        bottom_pbody->setCollisionBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER);
        bottom_pbody->setContactTestBitmask(COLLISION_BITMASK_t::COLLISION_BARRIER | COLLISION_BITMASK_t::COLLISION_HERO);
        bottom->setPhysicsBody(bottom_pbody);

        addChild(bottom);
    }

    {
        auto checkpoint = PhysicsBody::createBox(Size(1, gap));
        checkpoint->setPositionOffset(Vec2(m_barrier_width / 2.0f, gap_pos_y));
        checkpoint->setGravityEnable(false);
        checkpoint->setDynamic(false);
        checkpoint->setCategoryBitmask(COLLISION_BITMASK_t::COLLISION_CHECKPOINT);
        checkpoint->setCollisionBitmask(COLLISION_BITMASK_t::COLLISION_CHECKPOINT);
        checkpoint->setContactTestBitmask(COLLISION_BITMASK_t::COLLISION_CHECKPOINT | COLLISION_BITMASK_t::COLLISION_HERO);

        setPhysicsBody(checkpoint);
    }

    return true;
}
