//
// Created by bolhuis on 12/6/17.
//

#include <cstdlib>
#include <iostream>
#include "../include/NeighbourList.h"

NeighbourList::NeighbourList()
{
    this->length = 0;
    this->head = NULL;

}

NeighbourList::~NeighbourList()
{
    std::cout << "Destroyed a neighbour list" << std::endl;

}

void NeighbourList::add(osmium::object_id_type nodeid, osmium::object_id_type edge)
{
    GraphNode* node = new GraphNode();
    node->nodeId = nodeid;
    node->edge = edge;
    node->next = this->head;
    this->head = node;
    this->length++;
}

void NeighbourList::print()
{
    GraphNode *temp = this->head;
    int i = 0;

    while(temp)
    {
        std::cout << i << ": " << temp->nodeId << " and edge " << temp->edge << std::endl;
        temp = temp->next;
        ++i;
    }
}
