#pragma once

#include <memory>

class DataModel {
public:
    DataModel()
        : m_data(nullptr)
    {
    }
    DataModel(const std::shared_ptr<void>& data)
        : m_data(data)
    {
    }

    template<class T>
    std::shared_ptr<T> get() const { return std::static_pointer_cast<T>(m_data); }
private:
    std::shared_ptr<void> m_data;
};

