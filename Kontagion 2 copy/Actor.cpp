#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

/*Try to get user input (if any is available)
 If the user pressed the LEFT key then
 Move counterclockwise around the perimeter by 5 degrees
 If the user pressed the RIGHT key then
Move clockwise around the perimeter by 5 degrees
 ...
 If the user pressed space and Socrates has Sprays left, then
Introduce a new Spray object into the Petri dish in front
 of Socrates
 ...
 **/
void Socrates::doSomething()
{
    int ch;
    if(Actor::world()->getKey(ch))
    {
        double x=GraphObject::getX();
        double y=GraphObject::getY();
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                moveInCirc(5, x, y);
                break;
            case KEY_PRESS_RIGHT:
                moveInCirc(-5, x, y);
                break;
            case KEY_PRESS_ENTER:
                if(numFlames()>0)
                {
                    makeFlame();
                    m_flameCharges--;
                    Actor::world()->GameWorld::playSound(SOUND_PLAYER_FIRE);
                }
                break;
            case KEY_PRESS_SPACE:
                if(numSprays()>0)
                {
                    makeSpray();
                    m_sprayCharges--;
                    Actor::world()->GameWorld::playSound(SOUND_PLAYER_SPRAY);
                }
                break;
        }
    }
    else
    {
        if(numSprays()<20)
            m_sprayCharges++;
    }
    return;
}
// Increase the number of flamethrower charges the object has.
void Socrates::addFlames()
{
    m_flameCharges+=5;
}

   // How many flamethrower charges does the object have?
int Socrates::numFlames() const
{
    return m_flameCharges;
}

   // How many spray charges does the object have?
int Socrates::numSprays() const
{
    return m_sprayCharges;
}

int Socrates::soundWhenHurt() const
{
    return SOUND_PLAYER_HURT;
}

  // What sound should play when this agent is damaged and dies?
int Socrates::soundWhenDie() const
{
    return SOUND_PLAYER_DIE;
}






bool Actor::isDead() const
{
    return !m_isAlive;
}
void Actor::setDead()
{
    m_isAlive=false;
}

StudentWorld* Actor::world() const
{
    return m_world;
}

bool Actor::takeDamage(int damage)
{
    return false;
}

  // Does this object block bacterium movement?
bool Actor::blocksBacteriumMovement() const
{
    return false;
}

  // Is this object edible?
bool Actor::isEdible() const
{
    return false;
}

  // Does the existence of this object prevent a level from being completed?
bool Actor::preventsLevelCompleting() const
{
    return false;
}





//private socrates
void Socrates::changePosAngle(int degree)
{
    m_posAngle+=degree;
}
void Socrates::adjustCoord(int posAngle, double &x, double &y)
{
    x=cos(posAngle*PI/180)*VIEW_RADIUS+VIEW_WIDTH/2;
    y=sin(posAngle*PI/180)*VIEW_RADIUS+VIEW_HEIGHT/2;
}
void Socrates::moveInCirc(int posAngle, double &x, double &y)
{
    
    changePosAngle(posAngle);
    Actor::world()->getPositionOnCircumference(m_posAngle, x, y);
    GraphObject::moveTo(x, y);
    GraphObject::setDirection(m_posAngle+180);
}

void Socrates::makeFlame()
{
    for (int i=0;i<16;++i)
    {
        double x,y;
        int flameDir=(Actor::getDirection()+i*22)%360;
        getPositionAroundSoc(flameDir,x,y);
        Flame *f=new Flame(x, y, flameDir, Actor::world());
        Actor::world()->addActor(f);
    }
}

void Socrates::makeSpray()
{
    double x,y;
    int sprayDir=Actor::getDirection();
    getPositionAroundSoc(sprayDir, x, y);
    Spray *s=new Spray(x, y, sprayDir, Actor::world());
    Actor::world()->addActor(s);
    
}

void Socrates::getPositionAroundSoc(int dir, double &x, double &y)
{
    x=cos(dir*PI/180)*SPRITE_WIDTH+GraphObject::getX();
    y=sin(dir*PI/180)*SPRITE_WIDTH+GraphObject::getY();
}


//end of private socrates




void Dirt::doSomething()
{
    return;
}

bool Dirt::takeDamage(int damage)
{
    setDead();
    return true;
    //fix
}

bool Dirt::blocksBacteriumMovement() const
{
    return true;
}






void Food::doSomething()
{
    return;
}
bool Food::isEdible() const
{
    return true;
}



