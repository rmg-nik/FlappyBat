#ifndef __HERO_HPP__
#define __HERO_HPP__

#include "cocos2d.h"

class Hero : public cocos2d::Node
{
public:

    static Hero* createHero();

    bool init() override;

    CREATE_FUNC(Hero);

    void Fly();

    void Drop();

    virtual ~Hero();

private:

    cocos2d::Sprite* m_hero{ nullptr };

    cocos2d::Animate* m_hero_animation{ nullptr };

    bool m_is_dropped{ false };
};

#endif