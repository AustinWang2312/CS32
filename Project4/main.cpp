#include "provided.h"
#include "ExpandableHashMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
using namespace std;
int randInt(int min, int max)
{
    if (max < min)
        std::swap(max, min);
    static std::random_device rd;
    static std::default_random_engine generator(rd());
    std::uniform_int_distribution<> distro(min, max);
    return distro(generator);
}
bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
bool parseDelivery(string line, string& lat, string& lon, string& item);

int main(int argc, char *argv[])
{
    ExpandableHashMap<GeoCoord, string> a;
    GeoCoord an("123","321");
    GeoCoord b("1","2");
    GeoCoord c("2","1");
    GeoCoord d("3","4");
    GeoCoord e("0","4");
    GeoCoord f("1","4");
    a.associate(an, "asdf");
    a.associate(b, "basdfa");
    a.associate(c, "casdfa");
    a.associate(d, "dasdfa");
    a.associate(e, "asdfad");
    a.associate(f, "asdfad");

    //a.reset();
    //cout <<a.find(c);
    //cout<<a.size();
    StreetMap smTest;
    smTest.load("/Users/austinwang/Desktop/CS32/Project4/Project4/mapdata.txt");
    GeoCoord test("34.0724746", "-118.4923463");
    GeoCoord test2("34.0731003", "-118.4931016");
    GeoCoord test3("34.0732851", "-118.4931016");
    GeoCoord test4("34.0736122", "-118.4927669");
    
    PointToPointRouter ptp(&smTest);
    std::list<StreetSegment> route;
    double dist=0;
    

    GeoCoord d1("34.0625329", "-118.4470263");
    GeoCoord d2("34.0636671", "-118.4461842");
    
    
    ptp.generatePointToPointRoute(d1, d2, route, dist);
    std::list<StreetSegment>::iterator lit;
    for (lit=route.begin() ;lit!=route.end();lit++)
    {
        //std::cout<<(*lit).start.latitudeText<<(*lit).start.longitudeText<<std::endl;
        //std::cout<<(*lit).end.latitudeText<<(*lit).end.longitudeText<<std::endl;
        //std::cout<<dist<<std::endl;
    }
    
    
    double randChance=(1.0*randInt(0, RAND_MAX)/RAND_MAX);
    using namespace std;
    //cout<<randChance<<endl;
    int size=4;
    //cout<<exp(2.234)<<endl;
    //cout<<randInt(0, RAND_MAX)%size<<endl;
    vector<StreetSegment> v;
    smTest.getSegmentsThatStartWith(test3, v);
    std::vector<StreetSegment>::const_iterator it;
    //for (it=v.begin() ;it!=v.end();it++)
    //    std::cout<<(*it).end.latitudeText<<(*it).end.longitudeText<<endl;
    
    
    
    
    
    
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
        return 1;
    }

    StreetMap sm;
        
    if (!sm.load(argv[1]))
    {
        cout << "Unable to load map data file " << argv[1] << endl;
        return 1;
    }

    GeoCoord depot;
    vector<DeliveryRequest> deliveries;
    if (!loadDeliveryRequests(argv[2], depot, deliveries))
    {
        cout << "Unable to load delivery request file " << argv[2] << endl;
        return 1;
    }

    cout << "Generating route...\n\n";

    DeliveryPlanner dp(&sm);
    vector<DeliveryCommand> dcs;
    double totalMiles;
    DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
    if (result == BAD_COORD)
    {
        cout << "One or more depot or delivery coordinates are invalid." << endl;
        return 1;
    }
    if (result == NO_ROUTE)
    {
        cout << "No route can be found to deliver all items." << endl;
        return 1;
    }
    cout << "Starting at the depot...\n";
    for (const auto& dc : dcs)
        cout << dc.description() << endl;
    cout << "You are back at the depot and your deliveries are done!\n";
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << totalMiles << " miles travelled for all deliveries." << endl;
}

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
{
    ifstream inf(deliveriesFile);
    if (!inf)
        return false;
    string lat;
    string lon;
    inf >> lat >> lon;
    inf.ignore(10000, '\n');
    depot = GeoCoord(lat, lon);
    string line;
    while (getline(inf, line))
    {
        string item;
        if (parseDelivery(line, lat, lon, item))
            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
    }
    return true;
}

bool parseDelivery(string line, string& lat, string& lon, string& item)
{
    const size_t colon = line.find(':');
    if (colon == string::npos)
    {
        cout << "Missing colon in deliveries file line: " << line << endl;
        return false;
    }
    istringstream iss(line.substr(0, colon));
    if (!(iss >> lat >> lon))
    {
        cout << "Bad format in deliveries file line: " << line << endl;
        return false;
    }
    item = line.substr(colon + 1);
    if (item.empty())
    {
        cout << "Missing item in deliveries file line: " << line << endl;
        return false;
    }
    return true;
}

