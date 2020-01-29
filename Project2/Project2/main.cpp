//
//  main.cpp
//  Project2
//
//  Created by Austin Wang on 1/26/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#include <iostream>
#include "Map.h"
#include <cassert>
bool combine(const Map& m1, const Map& m2, Map& result);
void makeMap()
{
    Map *t=new Map();
    delete t;
}
int main() {
    // insert code here...
    makeMap();
    Map *i = new Map();
    //i->dump();
//    std::cerr << i->empty()<<std::endl;
    assert(i->insert("test", 1));
    assert(i->insert("test2", 1));
    assert(i->insert("test2", 1)==false);
    assert(i->update("test2", 2));
    assert(i->update("testnot", 2)==false);
    assert(i->insertOrUpdate("test2", 3));
    assert(i->contains("test")==true && i->contains("testnot")==false);
    assert(i->erase("test"));
    //i->dump();
    //std::cerr<<"done"<<std::endl;
    assert(i->insertOrUpdate("test3", 0));
    assert(i->insertOrUpdate("test4", 0));
    //i->dump();
    assert(i->erase("test3"));
    
    //i->dump();
    assert(i->erase("test4"));
    assert(i->erase("test2"));
    assert(i->empty());
    assert(i->insert("newtest1", 1));
    assert(i->insert("newtest1", 2)==false);
    assert(i->insert("newtest2", 1));
    assert(i->size()==2);
    
    ValueType v;
    assert(i->get("newtest1", v)&&v==1);
    assert(i->get("newtest2", v)&&v==1);
    assert(i->get("newtestnot", v)==false);
    
    KeyType k;
    assert(i->get(0,k,v)&&v==1);
    assert(i->get(1,k,v)&&v==1);
    assert(i->get(2,k,v)==false);
    assert(i->get(-22,k,v)==false);


    //i->dump();
    //std::cerr<<"end of i"<<std::endl;
    Map *j=new Map();
    *j=*i;
    //j->dump();
    j->erase("newtest1");
    //std::cerr<<"end of j"<<std::endl;
    //i->dump();
    Map a;
    a.insert("a1", 0);
    a.insert("a1", 0);
    a.insert("a2", 1);
    Map b(a);
    b.swap(a);
    //b.dump();
    delete i;
    delete j;
    
    
    Map part1;
    Map part2;
    Map combineMap;
    part1.insert("u1", 0);
    part1.insert("u2", 0);
    part1.insert("u3", 0);
    part1.insert("r1", 0);
    
    part2.insert("v1", 0);
    part2.insert("v2", 0);
    part2.insert("v3", 0);
    part2.insert("r1", 0);
    combine(part1, part2, combineMap);
    combine(part1, part2, combineMap);
    combine(part1, part2, combineMap);
    combine(part1, part2, combineMap);
    //combineMap.dump();
    
    part1.insert("v1", 0);
    part1.insert("v2", 1);
    assert(!combine(part1,part2,combineMap));
    
    
    combineMap.dump();
    combineMap=a;
    combineMap.dump();
    //part1.dump();
    //part2.dump();
}
