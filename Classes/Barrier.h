#ifndef __BARRIER_HPP__
#define __BARRIER_HPP__

#include "cocos2d.h"

class Barrier : public cocos2d::Node
{
public:

    static Barrier* createBarrier(float gapSize, int gapVar, float gapPosY, int gapPosYVar);
    
    float getBarrierWidth() const;

private:

    bool init(float gapSize, int gapVar, float gapPosY, int gapPosYVar);

private:

    float m_barrier_width;
};
#endif