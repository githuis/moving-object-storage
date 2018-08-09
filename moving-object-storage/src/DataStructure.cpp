#include <iostream>
#include <tuple>
#include <functional>
#include <algorithm>
#include "../include/DataStructure.h"
#include <math.h>
#include <osmium/geom/haversine.hpp>
#include <stdexcept>
#include <queue>

DataStructure::DataStructure()
{
    EVList = EdgeVehicleList();
    //ctor
}

DataStructure::~DataStructure()
{
    //dtor
}

Trajectory_t DataStructure::testTra()
{
    auto x = std::vector<std::tuple<osmium::object_id_type, long>>();
    x.push_back(std::make_tuple(30249577, 0));
    x.push_back(std::make_tuple(7867288, 2));
    x.push_back(std::make_tuple(30249577, 4));

    return x;
}

EdgeVehicleList DataStructure::EVListBuilder(vector<osmium::object_id_type> allWays,
                                             vector<tuple<double, double>> costAndLength)
{
    auto x = unordered_map<osmium::object_id_type, EdgeVehicleReference>();

    for (int i = 0; i < allWays.size(); ++i) {
        vector<Vehicle *> y = vector<Vehicle *>();

        auto EVR = EdgeVehicleReference(allWays[i]);
        EVR.vehicles = y;
        EVR.idealCost = get<0>(costAndLength[i]);
        EVR.length = get<1>(costAndLength[i]);

        x[allWays[i]] = EVR;
    }

    return x;
}

void DataStructure::Insert(Vehicle v)
{
    if (v.trajectory.empty() || v.trajectory.size() == 0) {
        cout << "Vehicle has no (an empty) trajectory!!!!" << endl;
        return;
    }
    auto it = unordered_map<osmium::object_id_type, EdgeVehicleReference>::iterator();

    auto list = this->FindAllEdges(v.trajectory);

    for (int i = 0; i < list.size(); ++i) {
        it = EVList.find(list[i]);
        if (it != EVList.end()) {
            EVList[list[i]].vehicles.push_back(&v);
        } else {
            cout << "Edge not found!" << endl;
        }
    }
}

vector<osmium::object_id_type> DataStructure::FindAllEdges(Trajectory_t traj)
{
    auto list = vector<osmium::object_id_type>();

    for (int i = 0; i < traj.size(); ++i) {
        list.push_back((long &&) get<0>(traj[i]));
    }

    return list;
}

void DataStructure::UpdateVehicleTrajectory(Vehicle v, Trajectory_t newTrajectory)
{
    DeleteVehicleFromEVList(v);
    v.UpdateTrajectory(newTrajectory);
    Insert(v);
}

void DataStructure::DeleteVehicleFromEdge(Vehicle v, osmium::object_id_type edgeId)
{
    vector<Vehicle *>::iterator vit;

    if (EdgeInEVList(edgeId))
        remove(EVList[edgeId].vehicles.begin(), EVList[edgeId].vehicles.end(), &v);
}

void DataStructure::DeleteVehicleFromEVList(Vehicle v)
{
    for (auto i : v.trajectory) {
        DeleteVehicleFromEdge(v, get<0>(i));
    }
}

void DataStructure::UpdateVehicleTime(Vehicle v, long deltaTime)
{
    v.UpdateTime(deltaTime);
}

long DataStructure::GetNumCarsTotal(osmium::object_id_type edgeId)
{
    if (EdgeInEVList(edgeId)) {
        return EVList[edgeId].vehicles.size();
    }

}

bool DataStructure::EdgeInEVList(osmium::object_id_type edgeId)
{
    auto it = EVList.find(edgeId);

    return (it != EVList.end());
}

long DataStructure::GetNumCarsInSeconds(osmium::object_id_type edgeId, long time)
{
    if (edgeId = 143978698)return 0;
    int cars = 0;
    if (EdgeInEVList(edgeId)) {
        auto ev = EVList[edgeId];
        for (auto v : ev.vehicles) {
            auto timeSpan = ev.idealCost/2;
            auto something = &v;
            auto arrival = *v->trajectoryMap[edgeId];

            if ((time - timeSpan) <= arrival && (time + timeSpan) >= arrival)
                cars++;


        }
    }
    return cars;
}

double DataStructure::CostCalc(osmium::object_id_type edge, long startDelay)
{

    long carsOnEdge = GetNumCarsInSeconds(edge, startDelay);
    if (carsOnEdge > 0){
        cout << "poggers" << endl;
    }
    auto density = carsOnEdge / EVList[edge].length;

    if(EdgeInEVList(edge))
    {
        auto res = SpeedyCongestedCondition + ((SpeedLimit - SpeedyCongestedCondition) /  pow( (1 + exp(  (density - turningPoint) / scaleOne  )), scaleTwo ));

        return res;
    }


    return -1;
}

/*
 *
 * =================================
 *
 *  New pathfinding
 *
 * =================================
 */
typedef pair<osmium::object_id_type, osmium::object_id_type> iPair;

Trajectory_t
DataStructure::Dijkstra(osmium::object_id_type startNode, osmium::object_id_type endNode, NodeMapGraph graph)
{
    priority_queue<iPair, vector<iPair>, greater<iPair> > Q;
    unordered_map<osmium::object_id_type, long> distance;
    unordered_map<osmium::object_id_type, osmium::object_id_type> previous;
    unordered_map<osmium::object_id_type, long> edges;

    for (auto i = graph.begin(); i != graph.end() ; ++i)
    {
        distance[i->first] = std::numeric_limits<long>::max();
        previous[i->first] = -1;
        edges[i->first] =-1;


    }
    Q.push({0,startNode});
    distance[startNode] = 0;

    while(!Q.empty()) {
        long u = Q.top().second;
        Q.pop();
        int count = 0;
        auto c = graph[u].head;
        auto something = graph[u].length;
        while (count < graph[u].length) {
            osmium::object_id_type v = c->nodeId;
            auto w = CostCalc(c->edge, distance[u]);

            if (distance[v] > distance[u] + w) {
                distance[v] = static_cast<long>(distance[u] + w);
                previous[v] = u;
                edges[v] = c->edge;
                Q.push({distance[v], v});
            }
            count++;
            //if(count == graph[u].length)
            //   continue;
            c = c->next;

        }
        if (u == endNode) {
            return ReturnPath(previous, endNode, distance,edges);
        }
    }
        auto s1 = distance[endNode];
        auto s3 = previous[endNode];
        return ReturnPath(previous, endNode ,distance,edges);
        //cout << distance.size() << endl;
        //cout << Q.size() << " " << Q.empty() << endl;



}

Trajectory_t
DataStructure::ReturnPath(unordered_map<osmium::object_id_type, osmium::object_id_type> prev, osmium::object_id_type target
        ,unordered_map<osmium::object_id_type, long> distance, unordered_map<osmium::object_id_type, long> edges)
{
    auto S = Trajectory_t();

    while(prev[target] != NULL && edges[target] != -1)
    {
        auto s1 = edges[target];
        S.emplace_back(make_tuple(s1,distance[target]));
        target = prev[target];

    }

    return S;
}








