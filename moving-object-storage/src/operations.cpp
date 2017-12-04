//
// Created by Thor Jensen on 23/11/2017.
//
#include <iostream>
#include <tuple>
#include <functional>
#include "../include/DataStructure.h"
#include <algorithm>
#include <list>


void DataStructure::Insert(Vehicle currentVehicle, List_Trajectory_t currentTrajectory)
{
    //add til trajectories

    List_Trajectory_t trajectory;
    trajectory = std::get<1>(currentVehicle);
    std::list<uint64_t> edges;
    std::list<uint64_t >::iterator it;
    for (std::list<Trajectory_t>::iterator traElement = trajectory.begin(); traElement != trajectory.end(); ++traElement)
    {
        uint64_t insert;
        insert = std::get<0>(*traElement);
        edges.insert(it,insert);
    }
    InsertVehiclesIntoEdge(currentVehicle,edges);

    //set in i vehicle database?

}

void DataStructure::Delete(Vehicle currentVehicle)
{
    //fjern fra trajectories
    List_Trajectory_t trajectory;
    trajectory = std::get<1>(currentVehicle);
    std::list<uint64_t> edges;
    std::list<uint64_t >::iterator it;
    for (std::list<Trajectory_t>::iterator traElement = trajectory.begin(); traElement != trajectory.end(); ++traElement)
    {
        uint64_t insert;
        insert = std::get<0>(*traElement);
        edges.insert(it,insert);
    }
    DeleteVehiclesFromEdge(currentVehicle,edges);

    //fjern fra vehicle database?



}

void DataStructure::TrajectoryUpdate(Vehicle currentVehicle, List_Trajectory_t trajectory)
{
    List_Trajectory_t trajOld;
    List_Trajectory_t *tPointer;
    tPointer = &std::get<1>(currentVehicle);
    trajOld = std::get<1>(currentVehicle);
    std::list<uint64_t > oldEdges;
    std::list<uint64_t > newEdges;
    std::list<uint64_t > onlyNew;
    std::list<uint64_t > onlyOld;
    std::list<uint64_t >::iterator it;
    for (std::list<Trajectory_t>::iterator traElement = trajectory.begin(); traElement != trajectory.end(); ++traElement)
    {
        uint64_t insert;
        insert = std::get<0>(*traElement);
        newEdges.insert(it,insert);
    }
    for (std::list<Trajectory_t>::iterator traElement = trajOld.begin(); traElement != trajOld.end(); ++traElement)
    {
        uint64_t insert;
        insert = std::get<0>(*traElement);
        oldEdges.insert(it,insert);
    }
    std::set_difference(newEdges.begin(),newEdges.end(),oldEdges.begin(),oldEdges.end(),std::inserter(onlyNew,onlyNew.end()));
    std::set_difference(oldEdges.begin(),oldEdges.end(),newEdges.begin(),newEdges.end(),std::inserter(onlyOld,onlyOld.end()));

    InsertVehiclesIntoEdge(currentVehicle, onlyNew);
    DeleteVehiclesFromEdge(currentVehicle, onlyOld);


    *tPointer = trajectory;
}

void DataStructure::TimeUpdate(Vehicle currentVehicle, int64_t time)
{
    List_Trajectory_t trajectory;
    trajectory = std::get<1>(currentVehicle);
    ulong * timePointer;

    for (std::list<Trajectory_t>::iterator traElement = trajectory.begin(); traElement != trajectory.end(); ++traElement)
    {
        timePointer = &std::get<1>(*traElement);
        *timePointer += time;
    }
}

void DataStructure::DeleteVehiclesFromEdge(Vehicle currentVehicle, std::list<uint64_t> &edges)
{

}

void DataStructure::InsertVehiclesIntoEdge(Vehicle currentVehicle, std::list<uint64_t> &edges)
{

}