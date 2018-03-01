#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include<iostream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), m_numActors(0), m_aliensDestroyed(0)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    for (int i = 0; i < 30; i++) {
        m_actors.push_back(new Star(randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1), this));
        m_numActors++;
    }
    m_nachBlaster = new NachenBlaster(this);
    m_numActors++;
    return GWSTATUS_CONTINUE_GAME;
}

/*
 • There is a one in fifteen chance that you will introduce a new star into the game
 on the far right side of the screen (at x=VIEW_WIDTH-1). Each such star will
 have a random y coordinate between [0, VIEW_HEIGHT). The size of each new
 star must also be determined randomly, and must be between .05 and .5 units in
 size.
 */

int StudentWorld::move()
{
    // for each actor, do something
    for (int i = 0; i < m_numActors; i++) {
        Actor* actor;
        (i < m_numActors - 1) ? actor = m_actors[i] : actor = m_nachBlaster;
        if (!actor->isDead())
            actor->doSomething();
        if (m_nachBlaster->isDead())
            return GWSTATUS_PLAYER_DIED;
    }
    // remove dead actors
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if ((*it)->isDead()) {
            delete *it;
            it = m_actors.erase(it);
            m_numActors--;
        } else {
            it++;
        }
    }
    // introduce stars
    if (randInt(1, 15) == 15) {
        m_actors.push_back(new Star(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this));
        m_numActors++;
    }
    
    // place aliens
    int aliensMustBeDestroyed = 6 + (4 * getLevel());
    int maximumAliensOnScreen = 4 + (0.5 * getLevel());
    int remainingShipsToBeDestroyed = aliensMustBeDestroyed - m_aliensDestroyed;
    if (m_numAliens < min(maximumAliensOnScreen, remainingShipsToBeDestroyed)) {
        // determine what type of ship to add
        int s1 = 60, s2 = 20 + (getLevel() * 5), s3 = 5 + (getLevel() * 10);
        int s = s1 + s2 + s3;
        int probability = randInt(1, s);
        Actor* newAlien;
        if (1 <= probability && probability <= s1)
            newAlien = new Smallgon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this);
        if (s1 < probability && probability < s1 + s2)
            newAlien = new Smoregon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this);
        if (s1 + s2 < probability && probability < s)
            newAlien = new Snagglegon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this);
        // add new ship
        m_actors.push_back(newAlien);
        m_numAliens++;
        m_numActors++;
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        delete *it;
        it = m_actors.erase(it);
    }
    if (m_nachBlaster != nullptr) {
        delete m_nachBlaster;
        m_nachBlaster = nullptr;
    }
    m_numActors = 0;
}

void StudentWorld::addActor(Actor* actor)
{
    m_actors.push_back(actor);
    m_numActors++;
}

NachenBlaster* StudentWorld::getNachBlaster() const
{
    return m_nachBlaster;
}

int StudentWorld::getNumAliensDestroyed() const
{
    return m_aliensDestroyed;
}

void StudentWorld::incrementNumAliensDestroyed()
{
    m_aliensDestroyed++;
    cout<< "Alien destroyed" << endl;
}

void StudentWorld::decrementNumAliens()
{
    --m_numAliens;
}


