#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), m_numActors(0)
{
}

StudentWorld::~StudentWorld()
{
}

int StudentWorld::init()
{
    for (int i = 0; i < 30; i++) {
        m_actors.push_back(new Star(this));
        m_numActors++;
    }
    m_nachBlaster = new NachenBlaster(this);
    m_numActors++;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
//    for (int i = 0; i < m_numActors; i++) {
//        Actor* actor;
//        (i < m_numActors - 1) ? actor = m_actors[i] : actor = m_nachBlaster;
//        if (!actor->isDead())
//            actor->doSomething();
//        if (m_nachBlaster->isDead())
//            return GWSTATUS_PLAYER_DIED;
//    }
//    vector<Actor*>::iterator it;
//    it = m_actors.begin();
//    while (it != m_actors.end()) {
//        if ((*it)->isDead()) {
//            delete *it;
//            it = m_actors.erase(it);
//            m_numActors--;
//        } else {
//            it++;
//        }
//    }
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        delete *it;
        it = m_actors.erase(it);
    }
    delete m_nachBlaster;
    m_numActors = 0;
}
