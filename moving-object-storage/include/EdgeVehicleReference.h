//
// Created by bolhuis on 12/13/17.
//

#ifndef MOVING_OBJECT_STORAGE_EDGEVEHICLEREFRENCE_H
#define MOVING_OBJECT_STORAGE_EDGEVEHICLEREFRENCE_H


#include <osmium/osm/types.hpp>
#include "Vehicle.h"

using namespace std;

class EdgeVehicleReference {

public:
    EdgeVehicleReference()= default;
    explicit EdgeVehicleReference(osmium::object_id_type id)
    {
        edge = id;
    }

    osmium::object_id_type edge{(long) -1};
    //           Vehicle  , When   it is here
    double idealCost = std::numeric_limits<double>::infinity();
    vector<Vehicle *> vehicles;

    double length;
};


#endif //MOVING_OBJECT_STORAGE_EDGEVEHICLEREFRENCE_H
