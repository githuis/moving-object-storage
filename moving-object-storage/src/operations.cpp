//
// Created by Thor Jensen on 23/11/2017.
//
#include <iostream>
#include <tuple>
#include <functional>
#include "../include/DataStructure.h"
#include <list>

void DataStructure::Insert(Vehicle currentVehicle, List_Trajectory_t currentTrajectory)
{

}

void DataStructure::Delete(Vehicle currentVehicle)
{
    List_Trajectory_t trajectory;
    trajectory = std::get<1>(currentVehicle);
    DeleteVehiclesFromEdge(currentVehicle,trajectory);

}

void DataStructure::TrajectoryUpdate(Vehicle currentVehicle, List_Trajectory_t trajectory)
{

}

void DataStructure::TimeUpdate(Vehicle currentVehicle, int64_t time)
{
    List_Trajectory_t trajectory;
    trajectory = std::get<1>(currentVehicle);

    for (std::list<Trajectory_t>::iterator traTime = trajectory.begin(); traTime != trajectory.end(); ++traTime)
    {
        ulong * timePointer;
        timePointer = &std::get<1>(*traTime);
        *timePointer += time;
    }
}

void DataStructure::DeleteVehiclesFromEdge(Vehicle currentVehicle, List_Trajectory_t currentTrajectory)
{

}

void DataStructure::InsertVehiclesIntoEdge(Vehicle currentVehicle, List_Trajectory_t currentTrajectory)
{

}