#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int imageId, int startX, int startY, int startDirection, double size, int depth, StudentWorld* world, bool damageable);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool isDead() const;
    void setDead();
    bool isDamageable() const;
    virtual void sufferDamage(int hp);
    StudentWorld* getWorld() const;
private:
    bool m_dead;
    StudentWorld* m_world;
    bool m_damageable;
};

class Star: public Actor
{
public:
    Star(int x, int y, StudentWorld* world);
    virtual ~Star();
    virtual void doSomething();
};

class Explosion: public Actor
{
public:
    Explosion(int x, int y, StudentWorld* world);
    virtual ~Explosion();
    virtual void doSomething();
    int count;
};

class NachenBlaster: public Actor
{
public:
    NachenBlaster(StudentWorld* world);
    virtual ~NachenBlaster();
    virtual void doSomething();
    void sufferDamage(int hp);
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
    Alien(int id, int x, int y, StudentWorld* world, double hp, int flightPlan, double travelSpeed);
    virtual ~Alien();
    void doSomething();
    double getHp() const;
    void setFlightPlan(int length);
    int getFlightPlan() const;
    void setFlightDirection(int dir);
    int getFlightDirection() const;
    double getTravelSpeed() const;
    void decrementFlight();
    void sufferDamage(int hp);
private:
    double m_hp;
    int m_flightPlan;
    int m_flightDirection;
    double m_travelSpeed;
};

class Smallgon: public Alien
{
public:
    Smallgon(int x, int y, StudentWorld* world);
    virtual ~Smallgon();
};

class Smoregon: public Alien
{
public:
    Smoregon(int x, int y, StudentWorld* world);
    virtual ~Smoregon();
};

class Snagglegon: public Alien
{
public:
    Snagglegon(int x, int y, StudentWorld* world);
    virtual ~Snagglegon();
};

#endif // ACTOR_H_
