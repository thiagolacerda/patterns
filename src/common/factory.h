#ifndef FACTORY_H
#define FACTORY_H

#include "config.h"

class DatabaseDecoder;

class Factory {
public:
    static DatabaseDecoder* dbDecoderInstance(Config::DBDecoder);
};

#endif  // FACTORY_H
