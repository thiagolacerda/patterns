#ifndef TrajectoryManager_h
#define TrajectoryManager_h

#include <unordered_map>

class Trajectory;

class TrajectoryManager {
public:
    TrajectoryManager() { }

    void addTrajectory(Trajectory*);
    Trajectory* trajectoryById(long);

private:
    std::unordered_map<long, Trajectory*> m_trajectories;
};

#endif // TrajectoryManager_h
