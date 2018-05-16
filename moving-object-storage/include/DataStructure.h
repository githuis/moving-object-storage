#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#define Trajectory_t std::vector<std::tuple<osmium::object_id_type, long>>
#define EdgeVehicleList unordered_map<osmium::object_id_type, EdgeVehicleReference>
#define GetEdge(X) std::get<0>(X)
#define GetTime(X) std::get<1>(X)
//#define Vehicle std::tuple<long, Trajectory_t>
#define KmhToMps / 3.6
#define DenseKmToM / 1000
#define NodeLocationMapPointer std::unordered_map<osmium::object_id_type, osmium::Location>


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

    bool NodeInNodeLocMap(osmium::object_id_type node);

    vector<osmium::object_id_type>
    ReconstructPath(unordered_map<osmium::object_id_type, osmium::object_id_type> cameFrom, osmium::object_id_type current);


    Trajectory_t ConstructRandomPath(int maxLength, NodeMapGraph graph);

    vector<tuple<osmium::object_id_type, long>>
    ConstructRandomPathQuick(int maxLength, unordered_map<osmium::object_id_type, NeighbourList> graph);

    vector<osmium::object_id_type> CalculatePathNew(osmium::object_id_type startNode, osmium::object_id_type endNode,
                                                    unordered_map<osmium::object_id_type, NeighbourList> graph);


    double CostCalc(osmium::object_id_type edge, long startDelay);

    double RandomCostCalc();

    virtual ~DataStructure();

    Trajectory_t testTra();

    vector<osmium::object_id_type> FindAllEdges(Trajectory_t traj);

protected:

private:

    bool EdgeInEVList(osmium::object_id_type edgeId);

    //map<osmium::object_id_type, long> ConstructGScore(osmium::object_id_type startNode, NodeMapGraph graph);

    osmium::object_id_type FindMinDist(unordered_map<osmium::object_id_type, double> *dist, vector<osmium::object_id_type> *Q);

    bool InList(osmium::object_id_type element, vector<osmium::object_id_type> list);


    static constexpr double SpeedLimit = 80 KmhToMps; //Speed limit is 80 kmh in M/s
    static constexpr double SpeedyCongestedCondition =
            15 KmhToMps; //At 15 kmh we assume traffic is stop and go. Also measured in M/s
    static constexpr double turningPoint = 20 DenseKmToM; //How many cars per m (km / 1000) to be congested
    static constexpr double scaleOne =
            0.1612 * (20 DenseKmToM) + 0.0337; //A scaling parameter, usually found from historical data, stolen
    static constexpr double scaleTwo =
            0.0093 * (20 DenseKmToM) + 0.0507; //A scaling parameter, usually found from historical data, stolen


    osmium::object_id_type FindMinDistNew(unordered_map<osmium::object_id_type, long> *dist, vector<osmium::object_id_type> *Q);


    bool InListNew(osmium::object_id_type element, vector<osmium::object_id_type> list);
};

#endif // DATASTRUCTURE_H
