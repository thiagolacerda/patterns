#pragma once

#include <memory>
#include <vector>

class DataHandler {
public:
    DataHandler()
        : m_done(false)
    {
    }

    virtual void onDataProviderFinished() = 0;

    bool done() const { return m_done; }

    void endDataHandler()
    {
        for (auto handler : m_handlers)
            handler->onDataProviderFinished();

        m_done = true;
    }

protected:
    bool m_done;
    std::vector<std::shared_ptr<DataHandler>> m_handlers;
};

