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
    std::vector<std::tuple<double, double>> IdealCost;
    std::map<osmium::object_id_type, osmium::Location> NodeLocations;

    void AddPoiTest(int argc, char **argv);

    void LocationTest();

    NodeMapGraph GraphBuilder(std::map<osmium::object_id_type, NeighbourList> verticies,
                              std::string fileName); //Builds neighbours

    NodeMapGraph NodeWayBuilder(std::string fileName); //Builds first array of nodes

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
                    double dist = osmium::geom::haversine::distance(i->location(), targetLocation);

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

    //Taken from RoadLengthHandler in the roadlength osmium example.
    //Finds all intersection nodes
    struct GraphBuilderHandler : public osmium::handler::Handler {

        std::map<osmium::object_id_type, NeighbourList> map;
        std::map<osmium::object_id_type, NeighbourList> moreThanOneConnectionMap;
        std::vector<osmium::object_id_type> allWays;
        std::vector<std::tuple<double, double >> idealCostAndLength;
        std::map<osmium::object_id_type, osmium::Location> nodeLoc;
        std::map<osmium::object_id_type, NeighbourList>::iterator it;
        bool init;

        void way(const osmium::Way &way)
        {
            const char *highway = way.tags()["highway"];

            //We don't want to include non roads
            if(!highway || !ViableNode(std::string(highway)))
            {
                return;
            }


            if (init) {
                allWays.push_back(way.id());
                double dist = osmium::geom::haversine::distance(way.nodes());
                //dist/80/3.6 is how many seconds it  takes to traverse road at 80kmh
                idealCostAndLength.push_back(std::make_tuple((dist / (80 / 3.6)), dist));


                NeighbourList *list = new NeighbourList();

                for (auto i = way.nodes().begin(); i != way.nodes().end(); ++i) {

                    it = moreThanOneConnectionMap.find(i->ref());

                    if (it == moreThanOneConnectionMap.end())
                        continue;


                    nodeLoc[i->ref()] = i->location();
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
                                if (temp != -1)
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

            //We don't want to include non roads
            if(!highway || !ViableNode(std::string(highway)))
            {
                return;
            }
            NeighbourList *list = new NeighbourList();


            for (auto i = way.nodes().begin(); i != way.nodes().end(); ++i) {
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

    static const std::string cycleHighWayTag, stepsHighWayTag, footHighWayTag, pedestrianHighWayTag;
    static const std::string ignoreTags[];

    void AddPoi(int argc, char *const *argv) const;

    static bool ViableNode(std::string tag)
    {
        for (int i = 0; i < ignoreTags->size(); ++i) {
            if(tag == ignoreTags[i])
                return false;
        }
        return true;
    }


};


#endif //MOVING_OBJECT_STORAGE_OSM_H
