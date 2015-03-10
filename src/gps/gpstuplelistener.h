#ifndef GPSTUPLELISTENER_H
#define GPSTUPLELISTENER_H

#include <cstdint>
#include <tuple>

class GPSTupleListener {
public:
    virtual void processGPSTuple(const std::tuple<uint32_t, double, double, uint64_t>&) = 0;

    virtual ~GPSTupleListener() { }
};

#endif  // GPSTUPLELISTENER_H
