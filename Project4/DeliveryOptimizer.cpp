#include "provided.h"
#include <vector>
#include <cstdlib>
#include <cmath>
#include <random>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    double calcTotalDistance(const GeoCoord& depot,vector<DeliveryRequest>& deliveries) const;
    int randInt(int min, int max) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    oldCrowDistance = 0;  // Delete these lines and implement this function correctly
    newCrowDistance = 0;
    
    oldCrowDistance = calcTotalDistance(depot, deliveries);
    
    //simulated annealing
    
    
    
    double temperature=20;
    double tempLimit=.0001;
    double tempChange=0.90;
    double numIter=100;
    
    vector<DeliveryRequest> best=deliveries;
    double bestDist=calcTotalDistance(depot, best);
    vector<DeliveryRequest> cur=deliveries;
    
    while(temperature>tempLimit)
    {
        int count=0;
        while (count<numIter)
        {
            double curDist=calcTotalDistance(depot, cur);
            if(curDist<bestDist)
            {
                bestDist=curDist;
                best=cur;
            }
            
            vector<DeliveryRequest> tmp=cur;
            int size=tmp.size();
            int index1=randInt(0, RAND_MAX)%size;
            int index2=randInt(0, RAND_MAX)%(size-1);
            if(index2>=index1)
                ++index2;
            std::swap(tmp[index1], tmp[index2]);

            
            //manipulate
            double newDist=calcTotalDistance(depot, tmp);
            
            double prob=exp((1.0*curDist-newDist)/temperature);
            double randChance=(1.0*randInt(0, RAND_MAX)/RAND_MAX);
            if(prob>randChance)
            {
                cur=tmp;
            }
            ++count;
        }
        temperature*=tempChange;
    }
    
    deliveries=best;
    newCrowDistance=calcTotalDistance(depot,best);
}
double DeliveryOptimizerImpl::calcTotalDistance(const GeoCoord& depot, vector<DeliveryRequest>& deliveries) const
{
    double distance=0;
    GeoCoord segStart=depot;
    for(int i=0;i<deliveries.size();++i)
    {
        GeoCoord segEnd=deliveries[i].location;
        distance+= distanceEarthMiles(segStart, segEnd);
        segStart=segEnd;
    }

    return distance;
}

int DeliveryOptimizerImpl::randInt(int min, int max) const
{
    if (max < min)
        std::swap(max, min);
    static std::random_device rd;
    static std::default_random_engine generator(rd());
    std::uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
