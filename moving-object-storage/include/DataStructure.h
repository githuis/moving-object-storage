#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#define Trajectory_t std::vector<std::tuple<osmium::object_id_type, long>>
#define EdgeVehicleList map<osmium::object_id_type, EdgeVehicleRefrence>
#define GetEdge(X) std::get<0>(X)
#define GetTime(X) std::get<1>(X)
//#define Vehicle std::tuple<long, Trajectory_t>


#include <string>
#include <list>
#include <tuple>
#include <cmath>
#include <map>
#include <osmium/osm/types.hpp>
#include <vector>
#include "NeighbourList.h"
#include "Osm.h"
#include "Vehicle.h"
#include "EdgeVehicleRefrence.h"


using namespace std;

class DataStructure {


public:
    DataStructure();

    //<editor-fold desc="Edge Vehicle List stuff">
    EdgeVehicleList EVList;


    EdgeVehicleList EVListBuilder(vector<osmium::object_id_type> allWays);

    void AddVehicleToEVList(Vehicle v);
    //</editor-fold>

    virtual ~DataStructure();

    static void Test();

    //Trajectory
    Trajectory_t testTra();
    vector<tuple<osmium::object_id_type, long>> FindAllEdgesWithArrivalTime(Trajectory_t traj);
    //vector<tuple<osmium::object_id_type, long>> FindAllEdgesWithArrivalTime(vector<tuple<osmium::object_id_type, long>> traj);

    //Vehicle
    tuple<long, Trajectory_t> testVec();

    Vehicle MakeVechicle();

    //Accessors


protected:

private:
    //Vehicle
    void Insert(Vehicle vehicle, Trajectory_t trajectory);

    void Delete(Vehicle vehicle);

    void TrajectoryUpdate(Vehicle vehicle);

    //void GetGridCell(std::tuple<uint64_t, std::tuple<uint64_t, uint64_t>> currentVehicle);
    //void GetNestedGridCell();

    void GetCars(); //på en traj?
};

#endif // DATASTRUCTURE_H
