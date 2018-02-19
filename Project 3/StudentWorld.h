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
    // Responsible for constructing a representation of the current level in your
    // StudentWorld object and populating it with initial objects (e.g., stars),
    // using one or more data structures that you come up with.
    virtual int init();
    
    // Each time the move() method is called, it must run a single tick of the game.
    //      1. Ask actors to do something
    //      2. Disposing actors that must be deleted
    virtual int move();
    
    // Responsible for freeing all actors that are currently in the game
    virtual void cleanUp();

private:
    std::vector<Actor*> m_actors;
    int m_numActors;
    NachenBlaster* m_nachBlaster;
};

#endif // STUDENTWORLD_H_
