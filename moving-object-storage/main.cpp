#include <iostream>
#include "include/DataStructure.h"
#include "include/Osm.h"
#include <string>
#include <tuple>
#include <iomanip>

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


#if QUICK_RUN
        NodeMapGraph graph = osm->GraphBuilder(osm->NodeWayBuilder("map.osm"), "map.osm");
#else
        NodeMapGraph graph = osm->GraphBuilder(osm->NodeWayBuilder("denmark-latest.osm.pbf"), "denmark-latest.osm.pbf");
#endif

    ds->NodeLocations = osm->NodeLocations;


    ds->EVList = ds->EVListBuilder(osm->AllWays, osm->IdealCost);

    EdgeVehicleList *EVPointer = &ds->EVList;

    //Vehicle testVehicle = Vehicle(1, testTrajectory);

    //Randers node = 703743906
    //auto path = ds->CalculatePath(28783202, 28785114, graph);
   // auto path = ds->CalculatePathAStar(28783202, 1043821932, graph);



    clock_t tStart;
    vector<Vehicle> testVehicles;
    int testMax = 10000;
    int trajMax = 10000;
    double buildTime = 0;

    for (int j = 0; j < 3; ++j) {

        cout << "Test #" << j+1 << endl;
        cout << "Buildtime, Cars, Trajectory, update traj (seconds)" << endl;

        for (int cars = 10; cars <= testMax; cars *= 10) {
            for (int trajectorySize = 10; trajectorySize <= trajMax; trajectorySize *= 10) {
                testVehicles = vector<Vehicle>();
                tStart = clock();

                for (int i = 0; i < cars; ++i) {
                    //testVehicles.push_back(Vehicle(i, ds->ConstructRandomPath(trajectorySize, graph)));
                    testVehicles.emplace_back(i, ds->ConstructRandomPath(trajectorySize, graph));
                }

                buildTime = (double) (clock() - tStart) / CLOCKS_PER_SEC;
                tStart = clock();

                //Update test
                //for (int k = 0; k < cars; ++k)
                //{
                //    testVehicles[k].UpdateTime(5);
                //}

                for (int k = 0; k < cars; ++k) {
                    if (k == 0)
                        testVehicles[k].UpdateTrajectory(testVehicles[k + 3].trajectory);
                    else
                        testVehicles[k].UpdateTrajectory(testVehicles[k - 1].trajectory);
                }

                cout << buildTime << cars << "," << trajectorySize << "," << (double) (clock() - tStart) / CLOCKS_PER_SEC << endl;
            }
        }

        cout << endl << endl;
    }



    //ds->Insert(testVehicle);
    //testVehicle.UpdateTime(4);

    cout << "End of running" << endl;




    return 0;
}
