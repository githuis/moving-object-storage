//
// Created by bolhuis on 12/13/17.
//

#ifndef MOVING_OBJECT_STORAGE_VEHICLE_H
#define MOVING_OBJECT_STORAGE_VEHICLE_H


#include <osmium/osm/types.hpp>
#include <tuple>
#include <vector>

using namespace std;


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

    void UpdateTrajectory(std::vector<std::tuple<osmium::object_id_type, long>> newTrajectory)
    {
        for (auto &i : trajectory ) {


        }

        trajectory = newTrajectory;


    }

    void UpdateTime(long deltaTime)
    {
        //for each edge in trajectory time += deltaTime.
        for (int i = 0; i < trajectory.size(); ++i) {
            get<1>(trajectory[i]) += deltaTime;
        }
    }
};


#endif //MOVING_OBJECT_STORAGE_VEHICLE_H
