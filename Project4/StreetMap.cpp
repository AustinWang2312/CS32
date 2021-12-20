#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, std::vector<StreetSegment>> *m_map;
};

StreetMapImpl::StreetMapImpl()
{
    m_map=new ExpandableHashMap<GeoCoord, std::vector<StreetSegment>>;
}

StreetMapImpl::~StreetMapImpl()
{
    delete m_map;
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream inf(mapFile);
    if(!inf)
    {
        return false;
    }
    string line;
    while(getline(inf,line))
    {
        istringstream iss(line);
        int numSeg;
        string street;
        string lat1;
        string lon1;
        string lat2;
        string lon2;
        
        if (! (iss >> street))
            return false;
        street=line;
        if(getline(inf,line))
        {
            istringstream iss(line);
            if(! (iss >> numSeg))
                return false;
            int count=0;
            while(count<numSeg && getline(inf,line))
            {
                istringstream iss(line);
                if(! (iss >> lat1 >> lon1 >> lat2 >> lon2))
                {
                    return false;
                }
                ++count;
                GeoCoord start(lat1, lon1);
                GeoCoord end(lat2, lon2);
                StreetSegment forward(start, end, street);
                StreetSegment rev(end, start, street);
                vector<StreetSegment> *ptr;
                ptr=m_map->find(start);
                if(ptr)
                {
                    ptr->push_back(forward);
                }
                else
                {
                    std::vector<StreetSegment> v;
                    v.push_back(forward);
                    m_map->associate(start, v);
                }

                
                ptr=m_map->find(end);
                if(ptr)
                {
                    ptr->push_back(rev);
                }
                else
                {
                    std::vector<StreetSegment> v;
                    v.push_back(rev);
                    m_map->associate(end, v);
                }
                
            }
        }
        else
            return false;
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    
    std::vector<StreetSegment>const *ptr=m_map->find(gc);
    if(ptr)
    {
        std::vector<StreetSegment>::const_iterator it;
        for (it=ptr->begin() ;it!=ptr->end();it++)
        {
            segs.push_back(*it);
        }
        return true;
    }
    return false;  // Delete this line and implement this function correctly
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
