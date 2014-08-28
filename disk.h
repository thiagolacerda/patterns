#ifndef Disk_h
#define Disk_h

#include <memory>
#include <unordered_set>
#include <vector>

class Grid;
class Trajectory;

class Disk {
public:
    Disk()
        : m_centerX(0)
        , m_centerY(0)
    { }

    Disk(double centerX, double centerY)
        : m_centerX(centerX)
        , m_centerY(centerY)
    { }

    double centerX() const { return m_centerX; }
    void setCenterX(double centerX) { m_centerX = centerX; }

    double centerY() const { return m_centerY; }
    void setCenterY(double centerY) { m_centerY = centerY; }

    void addTrajectory(const std::shared_ptr<Trajectory>&);
    void addAlreadyComputedGrids(const std::vector<std::shared_ptr<Grid>>&);
    void addAlreadyComputedGrid(const std::shared_ptr<Grid>&);
    bool isGridAlreadyComputed(const std::shared_ptr<Grid>& grid);
    const std::unordered_set<std::shared_ptr<Trajectory>>& trajectories() { return m_trajectories; }

private:
    double m_centerX;
    double m_centerY;
    std::unordered_set<std::shared_ptr<Trajectory>> m_trajectories;
    std::unordered_set<std::shared_ptr<Grid>> m_alreadyComputed;
};

#endif // Disk_h
