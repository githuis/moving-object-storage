//
// Created by bolhuis on 12/6/17.
// Inspiration by: https://gist.github.com/charlierm/5691020
//

#ifndef MOVING_OBJECT_STORAGE_LINKEDLIST_H
#define MOVING_OBJECT_STORAGE_LINKEDLIST_H

#include <osmium/osm/types.hpp>

class GraphNode
{
public:
    osmium::object_id_type nodeId;
    osmium::object_id_type edge;
    GraphNode *next;
};

class NeighbourList
{
public:
    int length;
    GraphNode* head;

    NeighbourList();
    ~NeighbourList();

    void add(osmium::object_id_type nodeid, osmium::object_id_type edge);
    void print();
};


#endif //MOVING_OBJECT_STORAGE_LINKEDLIST_H
