#include <iostream>
#include "../include/DataStructure.h"


DataStructure::DataStructure()
{
    //ctor
}

DataStructure::~DataStructure()
{
    //dtor
}

void DataStructure::Test()
{
    std::cout << "Hello Jesper" << std::endl;
}

std::tuple<std::string, int> testTra()
{
    return{"2",2};
};
