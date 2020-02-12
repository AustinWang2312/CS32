//
//  Map.cpp
//  Project2
//
//  Created by Austin Wang on 1/26/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#include <stdio.h>
#include "Map.h"
void Map::dump() const
{
    m_list.printItems();
}

bool Map::empty() const
{
    return m_list.size()==0;
}

int Map::size() const
{
    return m_list.size();
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    return m_list.addToRear(key, value);
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    return m_list.updateItem(key, value);
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (update(key, value))
        return true;
    else
        return insert(key, value);
}

bool Map::erase(const KeyType& key)
{
    return m_list.deleteItem(key);
}

bool Map::contains(const KeyType& key) const
{
    return m_list.isKey(key);
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    return m_list.findItem(key, value);
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    return m_list.findItem(i, key, value);
}

void Map::swap(Map& other)
{
    Map temp(other);
    other=*this;
    *this=temp;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    result=m1;
    bool can_Combine=true;
    int nCount=0;
    while(nCount<m2.size())
    {
        KeyType k2;
        ValueType v2;
        m2.get(nCount, k2, v2);
        
        ValueType v1;
        if(m1.get(k2, v1)&&v1!=v2)
        {
            result.erase(k2);
            can_Combine=false;
        }
        else if(!m1.contains(k2))
            result.insert(k2, v2);
        nCount++;
    }
    return can_Combine;
}

void reassign(const Map& m, Map& result)
{
    result=m;
    if (result.empty())
    {
        return;
    }
    if (result.size()==1)
    {
        return;
    }
    KeyType initial_k;
    ValueType initial_v;
    result.get(0, initial_k, initial_v);
    for (int i=1;i<result.size();i++)
    {
        KeyType k1;
        ValueType v1;
        
        KeyType k2;
        ValueType v2;
        
        result.get(i-1, k1, v1);
        result.get(i,k2,v2);
        
        result.update(k1,v2);
    }
    KeyType final_k;
    ValueType final_v;
    result.get(result.size()-1,final_k,final_v);
    result.update(final_k,initial_v);
}
