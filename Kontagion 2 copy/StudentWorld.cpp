#include "StudentWorld.h"
#include "GameConstants.h"


using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    
    m_socrates=new Socrates(0, VIEW_HEIGHT/2,this);
    
    addPits();
    addFood();
    addDirt();
    EColi *e=new EColi(VIEW_WIDTH/2+30,VIEW_HEIGHT/2,this);
    Salmonella *s=new RegularSalmonella(VIEW_WIDTH/2+30,VIEW_HEIGHT/2,this);
    addActor(s);
    addActor(e);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if(!m_socrates->Actor::isDead())
        m_socrates->doSomething();
    if(m_socrates->Actor::isDead())
        return GWSTATUS_PLAYER_DIED;
    list<Actor*>::iterator it;
    for(it= m_actors.begin();it!=m_actors.end();it++)
    {
        if(!(*it)->isDead())
            (*it)->doSomething();
        if(m_socrates->Actor::isDead())
            return GWSTATUS_PLAYER_DIED;
        //check if complete level
    }
    
    //check deaths
    for(it= m_actors.begin();it!=m_actors.end();it++)
    {
        if((*it)->isDead())
        {
            delete *it;
            m_actors.erase(it);
        }
    }
    
    addFungus();
    addGoodie();
    
    string gameStat="Score: ";
    string score=to_string(GameWorld::getScore());
    int index=0;
    if(score[0]=='-')
    {
        gameStat+='-';
        index++;
    }
    for (int i=0;i<6-score.size();++i)
        gameStat+="0";
    
    gameStat+=score.substr(index);
    gameStat+="  Level: "+to_string(GameWorld::getLevel());
    gameStat+="  Lives: "+to_string(GameWorld::getLives());
    gameStat+="  health: "+to_string(m_socrates->Agent::numHitPoints());
    gameStat+="  Sprays: "+to_string(m_socrates->numSprays());
    gameStat+="  Flames: "+to_string(m_socrates->numFlames());
    GameWorld::setGameStatText(gameStat);
    
    
    //continue
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_socrates;
    m_socrates=nullptr;
    list<Actor*>::iterator it;
    for(it= m_actors.begin();it!=m_actors.end();it++)
    {
        delete *it;
        m_actors.erase(it);
    }
}
void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}
bool StudentWorld::damageOneActor(Actor* a, int damage)
{
    list<Actor*>::iterator it;
    for(it= m_actors.begin();it!=m_actors.end();it++)
    {
        if(getOverlap(a,(*it)->getX(),(*it)->getY()) && (*it)->takeDamage(damage))
            return true;
    }
    return false;
}

// Is bacterium a blocked from moving to the indicated location?
bool StudentWorld::isBacteriumMovementBlockedAt(Actor* a, double x, double y) const
{
    list<Actor*>::const_iterator it;
    for(it= m_actors.begin();it!=m_actors.end();it++)
    {
        if(getDistance(x, y, (*it)->getX(), (*it)->getY())<=SPRITE_WIDTH/2 && (*it)->blocksBacteriumMovement())
        {
            return true;
        }
    }
    std::cout<<"canMove"<<std::endl;
    return false;
}

// If actor a overlaps this world's socrates, return a pointer to the
// socrates; otherwise, return nullptr
Socrates* StudentWorld::getOverlappingSocrates(Actor* a) const
{
    if(getOverlap(a, m_socrates->getX(),m_socrates->getY()))
        return m_socrates;
    return nullptr;
}

// If actor a overlaps a living edible object, return a pointer to the
// edible object; otherwise, return nullptr
Actor* StudentWorld::getOverlappingEdible(Actor* a) const
{
    
    list<Actor*>::const_iterator it;
    for(it=m_actors.begin();it!=m_actors.end();it++)
    {
        if(getOverlap(a, (*it)->getX(),(*it)->getY())&& (*it)->isEdible())
           return (*it);
    }
    return nullptr;
}

// Return true if this world's socrates is within the indicated distance
// of actor a; otherwise false.  If true, angle will be set to the
// direction from actor a to the socrates.
bool StudentWorld::getAngleToNearbySocrates(Actor* a, int dist, int& angle) const
{
    return getAngleBetweenActors(a, m_socrates, dist, angle);
}