void Pit::doSomething()
{
    if(m_aggrSalmonellaLeft==0 && m_salmonellaLeft==0 && m_ecoliLeft==0)
        setDead();
    if(randInt(0, 49)==0)
    {
        int numTypes=0;
        int typesConsidered [3]={-1, -1,-1};
        for(int i=0,j=0;i<3;++i)
        {
            if(typesLeft[i]==true)
            {
                typesConsidered[j]=i;
                numTypes++;
                j++;
            }
        }
                
        switch(typesConsidered[randInt(0, numTypes)])
        {
            case 0:
                //salmonella
                break;
            case 1:
                //aggr salmonella
                break;
            case 2:
                //ecoli
                break;
            default:
                break;
        }
        
    }
    
    return;
}
bool Pit::preventsLevelCompleting() const
{
    return true;
}



void Projectile::doSomething()
{
    if(Actor::isDead())
        return;
    if(Actor::world()->damageOneActor(this, m_dmg))
    {
        Actor::setDead();
        return;
    }
    if(travel())
        setDead();
    
    
    
}


//private projectile
bool Projectile::travel()
{
    GraphObject::moveAngle(GraphObject::getDirection(), SPRITE_WIDTH);
    m_travelDist+=SPRITE_WIDTH;
    if(m_travelDist>=m_totalDist)
        return true;
    return false;
    
}

bool Goodie::takeDamage(int)
{
    setDead();
    return true;
    //fix
}

Goodie::Goodie(int imageID, double x, double y,StudentWorld* w)
:Actor(imageID,x,y,0,1,w)
{
    //this->GraphObject::getX();
    m_lifetime= std::max(randInt(0,300-10*w->getLevel()-1),50);
}

bool Goodie::decLifeTime()
{
    m_lifetime--;
    if (m_lifetime<=0)
        return true;
    return false;
}
void Goodie::doSomething()
{
    if(Actor::isDead())
        return;
    if(Actor::world()->getOverlappingSocrates(this))
    {
        pickUp(Actor::world()->getOverlappingSocrates(this));
        Actor::setDead();
        return;
    }
    //determine overlap
    
    if(Goodie::decLifeTime())
        Actor::setDead();
    return;
}



void RestoreHealthGoodie::pickUp(Socrates* s)
{
    Actor::world()->GameWorld::increaseScore(250);
    s->Agent::restoreHealth();
    Actor::world()->GameWorld::playSound(SOUND_GOT_GOODIE);
    return;
}


void FlamethrowerGoodie::pickUp(Socrates* s)
{
    Actor::world()->GameWorld::increaseScore(300);
    s->addFlames();
    Actor::world()->GameWorld::playSound(SOUND_GOT_GOODIE);
    return;
}


void ExtraLifeGoodie::pickUp(Socrates* s)
{
    Actor::world()->GameWorld::increaseScore(500);
    Actor::world()->GameWorld::incLives();
    Actor::world()->GameWorld::playSound(SOUND_GOT_GOODIE);
    return;
}


void Fungus::pickUp(Socrates* s)
{
    Actor::world()->GameWorld::increaseScore(-50);
    s->takeDamage(20);
    return;
}



//Agent

bool Agent::takeDamage(int damage)
{
    
    m_currentHealth=m_currentHealth-damage;
    Actor::world()->GameWorld::playSound(soundWhenHurt());
    if(m_currentHealth<=0)
    {
        Actor::world()->GameWorld::playSound(soundWhenDie());
        m_currentHealth=0;
        Actor::setDead();
    }
    return true;
    //fix
}

int Agent::numHitPoints() const
{
    return m_currentHealth;
}

void Agent::restoreHealth()
{
    m_currentHealth=m_maxHealth;
    return;
}





bool Bacterium::takeDamage(int damage)
{
    
    Agent::takeDamage(damage);
    if(Actor::isDead())
    {
        Actor::world()->GameWorld::increaseScore(100);
        if(randInt(0, 1)==0)
        {
            Food *f=new Food(GraphObject::getX(), GraphObject::getY(), Actor::world());
            Actor::world()->addActor(f);
        }
    }
    return true;
}

bool Bacterium::preventsLevelCompleting() const
{
    return true;
}

