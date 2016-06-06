#pragma once

#include "gpspointlistener.h"

class GPSPoint;

class GPSPointBuffererListener : public GPSPointListener {
public:
    GPSPointBuffererListener(const std::unordered_map<std::string, std::string>& parameters)
        : GPSPointListener(parameters)
    {
    }

    ~GPSPointBuffererListener() override {}

    static GPSPointBuffererListener* instance(const std::unordered_map<std::string, std::string>& parameters)
    {
        return new GPSPointBuffererListener(parameters);
    }

protected:
    void onPointAdded(uint32_t id, uint64_t timeSlot) override;
    void onTimeSlotDone(uint64_t newTimeSlot) override;
    void sendDataToProcessors(const ProcessorData& data) override;
    void complete() override;

private:
    void addPresence(uint32_t tID, uint64_t n);
    void shiftPresenceMap();

    std::unordered_map<uint32_t, uint64_t> m_pointPresence;
};

