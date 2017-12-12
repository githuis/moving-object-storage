#include <iostream>
#include "include/DataStructure.h"
#include "include/Osm.h"
#include <string>
#include <tuple>

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


    NodeMapGraph graph = osm->GraphBuilder(osm->NodeWayBuilder());



    cout << "End of running" << endl;




    return 0;
}
