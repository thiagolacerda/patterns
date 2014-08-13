#ifndef GPSTupleListener_h
#define GPSTupleListener_h

#include <tuple>

class GPSTupleListener {
public:
    virtual void processGPSTuple(const std::tuple<long, double, double, unsigned long>&) = 0;
};

#endif // GPSTupleListener_h
