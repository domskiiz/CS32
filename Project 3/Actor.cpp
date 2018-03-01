#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>

const int       DIRECTION_RIGHT = 0;

const int       DEPTH_STAR = 3;

const double    EXPLOSION_SIZE = 1;
const int       EXPLOSION_DEPTH = 0;

const int       NACHENBLASTER_X = 0;
const int       NACHENBLASTER_Y = 128;
const double    NACHENBLASTER_SIZE = 1.0;
const int       NACHENBLASTER_DEPTH = 0;

const double    CABBAGE_SIZE = 0.5;
const int       CABBAGE_DEPTH = 1;
const int       CABBAGE_DAMAGE = 2;

const double    ALIEN_SIZE = 1.5;
const int       ALIEN_DEPTH = 1;

const int       FLIGHT_LEFT = 0;
const int       FLIGHT_UP_LEFT = 1;
const int       FLIGHT_DOWN_LEFT = 2;

//////////////////////////
// ACTOR IMPLEMENTATION //
//////////////////////////

Actor::Actor(int imageId, int startX, int startY, int startDirection, double size, int depth, StudentWorld* world, bool damageable)
: GraphObject(imageId, startX, startY, startDirection, size, depth)
{
    m_dead = false;
    m_world = world;
    m_damageable = damageable;
}

Actor::~Actor()
{
}

bool Actor::isDead() const
{
    return m_dead;
}

void Actor::setDead()
{
    m_dead = true;
}

bool Actor::isDamageable() const
{
    return m_damageable;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

void Actor::sufferDamage(int hp)
{
    
}

int Actor::getHP() const
{
    return 0;
}



/////////////////////////
// STAR IMPLEMENTATION //
/////////////////////////

Star::Star(int x, int y, StudentWorld* world)
: Actor(IID_STAR,
        x,
        y,
        DIRECTION_RIGHT,
        (randInt(5, 50) / 100.0),
        DEPTH_STAR,
        world,
        false)
{ }

Star::~Star()
{ }

void Star::doSomething()
{
    moveTo(getX()-1, getY()); 
    if (getX() < 0) {
        setDead();
    }
}

//////////////////////////////
// EXPLOSION IMPLEMENTATION //
//////////////////////////////

Explosion::Explosion(int x, int y, StudentWorld* world)
: Actor(IID_EXPLOSION,
        x,
        y,
        DIRECTION_RIGHT,
        EXPLOSION_SIZE,
        EXPLOSION_DEPTH,
        world,
        false)
{
    count = 0;
}

Explosion::~Explosion()
{ }

void Explosion::doSomething()
{
    setSize(1.5 * getSize());
    if (count < 4)
        count++;
    else
        setDead();
}


////////////////////////////
// CABBAGE IMPLEMENTATION //
////////////////////////////
Cabbage::Cabbage(int x, int y, StudentWorld* world)
: Actor(IID_CABBAGE,
        x,
        y,
        DIRECTION_RIGHT,
        CABBAGE_SIZE,
        CABBAGE_DEPTH,
        world,
        false)
{
    m_direction = 0;
}

Cabbage::~Cabbage()
{ }

void Cabbage::doSomething()
{
    if (isDead()) return;
    if (getX() >= VIEW_WIDTH) {
        setDead();
        return;
    }
    if (getWorld()->hitDamageableActors(this, CABBAGE_DAMAGE))
        return;
    moveTo(getX() + 8, getY());
    m_direction += 20;
    setDirection(m_direction);
}

//////////////////////////////////
// NACHENBLASTER IMPLEMENTATION //
//////////////////////////////////

NachenBlaster::NachenBlaster(StudentWorld* world)
: Actor(IID_NACHENBLASTER,
        NACHENBLASTER_X,
        NACHENBLASTER_Y,
        DIRECTION_RIGHT,
        NACHENBLASTER_SIZE,
        NACHENBLASTER_DEPTH,
        world,
        true)
{
    m_hp = 50;
    m_cabbageEnergyPoints = 30;
}

NachenBlaster::~NachenBlaster()
{ }

void NachenBlaster::doSomething()
{
    if (isDead()) return;
    int ch;
    if (getWorld()->getKey(ch)) {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                if (getX() - 6 > 0) {
                    moveTo(getX() - 6, getY());
                    break;
                } else break;
            case KEY_PRESS_RIGHT:
                if (getX() + 6 < VIEW_WIDTH) {
                    moveTo(getX() + 6, getY());
                    break;
                } else break;
            case KEY_PRESS_UP:
                if (getY() + 6 < VIEW_HEIGHT) {
                    moveTo(getX(), getY() + 6);
                    break;
                } else break;
            case KEY_PRESS_DOWN:
                if (getY() - 6 > 0) {
                    moveTo(getX(), getY() - 6);
                    break;
                } else break;
            case KEY_PRESS_SPACE:
                if (m_cabbageEnergyPoints >= 5) {
                    getWorld()->addActor(new Cabbage(getX() + 12, getY(), getWorld()));
                    getWorld()->playSound(SOUND_PLAYER_SHOOT);
                    m_cabbageEnergyPoints -= 5;
                }
            case KEY_PRESS_TAB:
                break;
        }
    }
    if (m_cabbageEnergyPoints < 30)
        m_cabbageEnergyPoints++;
}

