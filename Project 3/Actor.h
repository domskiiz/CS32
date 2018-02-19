#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
public:
    Actor(int imageId, int startX, int startY, int startDirection, double size, int depth);
    virtual void doSomething() = 0;
    bool isDead();
    void setDead();
private:
    bool m_dead;
};

class Star: public Actor
{
public:
    Star();
    virtual void doSomething();
};

class NachenBlaster: public Actor
{
public:
    NachenBlaster();
    virtual void doSomething();
private:
    int m_hp;
    int m_cabbageEnergyPoints;
};


#endif // ACTOR_H_
