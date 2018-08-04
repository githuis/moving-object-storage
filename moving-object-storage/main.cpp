#include <iostream>
#include "include/DataStructure.h"
#include "include/Osm.h"
#include <string>
#include <tuple>
#include <iomanip>

#include <unistd.h>
#include <fstream>
#include <ios>

#include <iostream>


#define QUICK_RUN true
#define MAC false

#if MAC
#include<mach/mach.h>
#endif


using namespace std;

#if MAC
void memvoid()
{

struct task_basic_info t_info;
mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

if (KERN_SUCCESS != task_info(mach_task_self(),
        TASK_BASIC_INFO, (task_info_t)&t_info,
&t_info_count))
{
return;
}

#else


//Code is taken from https://stackoverflow.com/questions/669438/how-to-get-memory-usage-at-run-time-in-c
void process_mem_usage(double &vm_usage, double &resident_set)
{
    using std::ios_base;
    using std::ifstream;
    using std::string;

    vm_usage = 0.0;
    resident_set = 0.0;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat", ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

#endif


int main(int argc, char *argv[])
{
    cout << "Beginning to build road network graph" << endl;
    DataStructure *ds = new DataStructure();

    //Initialize the data loader
    Osm *osm = new Osm();

    //Use either a small or large data set to construct road network graph
#if QUICK_RUN
    NodeMapGraph graph = osm->GraphBuilder(osm->NodeWayBuilder("map.osm"), "map.osm");
#else
    NodeMapGraph graph = osm->GraphBuilder(osm->NodeWayBuilder("denmark-latest.osm.pbf"), "denmark-latest.osm.pbf");
#endif

    //Build the Edges hashmap
    ds->NodeLocations = osm->NodeLocations;
    ds->EVList = ds->EVListBuilder(osm->AllWays, osm->IdealCost);

    //Run a pathing example
    cout << "Pathing: " << endl;
    auto path = ds->Dijkstra(28783203, 333245479, graph); //Creates a list of nodes, not a real trajectory.
    cout << "Done" << endl;

    clock_t tStart;
    vector<Vehicle> testVehicles;
    int testMax = 10000;
    int trajMax = 100000;
    double buildTime = 0;
    double vm, rss;

    ofstream outputFile;
    outputFile.open("programTestData.txt");

    cout << "Cars, Trajectory, update s, vram mb, total mem mb" << endl;
    outputFile << "Cars, Trajectory, update s, vram mb, total mem mb" << endl;

    for (int cars = 10; cars <= testMax; cars *= 10) {
        for (int trajectorySize = 10; trajectorySize <= trajMax; trajectorySize *= 10) {
            testVehicles = vector<Vehicle>();
            tStart = clock();

            for (int i = 0; i <= cars; ++i) {
                auto p = ds->Dijkstra(28783203, 333245479, graph);
                testVehicles.push_back(Vehicle(i, p));
            }

            buildTime = (double) (clock() - tStart) / CLOCKS_PER_SEC;
            cout << buildTime;

            tStart = clock();

            //Uncomment this loop for testing the update time operation
            for (int k = 0; k < cars; ++k) {
                testVehicles[k].UpdateTime(5);
            }

            //Uncomment this loop for testing the update trajectory operation
            //for (int k = 0; k < cars; ++k) {
            //    if (k == 0)
            //        testVehicles[k].UpdateTrajectory(testVehicles[k + 3].trajectory);
            //    else
            //        testVehicles[k].UpdateTrajectory(testVehicles[k - 1].trajectory);
            //}

            //Uncomment this loop for testing the weight calculation operation
            //for (int i = 0; i < cars; ++i) {

            //    auto veh = testVehicles[i];
            //    double totalCost;

            //    for (auto pair : veh.trajectory){
            //        totalCost += ds->CostCalc(get<0>(pair), get<1>(pair));
            //    }

            //}

#if MAC
                cout << cars << "," << trajectorySize << "," << fixed << (double) (clock() - tStart) / CLOCKS_PER_SEC << "," << t_info.virtual_size << endl;
                outputFile << "," << cars << "," << trajectorySize << "," << fixed << (double) (clock() - tStart) / CLOCKS_PER_SEC << "," << t_info.virtual_size << endl;
#else
                process_mem_usage(vm, rss);

                cout << cars << "," << fixed
                     << (double) (clock() - tStart) / CLOCKS_PER_SEC << "," << (vm / 1024) << "," << (rss / 1024)
                     << endl;
                outputFile << cars << "," << fixed
                           << (double) (clock() - tStart) / CLOCKS_PER_SEC << "," << (vm / 1024) << "," << (rss / 1024)
                           << endl;
#endif

            }
        }

        cout << endl << endl;

    cout << "End of running" <<
        endl;


    return 0;
}