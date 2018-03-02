#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int imageId, double startX, double startY, int startDirection, double size, int depth, StudentWorld* world, bool damageable);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool isDead() const;
    void setDead();
    bool isDamageable() const;
    StudentWorld* getWorld() const;
    virtual void sufferDamage(int hp);
    virtual int getHP() const;
    virtual int getScore() const;
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
    int getCount() const;
    void incCount();
private:
    int count;
};

class Projectile: public Actor
{
public:
    Projectile(int id, int x, int y, StudentWorld* world, int direction);
    virtual ~Projectile();
    virtual void doSomething() = 0;
    int getDirection() const;
    void setThisDirection(int dir);
private:
    int m_direction;
};

class Cabbage: public Projectile
{
public:
    Cabbage(int x, int y, StudentWorld* world);
    virtual ~Cabbage();
    virtual void doSomething();
};

class Turnip: public Projectile
{
public:
    Turnip(int x, int y, StudentWorld* world);
    virtual ~Turnip();
    virtual void doSomething();
};

class Torpedo: public Projectile
{
public:
    Torpedo(int x, int y, StudentWorld* world, int direction);
    virtual ~Torpedo();
    virtual void doSomething();
    virtual void specializedAttack() = 0;
};

class AlienTorpedo: public Torpedo
{
public:
    AlienTorpedo(int x, int y, StudentWorld* world);
    virtual ~AlienTorpedo();
    void specializedAttack();
};

class NachTorpedo: public Torpedo
{
public:
    NachTorpedo(int x, int y, StudentWorld* world);
    virtual ~NachTorpedo();
    void specializedAttack();
};

class NachenBlaster: public Actor
{
public:
    NachenBlaster(StudentWorld* world);
    virtual ~NachenBlaster();
    virtual void doSomething();
    void sufferDamage(int hp);
    void increaseHP(int hp);
    virtual int getHP() const;
    int getCabbagePercent() const;
    int getNumTorpedoes() const;
    void setNumTorpedoes(int num);
    void decTorpedoes();
private:
    int m_hp;
    int m_cabbageEnergyPoints;
    int m_nachTorpedoes;
};

class Alien: public Actor
{
public:
    Alien(int id, int x, int y, StudentWorld* world, double hp, int flightPlan, double travelSpeed, int score);
    virtual ~Alien();
    virtual void doSomething();
    virtual void specializedAttack() = 0;
    virtual void dropSomething() = 0;
    virtual int getHP() const;
    virtual void setHP(int hp);
    void setFlightPlan(int length);
    int getFlightPlan() const;
    void setFlightDirection(int dir);
    int getFlightDirection() const;
    void decrementFlight();
    void setTravelSpeed(double speed);
    double getTravelSpeed() const;
    void sufferDamage(int hp);
    virtual int getScore() const;
private:
    double m_hp;
    int m_flightPlan;
    int m_flightDirection;
    double m_travelSpeed;
    int m_scorePoints;
};

class Smallgon: public Alien
{
public:
    Smallgon(int x, int y, StudentWorld* world);
    virtual ~Smallgon();
    virtual void specializedAttack();
    virtual void dropSomething();
};

class Smoregon: public Alien
{
public:
    Smoregon(int x, int y, StudentWorld* world);
    virtual ~Smoregon();
    virtual void specializedAttack();
    virtual void dropSomething();
};

class Snagglegon: public Alien
{
public:
    Snagglegon(int x, int y, StudentWorld* world);
    virtual ~Snagglegon();
    virtual void specializedAttack();
    virtual void dropSomething();
};

class Goodie: public Actor
{
public:
    Goodie(int x, int y, StudentWorld* world, int id);
    virtual ~Goodie();
    virtual void doSomething();
    virtual void specialized() = 0;
};

class ExtraLifeGoodie: public Goodie
{
public:
    ExtraLifeGoodie(int x, int y, StudentWorld* world);
    virtual ~ExtraLifeGoodie();
    void specialized();
};

class RepairGoodie: public Goodie
{
public:
    RepairGoodie(int x, int y, StudentWorld* world);
    virtual ~RepairGoodie();
    void specialized();
};

class TorpedoGoodie: public Goodie
{
public:
    TorpedoGoodie(int x, int y, StudentWorld* world);
    virtual ~TorpedoGoodie();
    void specialized();
};



#endif // ACTOR_H_
