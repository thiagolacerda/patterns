#ifndef GPSTupleListener_h
#define GPSTupleListener_h

#include <tuple>

class GPSTupleListener {
public:
    virtual void processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>&) = 0;

    virtual ~GPSTupleListener() { }
};

#endif // GPSTupleListener_h
