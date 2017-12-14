//
// Created by bolhuis on 12/4/17.
//

#ifndef MOVING_OBJECT_STORAGE_OSM_H
#define MOVING_OBJECT_STORAGE_OSM_H


#include <osmium/handler.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/geom/haversine.hpp>
#include <iostream>
#include "NeighbourList.h"
#include <list>

#define NodeMapGraph std::map<osmium::object_id_type, NeighbourList>

class Osm {
public:
    Osm();

    std::vector<osmium::object_id_type> AllWays;
    std::vector<long> IdealCost;

    void AddPoiTest(int argc, char **argv);

    void LocationTest();

    NodeMapGraph GraphBuilder(std::map<osmium::object_id_type, NeighbourList> verticies, std::string fileName); //Builds neighbours

    NodeMapGraph NodeWayBuilder(std::string fileName); //Builds first array of nodes

    osmium::object_id_type FindClosestWay(std::string file, osmium::Location target);


protected:

    //TODO cars on edge list herepls

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
        std::map<osmium::object_id_type, NeighbourList> moreThanOneConnectionMap;
        std::vector<osmium::object_id_type> allWays;
        std::vector<long> idealCost;
        std::map<osmium::object_id_type, NeighbourList>::iterator it;
        bool init;

        void way(const osmium::Way &way)
        {
            const char *highway = way.tags()["highway"];

            if (!highway)
                return;

            //We don't want to include cycleways, so if the highway is a cycleway, skip it.
            if(std::string(highway) == cycleHighWayTag)
            {
                //std::cout << "IT'S A CYCLEWAY, GET OUT" << std::endl;

                return;
            }

            if (init) {
                allWays.push_back(way.id());
                idealCost.push_back(osmium::geom::haversine::distance(way));

                NeighbourList *list = new NeighbourList();

                for (auto i = way.nodes().begin(); i != way.nodes().end(); ++i) {

                    it = moreThanOneConnectionMap.find(i->ref());

                    if (it == moreThanOneConnectionMap.end())
                        continue;


                    map[i->ref()] = *list;


                }
            } else {

                //i : osmium::OSMObject::const_iterator
                // http://www.cplusplus.com/reference/map/map/find/

                std::list<osmium::object_id_type> lst;
                for (auto i = way.nodes().begin(); i != way.nodes().end(); ++i) {
                    it = moreThanOneConnectionMap.find(i->ref());

                    if (it != moreThanOneConnectionMap.end()) {
                        lst.push_back(i->ref());
                    }
                }

                //std::cout << "List len: " << lst.size() << "\tNodes: " << way.nodes().size() << std::endl;
                bool foundSelf = false;

                for (auto i = way.nodes().begin(); i != way.nodes().end(); ++i) {
                    it = map.find(i->ref());

                    if (it != map.end()) {
                        foundSelf = false;
                        osmium::object_id_type temp = -1;
                        for (std::list<osmium::object_id_type>::iterator j = lst.begin(); j != lst.end(); ++j) {

                            if (*j == i->ref()) // Node i should not be able to go to node i
                            {
                                foundSelf = true;
                                if(temp != -1)
                                    map[i->ref()].add(temp, way.id());
                                continue;
                            }

                            auto l = map[i->ref()].head;
                            bool addFlag = true; //Assume we should not add node *j
                            for (int k = 0; k < map[i->ref()].length; ++k) {
                                if (l->nodeId != *j) {
                                    l = l->next;
                                    continue;
                                } else {
                                    addFlag = true;
                                }
                            }

                            if (!addFlag)
                                continue;

                            if (!foundSelf) {
                                temp = *j;
                            } else {
                                map[i->ref()].add(*j, way.id());
                                break;

                            }


                        }
                    }
                }
            }
        }
    }; // struct GraphBuilderHandler


    struct NodeWayAssignmentHandler : public osmium::handler::Handler {

        std::map<osmium::object_id_type, NeighbourList> partOfWayMap;

        void way(const osmium::Way &way)
        {
            const char *highway = way.tags()["highway"];

            if (!highway)
                return;

            //We don't want to include cycleways, so if the highway is a cycleway, skip it.
            if(std::string(highway) == cycleHighWayTag)
            {
                //std::cout << "IT'S A CYCLEWAY, GET OUT" << std::endl;

                return;
            }

            NeighbourList *list = new NeighbourList();


            for (auto i = way.nodes().begin(); i != way.nodes().end(); ++i)
            {
                partOfWayMap[i->ref()].add(i->ref(), way.id());
            }
        }

        void DeleteSingles()
        {
            for (auto j = partOfWayMap.begin(); j != partOfWayMap.end(); ++j) {
                if (j->second.length == 1) {
                    partOfWayMap.erase(j->first);
                }
            }
        }


    }; // struct NodeWayAssignmentHandler


private:

    static const std::string cycleHighWayTag;

    void AddPoi(int argc, char *const *argv) const;


};


#endif //MOVING_OBJECT_STORAGE_OSM_H
