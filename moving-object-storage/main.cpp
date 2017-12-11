#include <iostream>
#include "include/DataStructure.h"
#include "include/Osm.h"
#include <string>
#include <tuple>

using namespace std;

int main(int argc, char* argv[])
{
    cout << "Hello" << endl;
    DataStructure *ds = new DataStructure();
    ds->Test();

    int time;
    int eID;
    tie(eID,time) = ds -> testTra();

    cout << "Edge id: " << eID << " At time: " << time << endl;


    //Test Osmium
    Osm *osm = new Osm();
    //osm->LocationTest();
    //osm->AddPoiTest(argc, argv);

    osm->GraphBuilderTest(osm->NodeWayBuilder());


    cout << "End of running" << endl;




    return 0;
}
