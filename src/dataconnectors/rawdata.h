#pragma once

class RawData {
public:
    virtual int getAsInt(int colIndex) const = 0;
    virtual std::string getAsString(int colIndex) const = 0;
    virtual double getAsDouble(int colIndex) const = 0;
    virtual uint32_t getAsUInt32_t(int colIndex) const = 0;
    virtual uint64_t getAsUInt64_t(int colIndex) const = 0;

    virtual ~RawData() {}
};

