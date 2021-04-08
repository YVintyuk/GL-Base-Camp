#pragma once

#include <thread>
#include <chrono>


class Runnable {
private:
    std::thread m_thread;
    bool m_stop = false;
    void threadFunction ();

protected:
    virtual void iteration();

public:
    void run ();
    void stop ();
};
