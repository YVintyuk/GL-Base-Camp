#pragma once

#include <thread>


class runable {
private:
    std::thread m_thread;
    bool m_stop = false;

public:
    virtual void run () = 0;
    void stop ();
};
