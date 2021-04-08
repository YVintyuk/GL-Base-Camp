#include "Runnable.h"
#include "model/ProcessInfo.h"
#include "model/SystemInfo.h"
#include "uiObjects.h"
#include "UserInterface.h"

void Runnable::threadFunction() {
    while (!m_stop) {
        iteration();
        auto duration = std::chrono::duration<double, std::milli>(100);
        std::this_thread::sleep_for(duration);
    }
}

void Runnable::iteration() {
    auto duration = std::chrono::duration<double, std::milli>(1000);
    std::this_thread::sleep_for(duration);
}

void Runnable::run() {
    m_thread = std::thread(&Runnable::threadFunction, this);
}

void Runnable::stop() {
    m_stop = true;
    if  (m_thread.joinable()) {
        m_thread.join();
    };
}
