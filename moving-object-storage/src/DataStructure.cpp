#include <iostream>
#include <tuple>
#include <functional>
#include "../include/DataStructure.h"

DataStructure::DataStructure()
{
    //ctor
}

DataStructure::~DataStructure()
{
    //dtor
}

void DataStructure::Test()
{
    std::cout << "Hello Jesper" << std::endl;
}


std::tuple<int, int> DataStructure::testTra()
{
    return std::make_tuple(2,23);
}

DataStructure::DataStructure(Trajectory_t currentTrajectory)
{
    current = currentTrajectory;
}

DataStructure::DataStructure(List_Trajectory_t plannedTrajectory)
{
    planned = plannedTrajectory;

}

DataStructure::DataStructure(Trajectory_t currentTrajectory, List_Trajectory_t plannedTrajectory)
{
    current = currentTrajectory;
    planned = plannedTrajectory;

}

void DataStructure::MakeTest()
{
   // current = new Trajectory_t(2,2);

}




