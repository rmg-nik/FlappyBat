#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"

class GameScene : public cocos2d::Layer
{
public:
    
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(GameScene);
    
private:
    
    void setupScene();

    bool onTouch(cocos2d::Touch *touch, cocos2d::Event *event);

    void spawnBarrier(float);

    bool onContactBegin(cocos2d::PhysicsContact& contact);

    void onContactSeparate(cocos2d::PhysicsContact& contact);

private:

    void switchState();

    Hero* m_hero{ nullptr };

    float m_barrier_src_pos_x;

    float m_dst_pos_x;

    int m_score{ 0 };

    cocos2d::Label* m_score_lbl;    
};

#endif