//
// Created by bolhuis on 12/4/17.
//

#ifndef MOVING_OBJECT_STORAGE_OSM_H
#define MOVING_OBJECT_STORAGE_OSM_H


#include <osmium/handler.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <iostream>
#include "NeighbourList.h"


class Osm {
public:
    Osm();

    void AddPoiTest(int argc, char **argv);

    void LocationTest();

    void GraphBuilderTest();

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
                    double dist = distanceCalculate(i->location().x(), i->location().y(), targetLocation.x(),
                                                    targetLocation.y());

                    if (name == "Selma Lagerløfs Vej") {
                        std::cout << " Id: " << way.id() << " Distance: " << (dist / 100000000) << std::endl;

                    }

                    if (dist < distance) {
                        distance = dist;
                        closestWayId = way.id();
                        std::cout << "Node: " << i->ref() << " Id: " << way.id() << " Distance: " << (dist / 100000000)
                                  << std::endl;

                    }
                }
            }
        }

    }; // struct ClosestWayHandler

    //From http://www.cplusplus.com/forum/beginner/178293/
    static double distanceCalculate(double x1, double y1, double x2, double y2)
    {
        double x = x2 - x1; //calculating number to square in next step
        double y = y2 - y1;
        double dist;

        dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
        dist = sqrt(dist);

        return dist;
    }

    //Taken from RoadLengthHandler in the roadlength osmium example.
    //Finds all intersection nodes
    struct GraphBuilderHandler : public osmium::handler::Handler {

        std::map<osmium::object_id_type, NeighbourList> map;
        std::map<osmium::object_id_type, NeighbourList>::iterator it;
        bool init;

        void way(const osmium::Way &way)
        {
            const char *highway = way.tags()["highway"];

            if (!highway)
                return;

            if (init)
            {
                auto startNode = way.nodes().front();
                auto endNode = way.nodes().back();

                NeighbourList *list = new NeighbourList();

                list->add(endNode.ref(), way.id());

                map[startNode.ref()] = *list;
            }
            else
            {

                //i : osmium::OSMObject::const_iterator
                // http://www.cplusplus.com/reference/map/map/find/
                for (auto i = way.nodes().begin(); i != way.nodes().end(); ++i) {
                    it = map.find(i->ref());
                    if(it != map.end() && i->ref() != way.nodes().front().ref())
                    {
                        std::cout << way.nodes().front().data() << std::endl;
                        map[i->ref()].add(way.nodes().front().ref(), way.id());
                        map[i->ref()].add(way.nodes().back().ref(), way.id());

                    }
                }
            }

        }


    }; // struct GraphBuilderHandler




private:
    void AddPoi(int argc, char *const *argv) const;


};


#endif //MOVING_OBJECT_STORAGE_OSM_H
