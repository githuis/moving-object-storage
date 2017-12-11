#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <string>
#include <list>
#include <tuple>
#include <cmath>
#include <map>
#include <osmium/osm/types.hpp>
#include "NeighbourList.h"


#define Trajectory_t std::list<osmium::object_id_type>
#define List_Trajectory_t std::list<Trajectory_t>
#define Vehicle std::tuple<uint64_t,Trajectory_t>
#define Nodemap_t std::map<osmium::object_id_type, GraphNode*>

class DataStructure {


public:
    DataStructure();

    Nodemap_t NodeMap;

    explicit DataStructure(Trajectory_t currentTrajectory);

    explicit DataStructure(List_Trajectory_t plannedTrajectory);

    DataStructure(Trajectory_t currentTrajectory, List_Trajectory_t plannedTrajectory);

    virtual ~DataStructure();

    static void Test();

    //Trajectory
    std::tuple<int, int> testTra();

    void MakeTest();

    Trajectory_t current;
    List_Trajectory_t planned;


    //Vehicle
    std::tuple<int, Trajectory_t > testVec();

    void MakeVechicle();

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
