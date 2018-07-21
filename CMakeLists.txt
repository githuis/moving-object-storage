cmake_minimum_required(VERSION 3.8)
project(moving_object_storage)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Osmium REQUIRED COMPONENTS <XXX>)
include_directories(SYSTEM ${OSMIUM_INCLUDE_DIRS})

find_package(Protozero REQUIRED)
include_directories(SYSTEM ${PROTOZERO_INCLUDE_DIR})

#set(GEM_DEBUG TRUE)
#find_package(Gem COMPONENTS rice hoe)
#include_directories(${GEM_INCLUDE_DIRS})
#target_link_libraries(${GEM_LIBRARIES})


set(SOURCE_FILES
        moving-object-storage/src/DataStructure.cpp
        moving-object-storage/main.cpp moving-object-storage/src/operations.cpp moving-object-storage/src/Osm.cpp moving-object-storage/include/Osm.h moving-object-storage/src/NeighbourList.cpp moving-object-storage/include/NeighbourList.h moving-object-storage/include/Vehicle.h moving-object-storage/include/EdgeVehicleReference.h)

add_executable(moving_object_storage ${SOURCE_FILES} moving-object-storage/src/operations.cpp)


target_link_libraries(moving_object_storage z bz2 expat pthread)