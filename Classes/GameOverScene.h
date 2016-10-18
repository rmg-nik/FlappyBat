#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene(int new_score);

    virtual bool init();

    CREATE_FUNC(GameOverScene);

private:

    void setupScene();

    void setNewScore(int new_score);

    static int m_high_score;

};

#endif