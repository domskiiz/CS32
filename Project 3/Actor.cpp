#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>

// MISC CONSTS
const int       DIRECTION_RIGHT = 0;
const int       DIRECTION_LEFT = 180;

const int       DEPTH_STAR = 3;

const double    EXPLOSION_SIZE = 1;
const int       EXPLOSION_DEPTH = 0;

// NACHENBLASTER CONST
const int       NACHENBLASTER_X = 0;
const int       NACHENBLASTER_Y = 128;
const double    NACHENBLASTER_SIZE = 1.0;
const int       NACHENBLASTER_DEPTH = 0;
const double    NACHENBLASTER_HP = 50;

// PROJECTILE CONST
const double    PROJECTILE_SIZE = 0.5;
const int       PROJECTILE_DEPTH = 1;

const int       CABBAGE_DAMAGE = 2;
const int       TURNIP_DAMAGE = 2;
const int       TORPEDO_DAMAGE = 8;

// ALIEN CONST
const double    ALIEN_SIZE = 1.5;
const int       ALIEN_DEPTH = 1;

const int       FLIGHT_LEFT = 0;
const int       FLIGHT_UP_LEFT = 1;
const int       FLIGHT_DOWN_LEFT = 2;

const int       SMALLGON_SCORE = 250;
const int       SMOREGON_SCORE = 250;
const int       SNAGGLEGON_SCORE = 1000;

// GOODIE CONST
const double    GOODIE_SIZE = 0.5;
const int       GOODIE_DEPTH = 1;

//////////////////////////
// ACTOR IMPLEMENTATION //
//////////////////////////

Actor::Actor(int imageId, double startX, double startY, int startDirection, double size, int depth, StudentWorld* world, bool damageable)
: GraphObject(imageId, startX, startY, startDirection, size, depth)
{
    m_dead = false;
    m_world = world;
    m_damageable = damageable;
}

Actor::~Actor()
{ }

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

int Actor::getScore() const
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
    if (getCount() < 4)
        incCount();
    else
        setDead();
}

int Explosion::getCount() const
{
    return count;
}

void Explosion::incCount()
{
    count++;
}

///////////////////////////////
// PROJECTILE IMPLEMENTATION //
///////////////////////////////
Projectile::Projectile(int id, int x, int y, StudentWorld* world, int direction)
: Actor(id,
        x,
        y,
        direction,
        PROJECTILE_SIZE,
        PROJECTILE_DEPTH,
        world,
        false)
{
    m_direction = 0;
}

Projectile::~Projectile()
{ }

int Projectile::getDirection() const
{
    return m_direction;
}

void Projectile::setThisDirection(int dir)
{
    m_direction += dir;
}

////////////////////////////
// CABBAGE IMPLEMENTATION //
////////////////////////////
Cabbage::Cabbage(int x, int y, StudentWorld* world)
        :Projectile(IID_CABBAGE, x, y, world, DIRECTION_RIGHT)
{ }

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
    setThisDirection(20);
    setDirection(getDirection());
    if (getWorld()->hitDamageableActors(this, CABBAGE_DAMAGE))
        return;
}

///////////////////////////
// TURNIP IMPLEMENTATION //
///////////////////////////
Turnip::Turnip(int x, int y, StudentWorld* world)
:Projectile(IID_TURNIP, x, y, world, DIRECTION_RIGHT)
{ }

Turnip::~Turnip()
{ }

void Turnip::doSomething()
{
    if (isDead()) return;
    if (getX() < 0) {
        setDead();
        return;
    }
    if (getWorld()->hitNachBlaster(this, TURNIP_DAMAGE))
        return;
    moveTo(getX() - 6, getY());
    setThisDirection(20);
    setDirection(getDirection());
    if (getWorld()->hitNachBlaster(this, TURNIP_DAMAGE))
        return;
}

////////////////////////////
// TORPEDO IMPLEMENTATION //
////////////////////////////
Torpedo::Torpedo(int x, int y, StudentWorld* world, int direction)
:Projectile(IID_TORPEDO, x, y, world, direction)
{ }

Torpedo::~Torpedo()
{ }

void Torpedo::doSomething()
{
    if (isDead()) return;
    if (getX() < 0 || getX() > VIEW_WIDTH) {
        setDead();
        return;
    }
    specializedAttack();
}

AlienTorpedo::AlienTorpedo(int x, int y, StudentWorld* world)
:Torpedo(x, y, world, DIRECTION_LEFT)
{ }

AlienTorpedo::~AlienTorpedo()
{ }

