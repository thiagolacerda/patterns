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

private:
    static DBDecoder m_decoderInUse;
    static std::vector<std::string> m_decoderParameters;
};

#endif // Config_h
