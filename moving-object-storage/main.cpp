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
    string eID;
    tie(eID,time) = ds -> testTra();

    return 0;
}
