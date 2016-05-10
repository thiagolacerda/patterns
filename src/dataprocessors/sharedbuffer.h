#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template<typename T>
class SharedBuffer {
public:
    SharedBuffer()
        : m_ended(false)
    {
    }

    void reset()
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        std::queue<T>().swap(m_queue);
        m_ended = false;
    }

    void enqueue(const T& value)
    {
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_queue.push(value);
        }

        m_queueCond.notify_one();
    }

    T dequeue()
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_queueCond.wait(lock, [this] { return !m_queue.empty() || (m_ended && m_queue.empty()); });
        if (!m_queue.empty()) {
            auto value = m_queue.front();
            m_queue.pop();
            return value;
        }

        return T();
    }

    void end()
    {
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_ended = true;
        }
        m_queueCond.notify_all();
    }

    bool ended() const { return m_ended; }

private:
    bool m_ended;
    std::queue<T> m_queue;
    std::condition_variable m_queueCond;
    std::mutex m_queueMutex;
};

