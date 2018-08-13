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
#define MAC true

#if MAC
#include<mach/mach.h>
#endif


using namespace std;

#if MAC

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


int main(int argc, char *argv[]) {
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

    clock_t tStart;
    vector<Vehicle> testVehicles;
    int MaxOperations = 1000000;
    double buildTime = 0;
    double updateTime = 0;
    double vm, rss;

    ofstream outputFile;
    outputFile.open("programTestData.txt");

    cout << "Buildtime, Cars, update s, vram mb, total mem mb" << endl;
    outputFile << "Buildtime, Cars, update s, vram mb, total mem mb" << endl;

    for (int cars = 10; cars <= MaxOperations; cars *= 10) {

            testVehicles = vector<Vehicle>();
            tStart = clock();

            for (int i = 0; i <= cars; ++i) {
                auto p = ds->Dijkstra(4565674939, 1186095168, graph);
                testVehicles.push_back(Vehicle(i, p));
                ds->Insert(Vehicle(i,p));
            }

            buildTime = (double) (clock() - tStart) / CLOCKS_PER_SEC;
            cout << buildTime << endl;
            outputFile << buildTime;

            tStart = clock();

            //Uncomment this loop for testing the update time operation
            //for (int k = 0; k < cars; ++k) {
            //    testVehicles[k].UpdateTime(5);
            }

            //Uncomment for dijkstras test
            //auto test = ds->Dijkstra(28783203,333245479,graph);

            //Uncomment this loop for testing the update trajectory operation
            //for (int k = 0; k < cars; ++k) {
            //    if (k == 0)
            //        testVehicles[k].UpdateTrajectory(testVehicles[k + 3].trajectory);
            //    else
            //       testVehicles[k].UpdateTrajectory(testVehicles[k - 1].trajectory);
            //}

            //Uncomment this loop for testing the weight calculation operation
            //for (int i = 0; i < cars; ++i) {
            //
            //    auto veh = testVehicles[i];
            //    double totalCost;
            //
            //    for (auto pair : veh.trajectory){
            //        totalCost += ds->CostCalc(get<0>(pair), get<1>(pair));
            //    }
            //}
        updateTime =(double) (clock() - tStart)/ CLOCKS_PER_SEC;

#if MAC
                //from   https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
                struct task_basic_info t_info;

                mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;



                if (KERN_SUCCESS != task_info(mach_task_self(),

                                              TASK_BASIC_INFO, (task_info_t)&t_info,

                                              &t_info_count))

                {
                    return -1;
                }

     
                cout << cars << "," << updateTime << "," << t_info.virtual_size << "," << t_info.resident_size << endl;
                outputFile << "," << cars << "," << updateTime << "," << t_info.virtual_size << "," << t_info.resident_size << endl;
#else
                process_mem_usage(vm, rss);

                cout << "," << cars << "," << fixed
                     << updateTime << "," << (vm / 1024) << "," << (rss / 1024)
                     << endl;
                outputFile << "," << cars << "," << updateTime << "," << (vm / 1024) << "," << (rss / 1024)
                           << endl;
#endif


        }

        cout << endl << endl;

    cout << "End of running" <<
        endl;


    return 0;
}