// Return true if there is a living edible object within the indicated
// distance from actor a; otherwise false.  If true, angle will be set
// to the direction from actor a to the edible object nearest to it.
bool StudentWorld::getAngleToNearestNearbyEdible(Actor* a, int dist, int& angle) const
{
    list<Actor*>::const_iterator it;
    for(it=m_actors.begin();it!=m_actors.end();it++)
    {
        if(getDistance(a->getX(), a->getY(), (*it)->getX(), (*it)->getY())<=128 && (*it)->isEdible())
           return getAngleBetweenActors(a, (*it), dist, angle);
    }
    return false;
}

// Set x and y to the position on the circumference of the Petri dish
// at the indicated angle from the center.  (The circumference is
// where socrates and goodies are placed.)
void StudentWorld::getPositionOnCircumference(int angle, double& x, double& y) const
{
    x=cos(angle*PI/180)*VIEW_RADIUS+VIEW_WIDTH/2;
    y=sin(angle*PI/180)*VIEW_RADIUS+VIEW_HEIGHT/2;
}
double StudentWorld::getDistance(double x1,double y1, double x2, double y2) const
{
    return (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}



//PRIVATE
void StudentWorld::generateRandCoord(double &x, double &y)
{
    int theta=randInt(0, 360);
    double r= 120*sqrt(randInt(0,1000000)/1000000.0);
    x = r*cos(theta*PI/180);
    y = r*sin(theta*PI/180);


}
void StudentWorld::checkInitOverlap(double &x, double &y)
{

    bool doesNotOverlap=false;
    list<Actor*>::const_iterator it;
    
    while(!doesNotOverlap)
    {
        doesNotOverlap=true;
        generateRandCoord(x, y);
        for(it= m_actors.begin();it!=m_actors.end();it++)
        {
            if(getOverlap((*it), x+VIEW_WIDTH/2, y+VIEW_HEIGHT/2)&&!(*it)->blocksBacteriumMovement())
            {
                doesNotOverlap=false;
                break;
                //std::cout<<"overlap"<<std::endl;
            }
        }
    }
}

void StudentWorld::addPits()
{
    double x,y;
    for (int i=0;i<getLevel();++i)
    {
        checkInitOverlap(x, y);
        Pit *p=new Pit(x+VIEW_WIDTH/2,y+VIEW_HEIGHT/2,this);
        addActor(p);
        
    }
}

void StudentWorld::addFood()
{
    double x,y;
    for (int i=0;i<min(5*getLevel(),25);i++)
    {
        checkInitOverlap(x, y);
        Food *f=new Food(x+VIEW_WIDTH/2,y+VIEW_HEIGHT/2,this);
        addActor(f);
        
    }
}


void StudentWorld::addDirt()
{
    for (int i=0;i<max(180-20*getLevel(),20);i++)
    {
        double x,y;
        checkInitOverlap(x, y);
        Dirt *dirtPtr=new Dirt(x+VIEW_WIDTH/2,y+VIEW_HEIGHT/2,this);
        addActor(dirtPtr);
    }
}

void StudentWorld::addFungus()
{
    int chanceFungus=max(510-GameWorld::getLevel()*10,200)-1;
    if(randInt(0,chanceFungus)==0)
    {
        double x,y;
        int angle =randInt(0, 359);
        getPositionOnCircumference(angle, x, y);
        Fungus *f= new Fungus(x, y,this);
        addActor(f);
    }
}
void StudentWorld::addGoodie()
{
    int chanceGoodie=max(510-GameWorld::getLevel()*10,250)-1;
    if(randInt(0,chanceGoodie)==0)
    {
        double x,y;
        int angle =randInt(0, 359);
        getPositionOnCircumference(angle, x, y);
        int chance=randInt(0, 9);
        Goodie *g;
        if(chance==0)
            g=new ExtraLifeGoodie(x,y,this);
        else if(chance<=3&&chance>=1)
            g=new FlamethrowerGoodie(x,y,this);
        else
            g= new RestoreHealthGoodie(x, y,this);
        
        addActor(g);
    }
}

bool StudentWorld::getAngleBetweenActors(Actor* a, Actor *b, int dist, int& angle) const
{
    if(getDistance(a->getX(), a->getY(), b->getX(), b->getY())<=dist)
    {
        double x=b->getX()-a->getX();
        double y=b->getY()-a->getY();
        double local_angle=atan2(y,x);
        angle=local_angle*180/PI;
            
        return true;
    }
    return false;
}



bool StudentWorld::getOverlap(Actor *a, double x, double y) const
{
     if(getDistance( a->GraphObject::getX(), a->GraphObject::getY(), x, y) <= SPRITE_WIDTH)
         return true;
    return false;
}


