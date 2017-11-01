#include <iostream>
#include "include/DataStructure.h"
#include <string>
#include <tuple>

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    DataStructure *ds = new DataStructure();
    ds->Test();

    int time;
    int eID;
    tie(eID,time) = ds -> testTra();

    cout << "Edge id: " << eID << " At time: " << time << endl;

    return 0;
}
