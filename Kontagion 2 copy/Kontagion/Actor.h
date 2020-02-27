#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm>
const double PI=atan(1)*4;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor: public GraphObject
{
public:
    Actor(int imageID,int startX, int startY, Direction startDirection, int depth, StudentWorld  *w)
    :GraphObject(imageID,startX,startY,startDirection,depth)
    {
        m_isAlive=true;
        m_world=w;
    }
    virtual void doSomething() = 0;

      // Is this actor dead?
    bool isDead() const;
    
      // Mark this actor as dead.
    void setDead();

      // Get this actor's world
    StudentWorld* world() const;

      // If this actor can suffer damage, make it do so and return true;
      // otherwise, return false.
    virtual bool takeDamage(int damage);

      // Does this object block bacterium movement?
    virtual bool blocksBacteriumMovement() const;

      // Is this object edible?
    virtual bool isEdible() const;

      // Does the existence of this object prevent a level from being completed?
    virtual bool preventsLevelCompleting() const;
    
private:
    bool m_isAlive;
    StudentWorld *m_world;
    
};
class Agent : public Actor
{
public:
    Agent(int imageID, double x, double y, int dir, int hitPoints, StudentWorld* w)
    :Actor(imageID,x,y,dir,0,w)
    {
        m_maxHealth=hitPoints;
        m_currentHealth=hitPoints;
    }
    virtual bool takeDamage(int damage);

      // How many hit points does this agent currently have?
    int numHitPoints() const;

      // Restore this agent's hit points to their original level
    void restoreHealth();

      // What sound should play when this agent is damaged but does not die?
    virtual int soundWhenHurt() const = 0;

      // What sound should play when this agent is damaged and dies?
    virtual int soundWhenDie() const = 0;
private:
    int m_maxHealth;
    int m_currentHealth;
};

class Dirt: public Actor
{
public:
    Dirt(int startX, int startY,StudentWorld *w)
    :Actor(IID_DIRT,startX,startY,0,1,w)
    {
        
    }
    virtual void doSomething();
    virtual bool takeDamage(int);
    virtual bool blocksBacteriumMovement() const;
};
class Socrates: public Agent
 {
 public:
     Socrates(double x, double y, StudentWorld *studentWorldPtr)
     :Agent(IID_PLAYER,x,y,0,100,studentWorldPtr)
     {
         m_hp=100;
         m_sprayCharges=20;
         m_flameCharges=5;
         m_posAngle =180;
         m_world=studentWorldPtr;
     }
     virtual void doSomething();
     // Increase the number of flamethrower charges the object has.
    void addFlames();

    // How many flamethrower charges does the object have?
    int numFlames() const;

    // How many spray charges does the object have?
    int numSprays() const;
    virtual int soundWhenHurt() const;

    // What sound should play when this agent is damaged and dies?
    virtual int soundWhenDie() const;
     
     
 private:
     StudentWorld* getWorldPtr() const;
     void changePosAngle(int degree);
     void adjustCoord(int posAngle, double &x, double &y);
     void moveInCirc(int posAngle, double &x, double &y);
     void makeFlame();
     void makeSpray();
     void getPositionAroundSoc(int dir, double &x, double &y);
     
     int m_hp;
     int m_sprayCharges;
     int m_flameCharges;
     int m_posAngle;
     
     StudentWorld *m_world;
     
     
 

 };

class Food : public Actor
{
public:
    Food(double x, double y, StudentWorld* w)
    :Actor(IID_FOOD,x,y,90,1,w)
    {
    }
    virtual void doSomething();
    virtual bool isEdible() const;
};


//12345

class Pit : public Actor
{
public:
    Pit(double x, double y,StudentWorld* w);
    virtual void doSomething();
    virtual bool preventsLevelCompleting() const;
};

class Projectile : public Actor
{
public:
    Projectile(int imageID, double x, double y, int dir, int dist, int dmg,StudentWorld* w)
    :Actor(imageID,x, y, dir, 1, w)
    {
        m_travelDist=0;
        m_totalDist=dist;
        m_dmg=dmg;
    }
    virtual void doSomething();
    
    bool travel();
    //return true if dist is reached, false if still travelling
private:
    int m_travelDist;
    int m_totalDist;
    int m_dmg;
};

class Spray : public Projectile
{
public:
    Spray(double x, double y, int dir,StudentWorld* w)
    :Projectile(IID_SPRAY,x,y,dir,112,2,w)
    {
        
    }
};

class Flame : public Projectile
{
public:
    Flame(double x, double y, int dir, StudentWorld* w)
    :Projectile(IID_FLAME,x,y,dir,32,5,w)
    {
        
    }
};

class Goodie : public Actor
{
public:
    Goodie(int imageID, double x, double y, StudentWorld* w);
    virtual bool takeDamage(int);

      // Have s pick up this goodie.
    virtual void pickUp(Socrates* s) = 0;
    virtual void doSomething();

    
private:
    int m_lifetime;
    
    int lifetime();
    bool decLifeTime();
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(double x, double y,StudentWorld* w)
    :Goodie(IID_RESTORE_HEALTH_GOODIE,x,y,w)
    {
        
    }
    virtual ~RestoreHealthGoodie(){};
    virtual void pickUp(Socrates* s);

private:
};

class FlamethrowerGoodie : public Goodie
{
public:
    FlamethrowerGoodie(double x, double y,StudentWorld* w)
    :Goodie(IID_FLAME_THROWER_GOODIE,x,y,w)
    {
        
    }
    virtual ~FlamethrowerGoodie(){};
    virtual void pickUp(Socrates* s);
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(double x, double y,StudentWorld* w)
    :Goodie(IID_EXTRA_LIFE_GOODIE,x,y,w)
    {
        
    }
    virtual ~ExtraLifeGoodie(){};
    virtual void pickUp(Socrates* s);
};

class Fungus : public Goodie
{
public:
    Fungus(double x, double y,StudentWorld* w)
    :Goodie(IID_FUNGUS, x, y,w)
    {
        
    }
    virtual ~Fungus(){};
    virtual void pickUp(Socrates* s);
};




class Bacterium : public Agent
{
public:
    Bacterium(int imageID, double x, double y, int hitPoints, StudentWorld* w);
    virtual bool takeDamage(int damage);
    virtual bool preventsLevelCompleting() const;
};

class EColi : public Bacterium
{
public:
    EColi(double x, double y, StudentWorld* w);
    virtual void doSomething();
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
};

class Salmonella : public Bacterium
{
public:
    Salmonella(double x, double y, int hitPoints, StudentWorld* w);
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
};

class RegularSalmonella : public Salmonella
{
public:
    RegularSalmonella(double x, double y, StudentWorld* w);
    virtual void doSomething();
};

class AggressiveSalmonella : public Salmonella
{
public:
    AggressiveSalmonella(double x, double y, StudentWorld* w);
    virtual void doSomething();
};
#endif // ACTOR_H_
