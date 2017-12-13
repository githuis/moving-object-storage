#include <iostream>
#include <tuple>
#include <functional>
#include "../include/DataStructure.h"

DataStructure::DataStructure()
{
    EVList =EdgeVehicleList();
    //ctor
}

DataStructure::~DataStructure()
{
    //dtor
}

void DataStructure::Test()
{
    std::cout << "Created datastructure" << std::endl;
}

Trajectory_t DataStructure::testTra()
{
    auto x = std::vector<std::tuple<osmium::object_id_type, long>>();
    x.push_back(std::make_tuple(30249577, 0));
    x.push_back(std::make_tuple(7867288, 2));
    x.push_back(std::make_tuple(30249577, 4));

    return x;
}

EdgeVehicleList DataStructure::EVListBuilder(vector<osmium::object_id_type> allWays)
{
    //auto x = vector<tuple<osmium::object_id_type, vector<tuple<long, vector<tuple<osmium::object_id_type, long>*>> *>>>();
    auto x = map<osmium::object_id_type, EdgeVehicleRefrence>();

    for (int i = 0; i < allWays.size(); ++i) {
        vector<tuple<Vehicle *, long>> y = vector<tuple<Vehicle *, long>>();

        //TODO it->first is a node, we should find all ways
        auto EVR = EdgeVehicleRefrence(allWays[i]);
        EVR.vehicles = y;
        x[allWays[i]] = EVR;
    }

    //std::map<osmium::object_id_type, NeighbourList>::iterator it;
//
    //for (it = graph.begin(); it != graph.end(); ++it)
    //{
    //    vector<Vehicle*> y = vector<Vehicle*>();
//
    //    //TODO it->first is a node, we should find all ways
    //    auto EVR = EdgeVehicleRefrence(it->first);
    //    EVR.vehicles = y;
//
    //    x[it->first] = EVR;
//
//
    //}

    return x;
}

void DataStructure::AddVehicleToEVList(Vehicle v)
{
    if(v.trajectory.empty() || v.trajectory.size() == 0)
    {
        cout << "Vehicle has no (an empty) trajectory!!!!" << endl;
        return;
    }

    auto it = map<osmium::object_id_type, EdgeVehicleRefrence>::iterator();

    auto list = this->FindAllEdgesWithArrivalTime(v.trajectory);

    for (int i = 0; i < list.size(); ++i) {

        it = EVList.find(get<0>(list[i]));
        if(it != EVList.end())
        {
            auto carWithArrivalTime = make_tuple(&v, get<1>(list[i]));
            EVList[get<0>(list[i])].vehicles.push_back(carWithArrivalTime);
        } else{
            cout << "Edge not found!" << endl;
        }
    }
}

vector<tuple<osmium::object_id_type, long>> DataStructure::FindAllEdgesWithArrivalTime(Trajectory_t traj)
{
    auto list = vector<tuple<osmium::object_id_type, long>>();

    for (int i = 0; i < traj.size(); ++i) {
        auto x = make_tuple((long &&)get<0>(traj[i]), get<1>(traj[i]));
        list.push_back(x);
    }

    return list;


    //return vector<vector<tuple<osmium::object_id_type, long>>>();
}




