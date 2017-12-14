#include <iostream>
#include "include/DataStructure.h"
#include "include/Osm.h"
#include <string>
#include <tuple>

#define QUICK_RUN true

using namespace std;

int main(int argc, char* argv[])
{
    cout << "Hello" << endl;
    DataStructure *ds = new DataStructure();


    Trajectory_t testTrajectory = ds->testTra();
    auto res = GetEdge(testTrajectory[0]);

    //Test Osmium
    Osm *osm = new Osm();
    //osm->LocationTest();
    //osm->AddPoiTest(argc, argv);


    if(QUICK_RUN)
        NodeMapGraph graph = osm->GraphBuilder(osm->NodeWayBuilder("map.osm"), "map.osm");
    else
        NodeMapGraph graph = osm->GraphBuilder(osm->NodeWayBuilder("denmark-latest.osm.pbf"), "denmark-latest.osm.pbf");

    ds->EVList = ds->EVListBuilder(osm->AllWays);

    EdgeVehicleList *EVPointer = &ds->EVList;

    Vehicle testVehicle = Vehicle(1, testTrajectory);


    ds->Insert(testVehicle);
    testVehicle.UpdateTime(4);

    cout << "End of running" << endl;




    return 0;
}
