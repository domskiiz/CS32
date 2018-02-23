#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
public:
    Actor(int imageId, int startX, int startY, int startDirection, double size, int depth, StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool isDead() const;
    void setDead();
    StudentWorld* getWorld() const;
private:
    bool m_dead;
    StudentWorld* m_world;
};

class Star: public Actor
{
public:
    Star(int x, int y, StudentWorld* world);
    virtual ~Star();
    virtual void doSomething();
};

class NachenBlaster: public Actor
{
public:
    NachenBlaster(StudentWorld* world);
    virtual ~NachenBlaster();
    virtual void doSomething();
private:
    int m_hp;
    int m_cabbageEnergyPoints;
};

class Cabbage: public Actor
{
public:
    Cabbage(int x, int y, StudentWorld* world);
    virtual ~Cabbage();
    virtual void doSomething();
private:
    int m_direction;
};



class Alien: public Actor
{
public:
    Alien(int id, int x, int y, StudentWorld* world);
    virtual ~Alien();
    virtual void doSomething() = 0;
private:
    int m_hp;
    int m_flightPlan;
    double m_travelSpeed;
};

class Smallgon: public Alien
{
public:
    Smallgon(int x, int y, StudentWorld* world);
    virtual ~Smallgon();
    virtual void doSomething();
private:

};

#endif // ACTOR_H_
