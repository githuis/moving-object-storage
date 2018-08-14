#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#define Trajectory_t std::vector<std::tuple<osmium::object_id_type, long>>
#define EdgeVehicleList map<osmium::object_id_type, EdgeVehicleReference>
#define GetEdge(X) std::get<0>(X)
#define GetTime(X) std::get<1>(X)
//#define Vehicle std::tuple<long, Trajectory_t>
#define KmhToMps / 3.6
#define DenseKmToM / 1000
#define NodeLocationMapPointer std::map<osmium::object_id_type, osmium::Location>


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
#include <unordered_map>


using namespace std;

class DataStructure {


public:
    DataStructure();

    //<editor-fold desc="Edge Vehicle List stuff">
    EdgeVehicleList EVList;

    EdgeVehicleList EVListBuilder(vector<osmium::object_id_type> allWays, vector<tuple<double, double>> costAndLength);

    NodeLocationMapPointer NodeLocations;

    void Insert(Vehicle v);

    void UpdateVehicleTime(Vehicle v, long deltaTime);

    void UpdateVehicleTrajectory(Vehicle v, Trajectory_t newTrajectory);

    void DeleteVehicleFromEVList(Vehicle v);

    void DeleteVehicleFromEdge(Vehicle v, osmium::object_id_type edgeId);

    long GetNumCarsTotal(osmium::object_id_type edgeId);

    long GetNumCarsInSeconds(osmium::object_id_type edgeId, long time);
    //</editor-fold>

    //vector<osmium::object_id_type>
    //CalculatePath(osmium::object_id_type startNode, osmium::object_id_type endNode, NodeMapGraph graph);

    //vector<osmium::object_id_type>
    //CalculatePathAStar(osmium::object_id_type startNode, osmium::object_id_type endNode, NodeMapGraph graph);

    //double HeuristicCost(osmium::object_id_type nodeStart, osmium::object_id_type nodeEnd);

    Trajectory_t
    Dijkstra(osmium::object_id_type startNode, osmium::object_id_type endNode,
             map<osmium::object_id_type, NeighbourList> graph);


    double CostCalc(osmium::object_id_type edge, long startDelay);


    virtual ~DataStructure();

    Trajectory_t testTra();

    vector<osmium::object_id_type> FindAllEdges(Trajectory_t traj);

protected:

private:

    bool EdgeInEVList(osmium::object_id_type edgeId);

    //map<osmium::object_id_type, long> ConstructGScore(osmium::object_id_type startNode, NodeMapGraph graph);

    static constexpr double SpeedLimit = 80 KmhToMps; //Speed limit is 80 kmh in M/s
    static constexpr double SpeedyCongestedCondition =
            15 KmhToMps; //At 15 kmh we assume traffic is stop and go. Also measured in M/s
    static constexpr double turningPoint = 20 DenseKmToM; //How many cars per m (km / 1000) to be congested
    static constexpr double scaleOne =
            0.1612 * (20 DenseKmToM) + 0.0337; //A scaling parameter, usually found from historical data, stolen
    static constexpr double scaleTwo =
            0.0093 * (20 DenseKmToM) + 0.0507; //A scaling parameter, usually found from historical data, stolen

    Trajectory_t
    ReturnPath(map<osmium::object_id_type, osmium::object_id_type> prev, osmium::object_id_type target,map<osmium::object_id_type,
            long> distance,map<osmium::object_id_type, osmium::object_id_type> edges);

};

#endif // DATASTRUCTURE_H
