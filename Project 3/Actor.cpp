#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

const int       DEPTH_STAR = 3;
const int       DIRECTION_RIGHT = 0;

const int       NACHENBLASTER_X = 0;
const int       NACHENBLASTER_Y = 128;
const double    NACHENBLASTER_SIZE = 1.0;
const int       NACHENBLASTER_DEPTH = 0;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageId, int startX, int startY, int startDirection, double size, int depth, StudentWorld* world)
: GraphObject(imageId, startX, startY, startDirection, size, depth)
{
    m_dead = false;
    m_world = world;
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

StudentWorld* Actor::getWorld() const
{
    return m_world;
}


/////////////////////////
// STAR IMPLEMENTATION //
/////////////////////////

Star::Star(StudentWorld* world)
: Actor(IID_STAR,
        randInt(0, VIEW_WIDTH - 1),
        randInt(0, VIEW_HEIGHT - 1),
        DIRECTION_RIGHT,
        (randInt(5, 50) / 100.0),
        DEPTH_STAR,
        world)
{ }

Star::~Star()
{
}

void Star::doSomething()
{
    moveTo(getX()-1, getY());     // move one pixel to the left
    if (getX() < 0) {
        setDead();
    }
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
        world)
{
    m_hp = 50;
    m_cabbageEnergyPoints = 30;
}

NachenBlaster::~NachenBlaster()
{
}

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
                break;
            case KEY_PRESS_TAB:
                break;
        }
    }
    if (m_cabbageEnergyPoints < 30)
        m_cabbageEnergyPoints++;
}


