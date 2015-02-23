#ifndef Factory_h
#define Factory_h

#include <memory>
#include <mutex>

class DatabaseDecoder;

class Factory {
public:
    static DatabaseDecoder* dbDecoderInstance();

private:
    static std::unique_ptr<DatabaseDecoder> m_dbDecoderInstance;
    static std::once_flag m_dbDecoderOnceFlag;
};

#endif // Factory_h
