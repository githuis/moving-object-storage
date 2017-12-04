#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <string>
#include <list>
#include <tuple>
#include <ctime>

#define Trajectory_t std::tuple<uint64_t, uint64_t>
#define List_Trajectory_t std::list<Trajectory_t>
#define Vehicle std::tuple<uint64_t,List_Trajectory_t>


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

        //Vehicle
        std::tuple<int,Trajectory_t> testVec();
        void MakeVechicle();


        //Grid

        //Accessors


    protected:

    private:
    //Vehicle
    void Insert(Vehicle currentVehicle,
                List_Trajectory_t trajectory);

    void Delete(Vehicle currentVehicle);

    void TrajectoryUpdate(Vehicle currentVehicle, List_Trajectory_t trajectory);

    void InsertVehiclesIntoEdge(Vehicle currentVehicle,
                                std::list<uint64_t> &edges);
    void DeleteVehiclesFromEdge(Vehicle currentVehicle,
                                std::list<uint64_t> &edges);
    void TimeUpdate(Vehicle currentVehicle, int64_t time);
};

#endif // DATASTRUCTURE_H
