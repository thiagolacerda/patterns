#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <string>
#include <vector>

class Config {
public:
    enum CoordinateSystem {
        WSG84,
        Metric
    };

    enum DBDecoder {
        BerlinMOD,
        BrinkhoffMNTG,
        LifeMap,
        SortedFile,
        TDrive,
        Trucks,
        TrucksPaper
    };

    static void reset();
    static void setDecoderParameters(const std::vector<std::string>& parameters) { m_decoderParameters = parameters; }
    static std::vector<std::string>& decoderParameters() { return m_decoderParameters; }
    static void setDecoder(char const* decoderName);
    static DBDecoder decoder() { return m_decoderInUse; }
    static void setGridSize(double size);
    static double gridSize() { return m_gridSize; }
    static double radiusSquared() { return m_radiusSquared; }
    static double radius() { return m_radius; }
    static uint32_t flockLength() { return m_flockLength; }
    static void setFlockLength(uint32_t flockLength) { m_flockLength = flockLength; }
    static uint32_t numberOfTrajectoriesPerFlock() { return m_trajectoriesPerFlock; }
    static void setNumberOfTrajectoriesPerFlock(uint32_t trajectories) { m_trajectoriesPerFlock = trajectories; }
    static void setTimeSlotSize(double size) { m_timeSlotSize = size; }
    static double timeSlotSize() { return m_timeSlotSize; }
    static void setCoordinateSystem(CoordinateSystem coordSystem);
    static CoordinateSystem coordinateSystem() { return m_coordinateSystem; }
    static std::string coordinateSystemName(CoordinateSystem);
    static void setCompatibilityMode(bool compatibility) { m_compatibilityMode = compatibility; }
    static bool isInCompatibilityMode() { return m_compatibilityMode; }
    static void setInterpolate(bool interpolate) { m_interpolate = interpolate; }
    static bool interpolate() { return m_interpolate; }
    static void setOutlierSpeedCutOff(double outlierCutOff) { m_outlierSpeedCutOff = outlierCutOff; }
    static double outlierSpeedCutOff() { return m_outlierSpeedCutOff; }
    static void setFlushFlocksToFile(bool flushToFile) { m_flushFlocksToFile = flushToFile; }
    static bool flushFlocksToFile() { return m_flushFlocksToFile; }
    static void setOnlineProcessing(bool online) { m_onlineProcessing = online; }
    static bool onlineProcessing() { return m_onlineProcessing; }
    static void setBuffering(bool buf);
    static bool buffering() { return m_buffering; }
    static void setReportPerformance(bool report) { m_reportPerformance = report; }
    static bool reportPerformance() { return m_reportPerformance; }

private:
    static DBDecoder m_decoderInUse;
    static std::vector<std::string> m_decoderParameters;
    static uint32_t m_flockLength;
    static uint32_t m_trajectoriesPerFlock;
    static double m_gridSize;
    static double m_radiusSquared;
    static double m_radius;
    static uint32_t m_timeSlotSize;
    static CoordinateSystem m_coordinateSystem;
    static bool m_compatibilityMode;
    static bool m_interpolate;
    static double m_outlierSpeedCutOff;
    static bool m_flushFlocksToFile;
    static bool m_onlineProcessing;
    static bool m_buffering;
    static bool m_reportPerformance;
};

#endif  // CONFIG_H
