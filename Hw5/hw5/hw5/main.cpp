//
//  main.cpp
//  hw5
//
//  Created by Austin Wang on 3/12/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Class
{
  public:
    Class(string nm) : m_name(nm) {}
    string name() const { return m_name; }
    const vector<Class*>& subclasses() const { return m_subclasses; }
    void add(Class* d) { m_subclasses.push_back(d); }
    ~Class();
  private:
    string m_name;
    vector<Class*> m_subclasses;
};

Class::~Class()
{
    for (size_t k = 0; k < m_subclasses.size(); k++)
        delete m_subclasses[k];
}

void listAll(std::string path, const Class* c)  // two-parameter overload
{
    std::string lineOutput=path+c->name();
    if (path!="")
        lineOutput =path+"=>"+c->name();
    std::cout << lineOutput<<std::endl;
    for(std::vector<Class*>::const_iterator it =c->subclasses().begin();it!=c->subclasses().end();it++)
        listAll(lineOutput, *it);
}

void listAll(const Class* c)  // one-parameter overload
{
    if (c != nullptr)
        listAll("", c);
}

int main()
{
    Class* d1 = new Class("Salmonella");
    listAll(d1);
    cout << "====" << endl;
    d1->add(new Class("AggressiveSalmonella"));
    Class* d2 = new Class("Bacterium");
    d2->add(new Class("EColi"));
    d2->add(d1);
    Class* d3 = new Class("Goodie");
    d3->add(new Class("RestoreHealthGoodie"));
    d3->add(new Class("FlamethrowerGoodie"));
    d3->add(new Class("ExtraLifeGoodie"));
    listAll(d3);
    cout << "====" << endl;
    Class* d4 = new Class("Actor");
    d4->add(d2);
    d4->add(new Class("Flame"));
    d4->add(d3);
    listAll(d4);
    delete d4;
}