void AlienTorpedo::specializedAttack()
{
    if (getWorld()->hitNachBlaster(this, TORPEDO_DAMAGE))
        return;
    moveTo(getX() - 8, getY());
    if (getWorld()->hitNachBlaster(this, TORPEDO_DAMAGE))
        return;

}

NachTorpedo::NachTorpedo(int x, int y, StudentWorld* world)
:Torpedo(x, y, world, DIRECTION_RIGHT)
{ }

NachTorpedo::~NachTorpedo()
{ }

void NachTorpedo::specializedAttack()
{
    if (getWorld()->hitDamageableActors(this, TORPEDO_DAMAGE))
        return;
    moveTo(getX() + 8, getY());
    if (getWorld()->hitDamageableActors(this, TORPEDO_DAMAGE))
        return;
}

Ship::Ship(int x, int y, StudentWorld* world, double hp, int id, double size, int depth)
:Actor(id,
      x,
      y,
      DIRECTION_RIGHT,
      size,
      depth,
      world,
      true)
{
    m_hp = hp;
}

Ship::~Ship()
{ }

void Ship::sufferDamage(int hp)
{
    m_hp -= hp;
}

double Ship::getHP() const
{
    return m_hp;
}

void Ship::addHP(int hp)
{
    m_hp += hp;
}

void Ship::setHP(int hp)
{
    m_hp = hp;
}



//////////////////////////////////
// NACHENBLASTER IMPLEMENTATION //
//////////////////////////////////

NachenBlaster::NachenBlaster(StudentWorld* world)
: Ship(NACHENBLASTER_X,
       NACHENBLASTER_Y,
       world,
       NACHENBLASTER_HP,
       IID_NACHENBLASTER,
       NACHENBLASTER_SIZE,
       NACHENBLASTER_DEPTH)
{
    m_cabbageEnergyPoints = 30;
    m_nachTorpedoes = 0;
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
                    break;
                } else break;
            case KEY_PRESS_TAB:
                if (m_nachTorpedoes > 0) {
                    getWorld()->addActor(new NachTorpedo(getX() + 12, getY(), getWorld()));
                    decTorpedoes();
                    getWorld()->playSound(SOUND_TORPEDO);
                    break;
                } else break;
            default:
                break;
        }
    }
    if (m_cabbageEnergyPoints < 30)
        m_cabbageEnergyPoints++;
}


void NachenBlaster::increaseHP(int hp)
{
    if (getHP() + hp <= 50) {
        addHP(hp);
    } else {
        int add = 50 - getHP();
        addHP(add);
    }
}

int NachenBlaster::getCabbagePercent() const
{
    return (m_cabbageEnergyPoints / 30.0) * 100;
}

int NachenBlaster::getNumTorpedoes() const
{
    return m_nachTorpedoes;
}

void NachenBlaster::setNumTorpedoes(int num)
{
    m_nachTorpedoes += num;
}

void NachenBlaster::decTorpedoes()
{
    m_nachTorpedoes--;
}

