#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

class Actor;
class NachenBlaster;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    NachenBlaster* getNachBlaster() const;
    void addActor(Actor* actor);
    int getNumAliensDestroyed() const;
    void incrementNumAliensDestroyed();
    void decrementNumAliens();
    bool hitDamageableActors(Actor* colliding, int hp);
    bool shipCollision(Actor* alien, NachenBlaster* nach);
private:
    std::vector<Actor*> m_actors;
    int m_numActors;
    int m_aliensDestroyed;
    int m_numAliens;
    NachenBlaster* m_nachBlaster;
};

#endif // STUDENTWORLD_H_
