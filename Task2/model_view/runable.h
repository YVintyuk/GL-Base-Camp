#pragma once

#include <thread>
#include <chrono>


class runable {
private:
    std::thread m_thread;
    bool m_stop = false;
    void threadFunction () {
        while (!m_stop) {
            iteration();
            std::this_thread::sleep_for(100ms);
        }
    };

protected:
   virtual void iteration() = 0;

public:
    void run () {
        m_thread = std::thread(&runable::threadFunction, this);
    };
    void stop ()  {
        m_stop = true;
        m_thread.join();
    };
};
