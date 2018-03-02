#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

// helper funcitons
double euclideanDistance(int x1, int x2, int y1, int y2);
bool collisionOccurred(Actor* one, Actor* two);

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), m_numActors(0), m_aliensDestroyed(0)
{ }

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    for (int i = 0; i < 30; i++)
        addActor(new Star(randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1), this));
    m_nachBlaster = new NachenBlaster(this);
    m_numActors++;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    int aliensMustBeDestroyed = 6 + (4 * getLevel());
    
    // for each actor, do something
    for (int i = 0; i < m_numActors; i++) {
        Actor* actor;
        (i < m_numActors - 1) ? actor = m_actors[i] : actor = m_nachBlaster;
        if (!actor->isDead())
            actor->doSomething();
        if (m_nachBlaster->isDead()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (m_aliensDestroyed == aliensMustBeDestroyed) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
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
    if (randInt(1, 15) == 15)
        addActor(new Star(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this));
    
    // place aliens
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
        else if (s1 < probability && probability < s1 + s2)
            newAlien = new Smoregon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this);
        else
            newAlien = new Snagglegon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this);
        // add new ship
        addActor(newAlien);
        m_numAliens++;
    }
    
    // update display text
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(2);
    oss << "Lives: " << getLives() << "  ";
    oss << "Health: " << (m_nachBlaster->getHP() * 2) << "%  ";
    oss << "Score: " << getScore() << "  ";
    oss << "Level: " << getLevel() << "  ";
    oss << "Cabbages: " << m_nachBlaster->getCabbagePercent() << "%  ";
    oss << "Torpedoes: " << m_nachBlaster->getNumTorpedoes();
    setGameStatText(oss.str());
    
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
    m_numAliens = 0;
    m_aliensDestroyed = 0;
}

NachenBlaster* StudentWorld::getNachBlaster() const
{
    return m_nachBlaster;
}

void StudentWorld::addActor(Actor* actor)
{
    m_actors.push_back(actor);
    m_numActors++;
}

int StudentWorld::getNumAliensDestroyed() const
{
    return m_aliensDestroyed;
}

void StudentWorld::incrementNumAliensDestroyed()
{
    m_aliensDestroyed++;
}

void StudentWorld::decrementNumAliens()
{
    --m_numAliens;
}

// handles if a enemy projectile hits the NachenBlaster
bool StudentWorld::hitNachBlaster(Actor* colliding, int hp)
{
    vector<Actor*>::iterator p;
    if (collisionOccurred(colliding, m_nachBlaster)) {
        m_nachBlaster->sufferDamage(hp);
        if (m_nachBlaster->getHP() <= 0)
            m_nachBlaster->setDead();
        playSound(SOUND_BLAST);
        colliding->setDead();
        return true;
    }
    return false;
}

// handles if a NachenBlaster Cabbage hits an alien ship
bool StudentWorld::hitDamageableActors(Actor* colliding, int hp)
{
    vector<Actor*>::iterator p;
    for (p = m_actors.begin(); p != m_actors.end(); p++) {
        if (collisionOccurred(*p, colliding) && (*p)->isDamageable()) {
            (*p)->sufferDamage(hp);
            if ((*p)->getHP() < 0) {
                increaseScore((*p)->getScore());
                (*p)->setDead();
                (*p)->dropSomething();
                incrementNumAliensDestroyed();
                decrementNumAliens();
                playSound(SOUND_DEATH);
                addActor(new Explosion((*p)->getX(), (*p)->getY(), this));
            } else {
                playSound(SOUND_BLAST);
            }
            colliding->setDead();
            return true;
        }
    }
    return false;
}

// handles if the NachenBlaster recieves a goodie
bool StudentWorld::goodieReceived(Goodie* goodie)
{
    if (collisionOccurred(goodie, m_nachBlaster)) {
        increaseScore(100);
        goodie->setDead();
        goodie->specialized();
        playSound(SOUND_GOODIE);
        return true;
    }
    return false;
}

// handles if ships collide
bool StudentWorld::shipCollision(Actor* alien, NachenBlaster* nach)
{
    if (collisionOccurred(alien, nach)) {
        m_nachBlaster->sufferDamage(10);
        if (m_nachBlaster->getHP() <= 0)
            m_nachBlaster->setDead();
        alien->setDead();
        alien->dropSomething();
        
        increaseScore(alien->getScore());
        playSound(SOUND_DEATH);
        incrementNumAliensDestroyed();
        decrementNumAliens();
        addActor(new Explosion(alien->getX(), alien->getY(), this));
        return true;
    }
    return false;
}


// helper function implementation
double euclideanDistance(int x1, int x2, int y1, int y2)
{
    double x = x1 - x2;
    double y = y1 - y2;
    return sqrt(pow(x, 2) + pow(y, 2));
}

bool collisionOccurred(Actor* one, Actor* two)
{
    return (euclideanDistance(one->getX(), two->getX(),
                              one->getY(), two->getY())
            < 0.75 * (one->getRadius() + two->getRadius()));
}
