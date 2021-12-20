#include "provided.h"
#include <vector>
#include <string>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap *m_sm;
    const PointToPointRouter *m_ptpr;
    const DeliveryOptimizer *m_do;
    
    string getDir(double dir) const;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)

{
    m_sm=sm;
    m_ptpr=new PointToPointRouter(sm);
    m_do=new DeliveryOptimizer(sm);
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
    delete m_ptpr;
    delete m_do;
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    //optimize const vector<DeliveryRequest>& deliveries
    double oldCrowDistance;
    double newCrowDistance;
    
    vector<DeliveryRequest> optDeliveries=deliveries;
    
    m_do->optimizeDeliveryOrder(depot, optDeliveries, oldCrowDistance, newCrowDistance);
    DeliveryRequest returnRoute("", depot);
    optDeliveries.push_back(returnRoute);

    std::list<StreetSegment> deliRoute;
    std::vector<DeliveryRequest>::const_iterator it;
    GeoCoord start=depot;
    for(it=optDeliveries.begin();it!=optDeliveries.end();++it)
    {
        DeliveryResult result= m_ptpr->generatePointToPointRoute(start, (*it).location, deliRoute,totalDistanceTravelled);
        if(result!=DELIVERY_SUCCESS)
            return result;
        start=(*it).location;
    
        
        
        std::list<StreetSegment>::iterator lit;
        for(lit=deliRoute.begin();lit!=deliRoute.end();)
        {
            string currentStreet=(*lit).name;
            
            StreetSegment prev;
            double dir=angleOfLine(*lit);
            string direction=getDir(dir);
            double segmentdist=0;
            while((*lit).name==currentStreet)
            {
                segmentdist += distanceEarthMiles((*lit).start, (*lit).end);
                prev=*lit;
                ++lit;
            }
            DeliveryCommand proceed;
            proceed.initAsProceedCommand(direction, currentStreet, segmentdist);
            commands.push_back(proceed);
            
            if(prev.end==deliRoute.back().end)
            {
                DeliveryCommand mkdelivery;
                if((*it).item=="")
                {
                    break;
                }
                    
                mkdelivery.initAsDeliverCommand((*it).item);
                commands.push_back(mkdelivery);
                
                
                break;
            }
            else
            {
                double turnAngle=angleBetween2Lines(prev, *lit);
                string nextStreet=(*lit).name;
                DeliveryCommand turn;
                string turnDirection;
                if(turnAngle<1 || turnAngle>359)
                {
                    continue;
                }
                if(turnAngle>=1 && turnAngle<180)
                    turnDirection="left";
                if(turnAngle<=359 && turnAngle>=180)
                    turnDirection="right";
                turn.initAsTurnCommand(turnDirection, nextStreet);
                commands.push_back(turn);
            }
        }
    }
    return DELIVERY_SUCCESS;  // Delete this line and implement this function correctly
}

string DeliveryPlannerImpl::getDir(double dir) const
{
    string direction;
    if(0<=dir && dir<22.5)
        direction="east";
    else if(dir<67.5)
        direction="northeast";
    else if(dir<112.5)
        direction="north";
    else if(dir<157.5)
        direction="northwest";
    else if(dir<202.5)
        direction="west";
    else if(dir<247.5)
        direction="southwest";
    else if(dir<292.5)
        direction="south";
    else if(dir<337.5)
        direction="southeast";
    else if(dir<360)
        direction="east";
    
    return direction;
}



//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
