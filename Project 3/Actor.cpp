#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

const int DEPTH_STAR = 3;
const int DIRECTION_RIGHT = 0;

const int NACHENBLASTER_X = 0;
const int NACHENBLASTER_Y = 128;
const double NACHENBLASTER_SIZE = 1.0;
const int NACHENBLASTER_DEPTH = 0;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageId, int startX, int startY, int startDirection, double size, int depth)
: GraphObject(imageId, startX, startY, startDirection, size, depth)
{
    m_dead = false;
}

bool Actor::isDead()
{
    return m_dead;
}

void Actor::setDead()
{
    m_dead = true;
}


/////////////////////////
// STAR IMPLEMENTATION //
/////////////////////////

Star::Star()
: Actor(IID_STAR,
        randInt(0, VIEW_WIDTH - 1),
        randInt(0, VIEW_HEIGHT - 1),
        DIRECTION_RIGHT,
        (randInt(5, 50) / 100.0),
        DEPTH_STAR)
{ }

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

NachenBlaster::NachenBlaster()
: Actor(IID_NACHENBLASTER,
        NACHENBLASTER_X,
        NACHENBLASTER_Y,
        DIRECTION_RIGHT,
        NACHENBLASTER_SIZE,
        NACHENBLASTER_DEPTH)
{ }

void NachenBlaster::doSomething()
{
    if (isDead()) return;
}


