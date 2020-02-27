#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>
#include <cmath>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Socrates;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    // Add an actor to the world.
    void addActor(Actor* a);

    // If actor a ovelaps some live actor, damage that live actor by the
    // indicated amount of damage and return true; otherwise, return false.
    bool damageOneActor(Actor* a, int damage);

    // Is bacterium a blocked from moving to the indicated location?
    bool isBacteriumMovementBlockedAt(Actor* a, double x, double y) const;

    // If actor a overlaps this world's socrates, return a pointer to the
    // socrates; otherwise, return nullptr
    Socrates* getOverlappingSocrates(Actor* a) const;

    // If actor a overlaps a living edible object, return a pointer to the
    // edible object; otherwise, return nullptr
    Actor* getOverlappingEdible(Actor* a) const;

    // Return true if this world's socrates is within the indicated distance
    // of actor a; otherwise false.  If true, angle will be set to the
    // direction from actor a to the socrates.
    bool getAngleToNearbySocrates(Actor* a, int dist, int& angle) const;

    // Return true if there is a living edible object within the indicated
    // distance from actor a; otherwise false.  If true, angle will be set
    // to the direction from actor a to the edible object nearest to it.
    bool getAngleToNearestNearbyEdible(Actor* a, int dist, int& angle) const;
    
    // Set x and y to the position on the circumference of the Petri dish
    // at the indicated angle from the center.  (The circumference is
    // where socrates and goodies are placed.)
    void getPositionOnCircumference(int angle, double& x, double& y) const;
    

private:
    Socrates *m_socrates;
    std::list<Actor*> m_actors;
    
    
    double getDistance(double x1,double y1, double x2, double y2) const;
    void addDirt();
    void addFungus();
    void addGoodie();
};

#endif // STUDENTWORLD_H_