void NachenBlaster::sufferDamage(int hp)
{
    m_hp -= hp;
}

int NachenBlaster::getHP() const
{
    return m_hp;
}

//////////////////////////
// ALIEN IMPLEMENTATION //
//////////////////////////
Alien::Alien(int id, int x, int y, StudentWorld* world, double hp, int flightPlan, double travelSpeed)
: Actor(id,
        x,
        y,
        DIRECTION_RIGHT,
        ALIEN_SIZE,
        ALIEN_DEPTH,
        world,
        true)
{
    m_flightPlan = flightPlan;
    m_travelSpeed = travelSpeed;
}

Alien::~Alien()
{ }

void Alien::doSomething()
{
    if (isDead()) return;
    if (getX() < 0) {
        setDead();
        getWorld()->decrementNumAliens();
        return;
    }
    if (getWorld()->shipCollision(this, getWorld()->getNachBlaster())) {
        return;
    }
    // setting new travel directions
    if (getY() >= VIEW_HEIGHT - 1) {
        setFlightDirection(FLIGHT_DOWN_LEFT);
    } else if (getY() <= 0) {
        setFlightDirection(FLIGHT_UP_LEFT);
    } else if (getFlightPlan() == 0) {
        setFlightDirection(randInt(0, 2));
        setFlightPlan(randInt(1, 32));
    }
    // check to fire turnip
    
    // move on screen
    switch(getFlightDirection()) {
        case FLIGHT_LEFT:
            moveTo(getX() - getTravelSpeed(), getY());
            decrementFlight();
            break;
        case FLIGHT_UP_LEFT:
            moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed());
            decrementFlight();
            break;
        case FLIGHT_DOWN_LEFT:
            moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed());
            decrementFlight();
            break;
        default:
            break;
    }
    
    if (getWorld()->shipCollision(this, getWorld()->getNachBlaster())) {
        return;
    }
}

int Alien::getFlightPlan() const
{
    return m_flightPlan;
}

void Alien::setFlightPlan(int length)
{
    m_flightPlan = length;
}

void Alien::decrementFlight()
{
    m_flightPlan--;
}

int Alien::getHP() const
{
    return m_hp;
}

void Alien::setHP(int hp)
{
    m_hp = hp;
}


void Alien::setFlightDirection(int dir)
{
    m_flightDirection = dir;
}

int Alien::getFlightDirection() const
{
    return m_flightDirection;
}

double Alien::getTravelSpeed() const
{
    return m_travelSpeed;
}

void Alien::sufferDamage(int hp)
{
    m_hp -= hp;
}

/////////////////////////////
// SMALLGON IMPLEMENTATION //
/////////////////////////////
Smallgon::Smallgon(int x, int y, StudentWorld* world)
: Alien(IID_SMALLGON, x, y, world, 0, 0, 2.0)           // fix to be correct hp?
{
    setHP(5 * (1 + (getWorld()->getLevel() - 1) * .1));
}

Smallgon::~Smallgon()
{ }



/////////////////////////////
// SMOREGON IMPLEMENTATION //
/////////////////////////////
Smoregon::Smoregon(int x, int y, StudentWorld* world)
: Alien(IID_SMOREGON, x, y, world, 0, 0, 2.0)           // fix to be correct hp?
{
    setHP(5 * (1 + (getWorld()->getLevel() - 1) * .1));
}

Smoregon::~Smoregon()
{ }


///////////////////////////////
// SNAGGLEGON IMPLEMENTATION //
///////////////////////////////
Snagglegon::Snagglegon(int x, int y, StudentWorld* world)
: Alien(IID_SNAGGLEGON, x, y, world, 0, 0, 1.75)           // fix to be correct hp?
{
    setHP(10 * (1 + (getWorld()->getLevel() - 1) * .1));
}

Snagglegon::~Snagglegon()
{ }


