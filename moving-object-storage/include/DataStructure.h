#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#define Trajectory_t std::vector<std::tuple<osmium::object_id_type, long>>
#define EdgeVehicleList map<osmium::object_id_type, EdgeVehicleReference>
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
#include "EdgeVehicleReference.h"


using namespace std;

class DataStructure {


public:
    DataStructure();

    //<editor-fold desc="Edge Vehicle List stuff">
    EdgeVehicleList EVList;

    EdgeVehicleList EVListBuilder(vector<osmium::object_id_type> allWays, vector<tuple<double, double>> costAndLength);

    void Insert(Vehicle v);
    void UpdateVehicleTime(Vehicle v, long deltaTime);
    void UpdateVehicleTrajectory(Vehicle v, Trajectory_t newTrajectory);
    void DeleteVehicleFromEVList(Vehicle v);
    void DeleteVehicleFromEdge(Vehicle v, osmium::object_id_type edgeId);
    long GetNumCarsTotal(osmium::object_id_type edgeId);
    long GetNumCarsInSeconds(osmium::object_id_type edgeId, long time);
    //</editor-fold>

    vector<osmium::object_id_type> CalculatePath(osmium::object_id_type startNode, osmium::object_id_type endNode, NodeMapGraph graph);

    //double CostCalc(double length) //TODO implement

    virtual ~DataStructure();

    static void Test();

    //Trajectory
    Trajectory_t testTra();

    vector<osmium::object_id_type> FindAllEdges(Trajectory_t traj);

    //Vehicle
    tuple<long, Trajectory_t> testVec();

    Vehicle MakeVechicle();

    //Accessors


protected:

private:

    bool EdgeInEVList(osmium::object_id_type edgeId);

    //map<osmium::object_id_type, long> ConstructGScore(osmium::object_id_type startNode, NodeMapGraph graph);

    osmium::object_id_type FindMinDist(map<osmium::object_id_type, long> *dist, vector<osmium::object_id_type> *Q);
    bool InList(osmium::object_id_type *element, vector<osmium::object_id_type> *list);


    //Vehicle
    //void Insert(Vehicle vehicle, Trajectory_t trajectory);

//    void Delete(Vehicle vehicle);

//    void TrajectoryUpdate(Vehicle vehicle);

    //void GetGridCell(std::tuple<uint64_t, std::tuple<uint64_t, uint64_t>> currentVehicle);
    //void GetNestedGridCell();

//    void GetCars(); //på en traj?
};

#endif // DATASTRUCTURE_H
