#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

class Actor;
class Goodie;
class NachenBlaster;

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
    bool hitNachBlaster(Actor* colliding, int hp);
    bool hitDamageableActors(Actor* colliding, int hp);
    bool goodieReceived(Goodie* goodie);
    bool shipCollision(Actor* alien, NachenBlaster* nach);
private:
    std::vector<Actor*> m_actors;
    int m_aliensDestroyed;
    int m_numAliens;
    int m_numActors;
    NachenBlaster* m_nachBlaster;
};

#endif // STUDENTWORLD_H_
