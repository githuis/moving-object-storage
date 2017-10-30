cmake_minimum_required(VERSION 3.8)
project(moving_object_storage)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        moving-object-storage/src/DataStructure.cpp
        moving-object-storage/main.cpp)

add_executable(moving_object_storage ${SOURCE_FILES})