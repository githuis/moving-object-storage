//
// Created by bolhuis on 12/4/17.
//

#ifndef MOVING_OBJECT_STORAGE_OSM_H
#define MOVING_OBJECT_STORAGE_OSM_H


#include <osmium/handler.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <iostream>


class Osm {
public:
    Osm();

    void AddPoiTest(int argc, char **argv);

    void LocationTest();

    osmium::object_id_type FindClosestWay(std::string file, osmium::Location target);


protected:


    //Taken from RoadLengthHandler in the roadlength osmium example.
    //Finds the closest node in a way
    struct ClosestWayHandler : public osmium::handler::Handler {

        //Set distance to the max value of double (is good enough right?)
        double distance = std::numeric_limits<double>::max();
        osmium::object_id_type closestWayId;

        osmium::Location targetLocation;


        void way(const osmium::Way &way)
        {


            const char *highway = way.tags()["highway"];
            const char *name = way.tags()["name"];

            if (highway) {
                for (auto i = way.nodes().begin(); i != way.nodes().end(); ++i) {
                    //i->location().x//
                    double dist = distanceCalculate(targetLocation.x(), targetLocation.y(), i->location().x(),
                                                    i->location().y());


                    if(name)
                    {
                        std::cout << name << " Id: " << way.id() << " Distance: " << (dist /100000000)<< std::endl;
                    }

                    if (dist < distance) {
                        distance = dist;
                        closestWayId = way.id();
                    }
                }
            }


        }

    }; // struct ClosestWayHandler

    //From http://www.cplusplus.com/forum/beginner/178293/
    static double distanceCalculate(double x1, double y1, double x2, double y2)
    {
        double x = x1 - x2; //calculating number to square in next step
        double y = y1 - y2;
        double dist;

        dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
        dist = sqrt(dist);

        return dist;
    }

private:
    void AddPoi(int argc, char *const *argv) const;


};


#endif //MOVING_OBJECT_STORAGE_OSM_H
