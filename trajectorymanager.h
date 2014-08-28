#ifndef TrajectoryManager_h
#define TrajectoryManager_h

#include <memory>
#include <unordered_map>

class Trajectory;

class TrajectoryManager {
public:
    TrajectoryManager() { }

    void addTrajectory(const std::shared_ptr<Trajectory>&);
    std::shared_ptr<Trajectory> trajectoryById(unsigned long);
    void dump();

private:
    std::unordered_map<unsigned long, std::shared_ptr<Trajectory>> m_trajectories;
};

#endif // TrajectoryManager_h