void Bacterium::eatFood()
{
    Actor *a=Actor::world()->getOverlappingEdible(this);
    if(a!=nullptr)
    {
        m_foodEaten++;
        a->Actor::setDead();
    }
    return;
}
void Bacterium::newBactCoord(double &x, double &y)
{
    double newX=GraphObject::getX();
    if(newX<VIEW_WIDTH/2)
        newX+=SPRITE_WIDTH/2;
    else if(newX>VIEW_WIDTH/2)
        newX-=SPRITE_WIDTH/2;
    
    double newY=GraphObject::getY();
    if(newY<VIEW_HEIGHT/2)
        newY+=SPRITE_WIDTH/2;
    else if(newY>VIEW_HEIGHT/2)
        newY-=SPRITE_WIDTH/2;
    m_foodEaten=0;
    x=newX;
    y=newY;
}

int Bacterium::foodEaten() const
{
    return m_foodEaten;
}



void EColi::doSomething()
{
    if(Actor::isDead())
        return;
    Socrates *s=Actor::world()->getOverlappingSocrates(this);
    if(s!=nullptr)
        s->takeDamage(4);

    if(foodEaten()>=3)
    {
        double x,y;
        newBactCoord(x, y);
        EColi *e=new EColi(x,y,Actor::world());
        Actor::world()->addActor(e);
    }
    else
    {
        eatFood();
    }
    int angle;
    if(Actor::world()->getAngleToNearbySocrates(this, 256, angle))
    {
        for(int i=0;i<10;i++)
        {
            double newX = (getX() + 2 * cos((i*10+angle*1.0)/180 * PI));
            double newY = (getY() + 2* sin((i*10*+angle*1.0)/180 * PI));
            //std::cout<<i*10+angle*1.0<<std::endl;
            if(!Actor::world()->isBacteriumMovementBlockedAt(this, newX, newY))
            {
                GraphObject::moveAngle((i*10+angle*1.0),2);
                return;
            }
        }
    }
    return;
}
int EColi::soundWhenHurt() const
{
    return SOUND_ECOLI_HURT;
}

int EColi::soundWhenDie() const
{
    return SOUND_ECOLI_DIE;
}



int Salmonella::soundWhenHurt() const
{
    return SOUND_SALMONELLA_HURT;
}
int Salmonella::soundWhenDie() const
{
    return SOUND_SALMONELLA_DIE;
}




int Salmonella::planDistance() const
{
    return m_planDistance;
}
void Salmonella::doSomething()
{
    if(Actor::isDead())
        return;
    
    Socrates *s=Actor::world()->getOverlappingSocrates(this);
    if(s!=nullptr)
        s->takeDamage(1);
    else if(foodEaten()>=3)
    {
        double x,y;
        newBactCoord(x, y);
        RegularSalmonella *s=new RegularSalmonella(x,y,Actor::world());
        Actor::world()->addActor(s);
    }
    else
    {
        eatFood();
    }
    if(planDistance()>0)
    {
        std::cout<<m_planDistance<<std::endl;
        m_planDistance--;
        int angle=getDirection();
        double newX = (getX() + 3 * cos((angle)/180 * PI));
        double newY = (getY() + 3* sin((angle)/180 * PI));

        if(!Actor::world()->isBacteriumMovementBlockedAt(this, newX, newY)&& world()->getDistance(VIEW_WIDTH/2, VIEW_HEIGHT/2, newX, newY)<=VIEW_RADIUS)
        {
            std::cout<<"test0"<<std::endl;
            GraphObject::moveAngle((angle),3);
        }
        else
        {
            GraphObject::setDirection(randInt(0, 359));
            m_planDistance=10;
            std::cout<<"test2"<<std::endl;
        }
    }
    else
    {
        int angle;
        if(!world()->getAngleToNearestNearbyEdible(this, 128, angle))
        {
            std::cout<<"test3"<<std::endl;
            GraphObject::setDirection(randInt(0, 359));
            m_planDistance=10;
            return;
        }
        else
        {
            std::cout<<"test4"<<std::endl;
            world()->getAngleToNearestNearbyEdible(this, 128, angle);
            double newX = (getX() + 3 * cos((angle)/180 * PI));
            double newY = (getY() + 3* sin((angle)/180 * PI));
            if(!Actor::world()->isBacteriumMovementBlockedAt(this, newX, newY)&&world()->getDistance(VIEW_WIDTH/2, VIEW_HEIGHT/2, newX, newY)<VIEW_RADIUS)
            {
                setDirection(angle);
                m_planDistance=10;

            }
            else
                std::cout<<"test5"<<std::endl;
                GraphObject::setDirection(randInt(0, 359));
                m_planDistance=10;
        }
    }

    return;
}

void Salmonella::moveIfPossible(int angle)
{
    
}
void Salmonella::resetDistance()
{
    m_planDistance=10;
}

void AggressiveSalmonella::doSomething()
{
    return;
}


