//
// Created by Thor Jensen on 23/11/2017.
//
#include <iostream>
#include <tuple>
#include <functional>
#include "../include/DataStructure.h"

void DataStructure::Insert(Vehicle currentVehicle, Trajectory_t currentTrajectory)
{

}

void DataStructure::Delete(Vehicle currentVehicle)
{
    Trajectory_t trajectory;
    trajectory = std::get<1>(currentVehicle);
    DeleteVehiclesFromEdge(currentVehicle,trajectory);

}

void DataStructure::TrajectoryUpdate(Vehicle currentVehicle)
{

}

void DataStructure::TimeUpdate(Vehicle currentVehicle, uint64_t time)
{

}

void DataStructure::DeleteVehiclesFromEdge(Vehicle currentVehicle, Trajectory_t currentTrajectory)
{

}

void DataStructure::InsertVehiclesIntoEdge(Vehicle currentVehicle, Trajectory_t currentTrajectory)
{

}