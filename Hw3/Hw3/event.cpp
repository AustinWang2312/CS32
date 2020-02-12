//
//  event.cpp
//  Hw3
//
//  Created by Austin Wang on 2/10/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;


class Event
{
public:
    Event(string name){
        m_name=name;
    }
    string name() const
    {
        return m_name;
    }
    virtual ~Event()
    {
    }
    virtual string need() const=0;
    virtual bool isSport() const
    {
        return true;
    }
    
private:
    string m_name;

};

class Concert: public Event
{
public:
    Concert(string name, string genre)
    :Event(name)
    {
        m_genre=genre;
        
    }
    
    virtual ~Concert()
    {
        cout<<"Destroying the "<<Event::name()<<" "<<m_genre<<" concert\n";
    }
    
    virtual bool isSport() const
    {
        return false;
    }
    
    virtual string need() const
    {
        return "a stage";
    }
    
private:
    string m_genre;
};
class BasketballGame: public Event
{
public:
    BasketballGame(string name)
    :Event(name)
    {
    }
    virtual ~BasketballGame()
    {
        cout<<"Destroying the "<<Event::name()<<" "<<"basketball game\n";
    }
    virtual string need() const
    {
        return "hoops";
    }
    
};

class HockeyGame: public Event
{
    public:
    HockeyGame(string name)
    :Event(name)
    {
    }
    virtual ~HockeyGame()
    {
        cout<<"Destroying the "<<Event::name()<<" "<<"hockey game\n";
    }
    virtual string need() const
    {
        return "ice";
    }
};

void display(const Event* e)
{
    cout << e->name() << ": ";
    if (e->isSport())
    cout << "(sport) ";
    cout << "needs " << e->need() << endl;
}
/*
int main()
{
    Event* events[4];
    //Event *p=new Event("asd");
    events[0] = new BasketballGame("Lakers vs. Suns");
      // Concerts have a name and a genre.
    events[1] = new Concert("Banda MS", "banda");
    events[2] = new Concert("KISS", "hard rock");
    events[3] = new HockeyGame("Kings vs. Flames");

    cout << "Here are the events." << endl;
    for (int k = 0; k < 4; k++)
    display(events[k]);

      // Clean up the events before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
    delete events[k];
}
**/
