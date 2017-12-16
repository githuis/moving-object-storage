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
    auto x = map<osmium::object_id_type, EdgeVehicleReference>();

    for (int i = 0; i < allWays.size(); ++i) {
        vector<Vehicle *> y = vector<Vehicle *>();

        auto EVR = EdgeVehicleReference(allWays[i]);
        EVR.vehicles = y;
        EVR.idealCost = get<0>(costAndLength[i]);
        EVR.length = get<1>(costAndLength[i]);

        x[allWays[i]] = EVR;
    }

    //std::map<osmium::object_id_type, NeighbourList>::iterator it;
//
    //for (it = graph.begin(); it != graph.end(); ++it)
    //{
    //    vector<Vehicle*> y = vector<Vehicle*>();
//
    //    auto EVR = EdgeVehicleReference(it->first);
    //    EVR.vehicles = y;
//
    //    x[it->first] = EVR;
//
//
    //}

    return x;
}

void DataStructure::Insert(Vehicle v)
{
    if (v.trajectory.empty() || v.trajectory.size() == 0) {
        cout << "Vehicle has no (an empty) trajectory!!!!" << endl;
        return;
    }

    auto it = map<osmium::object_id_type, EdgeVehicleReference>::iterator();

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

vector<osmium::object_id_type>
DataStructure::CalculatePath(osmium::object_id_type startNode, osmium::object_id_type endNode, NodeMapGraph graph)
{
    auto Q = vector<osmium::object_id_type>();
    bool endNodeInData = false;

    map<osmium::object_id_type, double> dist;
    map<osmium::object_id_type , osmium::object_id_type> prev;


    for (auto i = graph.begin(); i != graph.end() ; ++i)
    {

        dist[i->first] = std::numeric_limits<double>::max();
        prev[i->first] = -1;
        Q.push_back(i->first);

        if(i->first == endNode)
        {
            endNodeInData = true;
        }
    }

    if(!endNodeInData)
        return vector<osmium::object_id_type>();

    dist[startNode] = 0;

    while(!Q.empty())
    {
        osmium::object_id_type u = this->FindMinDist(&dist, &Q);

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

        std::cout << Q.size() << endl;

        for (int i = 0; i <Q.size(); ++i) {
            if(Q[i] == u)
            {
                Q.erase(Q.begin() + i);
                i--;
            }
        }
        Q.erase(    remove(Q.begin(), Q.end(), u),    Q.end());
        std::cout << Q.size() << endl;


        bool inQ;// = InList(v->nodeId, Q);

        if(u == -1)
            continue;


        auto v = graph[u].head;
        if(v != NULL)
             inQ = InList(v->nodeId, Q);
        else
            continue;

        while(v->next != NULL && inQ)
        {
            //auto v = graph[u].head;
            auto alt = dist[u] + EVList[v->edge].idealCost;

            if(alt < dist[v->nodeId])
            {
                dist[v->nodeId] = static_cast<long>(alt);
                prev[v->nodeId] = u;
            }
            if(v->next == nullptr || v->next == NULL)
                break;
            v = v->next;

        }
    }


    auto emptyPath = vector<osmium::object_id_type>();
    emptyPath.push_back(startNode);
    return emptyPath;
    //return path;
}


vector<osmium::object_id_type>
DataStructure::CalculatePathAStar(osmium::object_id_type startNode, osmium::object_id_type endNode, NodeMapGraph graph)
{
    auto closedSet = vector<osmium::object_id_type>();
    auto openSet = vector<osmium::object_id_type>();
    openSet.push_back(startNode);
    bool endNodeInData = false;

    map<osmium::object_id_type, double> gScore;
    map<osmium::object_id_type, double> fScore;
    map<osmium::object_id_type , osmium::object_id_type> cameFrom;


    for (auto i = graph.begin(); i != graph.end() ; ++i)
    {

        gScore[i->first] = std::numeric_limits<double>::max();
        fScore[i->first] = std::numeric_limits<double>::max();
        cameFrom[i->first] = -1;
        closedSet.push_back(i->first);

        if(i->first == endNode)
        {
            endNodeInData = true;
        }
    }

    if(!endNodeInData)
        return vector<osmium::object_id_type>();

    gScore[startNode] = 0;
    fScore[startNode] = this->HeuristicCost(startNode, endNode);


    while(!openSet.empty())
    {
        osmium::object_id_type current = this->FindMinDist(&fScore, &openSet);

        if(current == -1)
        {
            cout << "ERROR" << endl;
        }

        if(current == endNode)
        {
            return ReconstructPath(cameFrom, current);
        }


        openSet.erase(  remove(openSet.begin(), openSet.end(), current),  openSet.end());

        closedSet.push_back(current);

        auto neighbour = graph[current].head;
        for (int i = 0; i < graph[current].length; ++i) {
            if(InList(neighbour->nodeId, closedSet))
            {
                continue;
            }

            if(!InList(neighbour->nodeId, openSet))
            {
                openSet.push_back(neighbour->nodeId);
            }

            auto tentativeGScore = gScore[current] + EVList[ neighbour->edge  ].idealCost;

            if(tentativeGScore >= gScore[neighbour->nodeId])
            {
                continue;
            }

            cameFrom[neighbour->nodeId] = current;
            gScore[neighbour->nodeId] = tentativeGScore;
            fScore[neighbour->nodeId] = gScore[neighbour->nodeId] + HeuristicCost(neighbour->nodeId, endNode);

            neighbour = neighbour->next;
        }

    }

    throw std::invalid_argument("No valid path!");


}

vector<osmium::object_id_type>
DataStructure::ReconstructPath(map<osmium::object_id_type , osmium::object_id_type> cameFrom, osmium::object_id_type current)
{
    vector<osmium::object_id_type> totalPath = vector<osmium::object_id_type>();
    totalPath.push_back(current);

    auto it = cameFrom.find(current);

    while(it != cameFrom.end())
    {
        current = cameFrom[current];
        totalPath.push_back(current);
        it = cameFrom.find(current);
    }

    return  totalPath;
}

osmium::object_id_type DataStructure::FindMinDist(map<osmium::object_id_type, double> *fScore, vector<osmium::object_id_type> *openSet)
{
    double distance = std::numeric_limits<double>::max();
    osmium::object_id_type target = -1;


    for (auto i : *openSet)
    {
        auto it = fScore->find(i);

        if(it != fScore->end())
        {
            auto tar = it->second;
            if(tar <= distance)
            {
                distance = it->second;
                target = i;
            }
        }
    }

    return target;
}

bool DataStructure::InList(osmium::object_id_type element, vector<osmium::object_id_type> list)
{
    if(element != NULL)
    {
        for(auto el : list)
        {
            if (el == element)
                return true;
        }
    }
    return false;
    bool res =  (find(list.begin(), list.end(), element) != list.end());
    return res;
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

double DataStructure::HeuristicCost(osmium::object_id_type nodeStart, osmium::object_id_type nodeEnd)
{
    if(this->NodeInNodeLocMap(nodeStart) && this->NodeInNodeLocMap(nodeEnd))
    {
        auto locS = NodeLocations[nodeStart];
        auto locE = NodeLocations[nodeEnd];
        return osmium::geom::haversine::distance(locS, locE) / (80 / 3.6);
    }
    return -1;
}

bool DataStructure::NodeInNodeLocMap(osmium::object_id_type node)
{
    auto it = NodeLocations.find(node);

    return (it != NodeLocations.end());
}








