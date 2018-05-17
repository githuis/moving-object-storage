#include <iostream>
#include <tuple>
#include <functional>
#include <algorithm>
#include "../include/DataStructure.h"
#include <math.h>
#include <osmium/geom/haversine.hpp>
#include <stdexcept>

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
    //auto x = vector<tuple<osmium::object_id_type, vector<tuple<long, vector<tuple<osmium::object_id_type, long>*>> *>>>();
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
    auto it;

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


    //return vector<vector<tuple<osmium::object_id_type, long>>>();
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
    int cars = 0;
    if (EdgeInEVList(edgeId)) {
        auto ev = EVList[edgeId];
        for (auto v : ev.vehicles) {
            auto timeSpan = ev.idealCost/2;
            auto arrival = *v->trajectoryMap[edgeId];

            if( (time-timeSpan) <= arrival && (time+timeSpan) >= arrival)
                cars++;
        }
    }
    return cars;
}

double DataStructure::CostCalc(osmium::object_id_type edge, long startDelay)
{
    long carsOnEdge = GetNumCarsInSeconds(edge, startDelay);
    auto density = carsOnEdge / EVList[edge].length;

    if(EdgeInEVList(edge))
    {
        //return ; //TODO apply func

        auto res = SpeedyCongestedCondition + ((SpeedLimit - SpeedyCongestedCondition) /  pow( (1 + exp(  (density - turningPoint) / scaleOne  )), scaleTwo ));

        return res;


    }


    return -1;
}



Trajectory_t DataStructure::ConstructRandomPathQuick(int maxLength, NodeMapGraph graph)
{
    auto path = Trajectory_t();
    auto item = graph.begin();

    srand(time(NULL));
    auto random = (rand() % (graph.size()-1));

    advance(item, random);


    auto v = item->second.head;

    for (int j = 0; j < maxLength; ++j) {

        double cost = CostCalc(v->edge, 1);
        auto target = v->edge;
        auto targetV = v;

        while(v != nullptr )
        {
            auto val = CostCalc(v->edge, 1);

            if(val <= cost)
            {
                target = v->edge;
                cost = val;
                targetV = v;
            }

            v = v->next;
        }



        path.emplace_back(make_tuple(target,cost));
        if(graph.find(targetV->nodeId) != graph.end())
        {
            v = graph.find(targetV->nodeId)->second.head;
        } else
        {
            break;
        }

    }


    return path;
}




/*
 *
 * =================================
 *
 *  New pathfinding
 *
 * =================================
 */


vector<osmium::object_id_type>
DataStructure::CalculatePathNew(osmium::object_id_type startNode, osmium::object_id_type endNode, NodeMapGraph graph)
{
    auto Q = vector<osmium::object_id_type>(); //lost of nodes

    unordered_map<osmium::object_id_type, long> dist; //dist to node
    unordered_map<osmium::object_id_type , osmium::object_id_type> prev; //node to node


    for (auto i = graph.begin(); i != graph.end() ; ++i)
    {
        dist[i->first] = std::numeric_limits<long>::max();
        prev[i->first] = -1;
        Q.push_back(i->first);

    }

    dist[startNode] = 0;

    while(!Q.empty())
    {
        osmium::object_id_type u = this->FindMinDistNew(&dist, &Q);


        if(u == endNode)
        {
            vector<osmium::object_id_type> S;

            while(prev[u] != -1)
            {
                S.insert(S.begin(),u);
                u = prev[u];
            }

            S.insert(S.begin(), u);

            return S;
        }

        Q.erase(remove(Q.begin(), Q.end(), u), Q.end());
        auto bStart = clock();


        auto v = graph[u].head;
        //auto inQ = InListNew(v->nodeId, Q);
        while(v != NULL)
        {
            auto distu = dist[u];
            auto ideal = EVList[v->edge].idealCost;

            auto alt =  distu + ideal;

            if(alt < dist[v->nodeId])
            {
                dist[v->nodeId] = static_cast<long>(alt);
                prev[v->nodeId] = u;
            }
            if(v->next == NULL)
                break;
            v = v->next;
        }
    }
    //return path;
}

osmium::object_id_type DataStructure::FindMinDistNew(unordered_map<osmium::object_id_type, long> *dist, vector<osmium::object_id_type> *Q)
{

    long distance = std::numeric_limits<long>::max();
    osmium::object_id_type target = -1;


    for (auto i : *Q)
    {
        auto it = dist->find(i);

        if(it != dist->end())
        {
            if(it->second <= distance)
            {
                distance = it->second;
                target = i;
            }
        }
    }
    return target;
}

bool DataStructure::InListNew(osmium::object_id_type element, vector<osmium::object_id_type> list)
{

    //return true; //(find(list->begin(), list->end(), *element) != list->end());
    bool result = (find(list.begin(), list.end(), element) != list.end());
    return result;
}







