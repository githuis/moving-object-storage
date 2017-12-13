//
// Created by bolhuis on 12/13/17.
//

#ifndef MOVING_OBJECT_STORAGE_VEHICLE_H
#define MOVING_OBJECT_STORAGE_VEHICLE_H


#include <osmium/osm/types.hpp>
#include <tuple>
#include <vector>


class Vehicle {

public:

    long Id = -1; //-1 means uninitialized!
    std::vector<std::tuple<osmium::object_id_type, long>> trajectory{};

    Vehicle() = default;

    explicit Vehicle(long id)
    {
        Id = id;
    }

    Vehicle(long id, std::vector<std::tuple<osmium::object_id_type, long>> traj)
    {
        Id = id;
        trajectory = traj;
    }

    //Vehicle(long id, Trajectory_t traj)
    //{
    //    Id = id;
    //    trajectory = traj;
    //    //for (int i = 0; i < evref->size(); ++i) {
    //    //    if(evref[0][i].edge
    //    //}
    //}


    void UpdateTrajectory(std::vector<std::tuple<osmium::object_id_type, long>> newTrajectory)
    {
        this->trajectory = newTrajectory;
    }

    void UpdateTime(long deltaTime)
    {
        //for each edge in trajectory time += deltaTime.
    }
};


#endif //MOVING_OBJECT_STORAGE_VEHICLE_H
