//
// Created by bolhuis on 12/4/17.
//

#include "../include/Osm.h"
#include <iostream>

// Allow any format of input files (XML, PBF, ...)
#include <osmium/io/any_input.hpp>

// For the osmium::geom::haversine::distance() function
#include <osmium/geom/haversine.hpp>

// For osmium::apply()
#include <osmium/visitor.hpp>

// For the location index. There are different types of indexes available.
// This will work for all input files keeping the index in memory.
#include <osmium/index/map/flex_mem.hpp>

// For the NodeLocationForWays handler
#include <osmium/handler/node_locations_for_ways.hpp>

//poi creation
#include <cstring>   // for std::strcmp
#include <ctime>     // for std::time
#include <exception> // for std::exception
#include <string>    // for std::string
#include <utility>   // for std::move

// Allow any format of output files (XML, PBF, ...)
#include <osmium/io/any_output.hpp>

// We want to use the builder interface
#include <osmium/builder/osm_object_builder.hpp>
#include <osmium/builder/attr.hpp>

// The type of index used. This must match the include file above
using index_type = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>;

// The location handler always depends on the index type
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

//For locations (x,y pairs)
#include <osmium/osm/location.hpp>

Osm::Osm()
{

}

void Osm::AddPoiTest(int argc, char *argv[])
{
    AddPoi(argc, argv);
}

void Osm::AddPoi(int argc, char *const *argv) const
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " OUTFILE\n";
        exit(1);
    }

    std::__cxx11::string output_file_name{argv[1]};

    osmium::io::File output_file{output_file_name, output_file_name == "-" ? ".opl" : ""};
    try {
        const size_t initial_buffer_size = 10000;
        osmium::memory::Buffer buffer{initial_buffer_size, osmium::memory::Buffer::auto_grow::no};

        using namespace osmium::builder::attr;

        osmium::builder::add_node(buffer,
                                  _id(-1),
                                  _version(1),
                                  _timestamp(time(nullptr)),
                                  _location(osmium::Location{1.23, 3.45}),
                                  _tag("routepoint", "start")
        );

        osmium::builder::add_node(buffer,
                                  _id(-2),
                                  _version(1),
                                  _timestamp(time(nullptr)),
                                  _location(1.24, 3.46),
                                  _tags({{"routepoint", "end"}})
        );

        std::cout << "Added two pois!" << std::endl;



        osmium::io::Header header;

        header.set("generator", "osmium_create_pois");

        osmium::io::Writer writer{output_file, header, osmium::io::overwrite::no };

        writer(std::move(buffer));

        writer.close();



    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        exit(1);

    }
}

void Osm::LocationTest()
{
    osmium::Location cass{57.01229, 9.99188};
    osmium::Location foetex{57.01825, 9.99365};

    this->FindClosestWay("map.osm", cass);


}

osmium::object_id_type Osm::FindClosestWay(std::string file, osmium::Location target)
{
    std::string fileName = file;
    osmium::io::Reader reader{fileName, osmium::osm_entity_bits::node | osmium::osm_entity_bits::way};
    index_type index;

    ClosestWayHandler handler;

    handler.targetLocation = target;

    location_handler_type locationHandler{index};

    osmium::apply(reader, locationHandler, handler);

    std::cout << "Closest way id is: " << handler.closestWayId << std::endl;
}
