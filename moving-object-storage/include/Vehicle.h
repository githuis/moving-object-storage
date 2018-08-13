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
    unordered_map<osmium::object_id_type, long*> trajectoryMap{};

    Vehicle() = default;

    explicit Vehicle(long id)
    {
        Id = id;
    }

    Vehicle(long id, std::vector<std::tuple<osmium::object_id_type, long>> traj)
    {
        Id = id;
        trajectory = traj;
        for (auto i : traj) {
            trajectoryMap[get<0>(i)] = &get<1>(i);
        }
    }

    void UpdateTrajectory(std::vector<std::tuple<osmium::object_id_type, long>> newTrajectory)
    {
        trajectoryMap.empty();
        trajectory = newTrajectory;
        for (auto &i : trajectory ) {
            trajectoryMap[get<0>(i)] = &get<1>(i);
        }




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
