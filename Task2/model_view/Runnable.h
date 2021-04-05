#pragma once

#include <thread>
#include <chrono>


class Runnable {
private:
    std::thread m_thread;
    bool m_stop = false;
    void threadFunction () {
        while (!m_stop) {
            iteration();
            auto duration = std::chrono::duration<double, std::milli>(100);
            std::this_thread::sleep_for(duration);
        }
    };

protected:
    virtual void iteration() {
        auto duration = std::chrono::duration<double, std::milli>(1000);
        std::this_thread::sleep_for(duration);
    };

public:
    void run () {
        m_thread = std::thread(&Runnable::threadFunction, this);
    };
    void stop ()  {
        m_stop = true;
        m_thread.join();
    };
};
