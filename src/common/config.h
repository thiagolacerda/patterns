#ifndef Config_h
#define Config_h

#include <string>
#include <vector>

class Config {
public:
    enum CoordinateSystem {
        WSG84,
        Cartesian,
        CartesianNoConvert
    };

    enum DBDecoder {
        LifeMap,
        Trucks,
        TrucksPaper
    };

    static void setDecoderParameters(const std::vector<std::string>& parameters) { m_decoderParameters = parameters; }
    static std::vector<std::string>& decoderParameters() { return m_decoderParameters; }
    static void setDecoder(char*);
    static DBDecoder decoder() { return m_decoderInUse; }
    static void setGridSize(double);
    static double gridSize() { return m_gridSize; }
    static double radiusSquared() { return m_radiusSquared; }
    static double radius() { return m_radius; }
    static unsigned long flockLength() { return m_flockLength; }
    static void setFlockLength(unsigned long flockLength) { m_flockLength = flockLength; }
    static unsigned numberOfTrajectoriesPerFlock() { return m_trajectoriesPerFlock; }
    static void setNumberOfTrajectoriesPerFlock(unsigned trajectories) { m_trajectoriesPerFlock = trajectories; }
    static void setTimeSlotSize(double size) { m_timeSlotSize = size; }
    static double timeSlotSize() { return m_timeSlotSize; }
    static void setCoordinateSystem(CoordinateSystem coordSystem) { m_coordinateSystem = coordSystem; }
    static CoordinateSystem coordinateSystem() { return m_coordinateSystem; }
    static std::string coordinateSystemName(CoordinateSystem);
    static void setCompatibilityMode(bool compatibility) { m_compatibilityMode = compatibility; }
    static bool isInCompatibilityMode() { return m_compatibilityMode; }
    static void setAutomaticTimeSlot(bool automatic) { m_automaticTimeSlot = automatic; }
    static bool automaticTimeSlot() { return m_automaticTimeSlot; }

private:
    static DBDecoder m_decoderInUse;
    static std::vector<std::string> m_decoderParameters;
    static unsigned long m_flockLength;
    static unsigned m_trajectoriesPerFlock;
    static double m_gridSize;
    static double m_radiusSquared;
    static double m_radius;
    static unsigned m_timeSlotSize;
    static CoordinateSystem m_coordinateSystem;
    static bool m_compatibilityMode;
    static bool m_automaticTimeSlot;
};

#endif // Config_h