//////////////////////////
// ALIEN IMPLEMENTATION //
//////////////////////////
Alien::Alien(int id, int x, int y, StudentWorld* world, double hp, int flightPlan, double travelSpeed, int score)
: Ship(x,
       y,
       world,
       hp,
       id,
       ALIEN_SIZE,
       ALIEN_DEPTH)
{
    m_flightPlan = flightPlan;
    m_travelSpeed = travelSpeed;
    m_scorePoints = score;
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
    
    specializedAttack();

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

void Alien::setFlightDirection(int dir)
{
    m_flightDirection = dir;
}

int Alien::getFlightDirection() const
{
    return m_flightDirection;
}

void Alien::setTravelSpeed(double speed)
{
    m_travelSpeed = speed;
}

double Alien::getTravelSpeed() const
{
    return m_travelSpeed;
}

int Alien::getScore() const
{
    return m_scorePoints;
}


/////////////////////////////
// SMALLGON IMPLEMENTATION //
/////////////////////////////
Smallgon::Smallgon(int x, int y, StudentWorld* world)
: Alien(IID_SMALLGON, x, y, world, 0, 0, 2.0, SMALLGON_SCORE)
{
    setHP(5 * (1 + (getWorld()->getLevel() - 1) * .1));
}

Smallgon::~Smallgon()
{ }

void Smallgon::specializedAttack()
{
    if (getWorld()->getNachBlaster()->getX() < getX()
        && getWorld()->getNachBlaster()->getY() >= getY() - 4
        && getWorld()->getNachBlaster()->getY() <= getY() + 4)
    {
        int probability = randInt(1, (20/getWorld()->getLevel()) + 5);
        if (probability == 1) {
            getWorld()->addActor(new Turnip(getX() - 14, getY(), getWorld()));
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return;
        }
    }
}

void Smallgon::dropSomething()
{ }


/////////////////////////////
// SMOREGON IMPLEMENTATION //
/////////////////////////////
Smoregon::Smoregon(int x, int y, StudentWorld* world)
: Alien(IID_SMOREGON, x, y, world, 0, 0, 2.0, SMOREGON_SCORE)
{
    setHP(5 * (1 + (getWorld()->getLevel() - 1) * .1));
}

Smoregon::~Smoregon()
{ }

void Smoregon::specializedAttack()
{
    if (getWorld()->getNachBlaster()->getX() < getX()
        && getWorld()->getNachBlaster()->getY() >= getY() - 4
        && getWorld()->getNachBlaster()->getY() <= getY() + 4)
    {
        int probability = randInt(1, (20/getWorld()->getLevel()) + 5);
        if (probability == 1) {
            getWorld()->addActor(new Turnip(getX() - 14, getY(), getWorld()));
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return;
        }
        int probability2 = randInt(1, (20/getWorld()->getLevel()) + 5);
        if (probability2 == 1) {
            setFlightDirection(FLIGHT_LEFT);
            setFlightPlan(VIEW_WIDTH);
            setTravelSpeed(5.0);
        }
    }
}

void Smoregon::dropSomething()
{
    int probabilityDrop = randInt(1, 3);
    int probabilityKind = randInt(1, 2);
    if (probabilityDrop == 1) {
        if (probabilityKind == 1)
            getWorld()->addActor(new RepairGoodie(getX(), getY(), getWorld()));
        else
            getWorld()->addActor(new TorpedoGoodie(getX(), getY(), getWorld()));
    }
}



///////////////////////////////
// SNAGGLEGON IMPLEMENTATION //
///////////////////////////////
Snagglegon::Snagglegon(int x, int y, StudentWorld* world)
: Alien(IID_SNAGGLEGON, x, y, world, 0, 0, 1.75, SNAGGLEGON_SCORE)
{
    setHP(10 * (1 + (getWorld()->getLevel() - 1) * .1));
}

Snagglegon::~Snagglegon()
{ }

void Snagglegon::specializedAttack()
{
    if (getWorld()->getNachBlaster()->getX() < getX()
        && getWorld()->getNachBlaster()->getY() >= getY() - 4
        && getWorld()->getNachBlaster()->getY() <= getY() + 4)
    {
        int probability = randInt(1, (15/getWorld()->getLevel()) + 10);
        if (probability == 1) {
            getWorld()->addActor(new AlienTorpedo(getX() - 14, getY(), getWorld()));
            getWorld()->playSound(SOUND_TORPEDO);
            return;
        }
    }
}

void Snagglegon::dropSomething()
{
    int probabilityDrop = randInt(1, 6);
    if (probabilityDrop == 1) {
        getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
    }
}


///////////////////////////
// GOODIE IMPLEMENTATION //
///////////////////////////
Goodie::Goodie(int x, int y, StudentWorld* world, int id)
:Actor(id,
       x,
       y,
       DIRECTION_RIGHT,
       GOODIE_SIZE,
       GOODIE_DEPTH,
       world,
       false)
{ }

Goodie::~Goodie()
{ }

void Goodie::doSomething()
{
    if (isDead()) return;
    if (getX() < 0 || getY() < 0) {
        setDead();
        return;
    }
    if (getWorld()->goodieReceived(this))
        return;
    moveTo(getX() - 0.75, getY() - 0.75);
    if (getWorld()->goodieReceived(this))
        return;
}

ExtraLifeGoodie::ExtraLifeGoodie(int x, int y, StudentWorld* world)
:Goodie(x, y, world, IID_LIFE_GOODIE)
{ }

ExtraLifeGoodie::~ExtraLifeGoodie()
{ }

void ExtraLifeGoodie::specialized()
{
    getWorld()->incLives();
}

RepairGoodie::RepairGoodie(int x, int y, StudentWorld* world)
:Goodie(x, y, world, IID_REPAIR_GOODIE)
{ }

RepairGoodie::~RepairGoodie()
{ }

void RepairGoodie::specialized()
{
    getWorld()->getNachBlaster()->increaseHP(10);
}

TorpedoGoodie::TorpedoGoodie(int x, int y, StudentWorld* world)
:Goodie(x, y, world, IID_TORPEDO_GOODIE)
{ }

TorpedoGoodie::~TorpedoGoodie()
{ }

void TorpedoGoodie::specialized()
{
    getWorld()->getNachBlaster()->setNumTorpedoes(5);
}


