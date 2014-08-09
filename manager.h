#ifndef Manager_h
#define Manager_h

class DatabaseDecoder;

class Manager {
public:
    Manager()
        : m_dbDecoder(nullptr)
    { }

    void start();

private:
    void retrieveData();
    DatabaseDecoder* m_dbDecoder;
};

#endif // Manager_h
