#ifndef Config_h
#define Config_h

#include <string>
#include <vector>

class Config {
public:
    enum DBDecoder {
        LifeMap
    };

    static void setDecoderParameters(const std::vector<std::string>& parameters) { m_decoderParameters = parameters; }
    static std::vector<std::string>& decoderParameters() { return m_decoderParameters; }
    static void setDecoder(char*);
    static DBDecoder decoder() { return m_decoderInUse; }
    static void setGridSize(double size) { m_gridSize = size; }
    static double gridSize() { return m_gridSize; }
    static void setTimeSlotSize(double size) { m_timeSlotSize = size; }
    static double timeSlotSize() { return m_timeSlotSize; }

private:
    static DBDecoder m_decoderInUse;
    static std::vector<std::string> m_decoderParameters;
    static double m_gridSize;
    static unsigned m_timeSlotSize;
};

#endif // Config_h
