#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <string>
#include <list>
#include <tuple>

#define Trajectory_t std::tuple<uint64_t, uint64_t>
#define List_Trajectory_t std::list<Trajectory_t>


class DataStructure
{



    public:
        DataStructure();
        explicit DataStructure(Trajectory_t currentTrajectory);
        explicit DataStructure(List_Trajectory_t plannedTrajectory);
        DataStructure(Trajectory_t currentTrajectory, List_Trajectory_t plannedTrajectory);

        virtual ~DataStructure();
        static void Test();

        //Trajectory
        std::tuple<int, int> testTra();
        void MakeTest();
        Trajectory_t current;
        List_Trajectory_t planned;

        //Accessors


    protected:

    private:
};

#endif // DATASTRUCTURE_H
