#ifndef TrajectoryManager_h
#define TrajectoryManager_h

#include <unordered_map>

class Trajectory;

class TrajectoryManager {
public:
    TrajectoryManager() { }

    void addTrajectory(Trajectory*);
    Trajectory* trajectoryById(unsigned long);
    void dump();

private:
    std::unordered_map<unsigned long, Trajectory*> m_trajectories;
};

#endif // TrajectoryManager_h
