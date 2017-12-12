#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <string>
#include <list>
#include <tuple>
#include <cmath>
#include <map>
#include <osmium/osm/types.hpp>
#include <vector>
#include "NeighbourList.h"
#include "Osm.h"

#define Trajectory_t std::vector<std::tuple<osmium::object_id_type, long>>
#define EdgeVehicleList vector<tuple<osmium::object_id_type, vector<Vehicle*>>>
#define GetEdge(X) std::get<0>(X)
#define GetTime(X) std::get<1>(X)
#define Vehicle std::tuple<long, Trajectory_t>

using namespace std;

class DataStructure {


public:
    DataStructure();

    EdgeVehicleList EVList;

    EdgeVehicleList EVListBuilder(NodeMapGraph graph);

    void AddEdgeToEVList(osmium::object_id_type edgeid);

    void AddVehicleToEVList(osmium::object_id_type edgeid, Vehicle v);

    virtual ~DataStructure();

    static void Test();

    //Trajectory
    Trajectory_t testTra();

    void MakeTest();


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
