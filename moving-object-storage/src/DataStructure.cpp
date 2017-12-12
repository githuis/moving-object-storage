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

EdgeVehicleList DataStructure::EVListBuilder(NodeMapGraph graph)
{
    auto x = vector<tuple<osmium::object_id_type, vector<tuple<long, vector<tuple<osmium::object_id_type, long>>> *>>>();
    std::map<osmium::object_id_type, NeighbourList>::iterator it;

    for (it = graph.begin(); it != graph.end(); ++it)
    {
        auto y = vector<Vehicle*>();
        x.push_back(make_tuple(it->first, y));

    }
    
}




