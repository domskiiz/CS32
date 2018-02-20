#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>

const int       DEPTH_STAR = 3;
const int       DIRECTION_RIGHT = 0;

const int       NACHENBLASTER_X = 0;
const int       NACHENBLASTER_Y = 128;
const double    NACHENBLASTER_SIZE = 1.0;
const int       NACHENBLASTER_DEPTH = 0;

const int       CABBAGE_DEPTH = 1;
const double    CABBAGE_SIZE = 0.5;

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

Star::Star(int x, int y, StudentWorld* world)
: Actor(IID_STAR,
        x,
        y,
        DIRECTION_RIGHT,
        (randInt(5, 50) / 100.0),
        DEPTH_STAR,
        world)
{ }

Star::~Star()
{
    std::cout << "Bai star" << std::endl;
}

void Star::doSomething()
{
    moveTo(getX()-1, getY());     // move one pixel to the left
    if (getX() < 0) {
        setDead();
    }
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
        world)
{
    m_direction = 0;
}

Cabbage::~Cabbage()
{
    std::cout << "Bai cabbs" << std::endl;
}

void Cabbage::doSomething()
{
    if (isDead()) return;
    if (getX() >= VIEW_WIDTH) {
        setDead();
        return;
    }
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
        world)
{
    m_hp = 50;
    m_cabbageEnergyPoints = 30;
}

NachenBlaster::~NachenBlaster()
{
    std::cout << "Bai nach" << std::endl;
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
                if (m_cabbageEnergyPoints >= 5) {
                    getWorld()->addCabbage(getX() + 12, getY());
                    m_cabbageEnergyPoints -= 5;
                }
            case KEY_PRESS_TAB:
                break;
        }
    }
    if (m_cabbageEnergyPoints < 30)
        m_cabbageEnergyPoints++;
}


