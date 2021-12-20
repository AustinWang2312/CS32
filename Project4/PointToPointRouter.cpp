#include "provided.h"
#include <list>
#include <queue>
#include <set>
#include <map>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap *m_sm;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_sm=sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    class Cmp
    {
    public:
        int operator() (const StreetSegment& s1, const StreetSegment& s2)
        {
            return distanceEarthMiles(s1.start, s1.end) > distanceEarthMiles(s2.start, s2.end);
        }
    };
    
    vector<StreetSegment> tmp;
    if(!m_sm->getSegmentsThatStartWith(start, tmp)||!m_sm->getSegmentsThatStartWith(start, tmp))
        return BAD_COORD;
    
    if (start==end)
    {
        return DELIVERY_SUCCESS;
    }
    std::priority_queue<StreetSegment,vector<StreetSegment>,Cmp> pq;
    std::queue<StreetSegment> q;
    std::set<GeoCoord> visited;
    std::map<GeoCoord,StreetSegment> coordMap;
    
    visited.insert(end);
    std::vector<StreetSegment> v;
    m_sm->getSegmentsThatStartWith(end, v);

    
    typename std::vector<StreetSegment>::iterator vit;
    for (vit=v.begin() ;vit!=v.end();vit++)
    {
        pq.push(*vit);
        StreetSegment rev((*vit).end,(*vit).start,(*vit).name);
        coordMap.insert(std::pair<GeoCoord,StreetSegment>((*vit).end, rev));
    }
    for(int i=0;i<pq.size();++i)
    {
        q.push(pq.top());
        pq.pop();
    }
    
    
    while(!q.empty())
    {
        StreetSegment top = q.front();
        q.pop();
        

        std::vector<StreetSegment> v;
        if(m_sm->getSegmentsThatStartWith(top.end, v))
        {
            typename std::vector<StreetSegment>::iterator vit;
            for (vit=v.begin() ;vit!=v.end();vit++)
            {
                if(visited.find((*vit).end)==visited.end())
                {
                    pq.push(*vit);
                    visited.insert((*vit).end);
                    
                    StreetSegment rev((*vit).end,(*vit).start,(*vit).name);
                    coordMap.insert(std::pair<GeoCoord,StreetSegment>((*vit).end, rev));
                    if((*vit).end==start)
                    {
                        //find path
                        GeoCoord pathIter=start;
                        while(pathIter!=end)
                        {
                            std::map<GeoCoord,StreetSegment>::iterator tmp_it=coordMap.find(pathIter);
                            pathIter=coordMap.find(pathIter)->second.end;
                            route.push_back(tmp_it->second);
                            totalDistanceTravelled+=distanceEarthMiles(tmp_it->second.start, tmp_it->second.end);
                        }
                        return DELIVERY_SUCCESS;
                    }
                }
                
            }
        
        
            for(int i=0;i<pq.size();++i)
            {
                q.push(pq.top());
                pq.pop();
            }
        }
        
    }

    return NO_ROUTE;  // Delete this line and implement this function correctly
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
