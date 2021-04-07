#pragma once

#include <list>
#include <mutex>
#include "model/SystemInfo.h"
#include "Runnable.h"

class Client : public Runnable {
private:
    std::list <SystemInfo> m_systemInfo;
    int savingPeriod;
    int agingCount;
    int saveSystemInfoPeriodicaly();
    int aging();
    std::mutex clientMutex;

protected:
    virtual void iteration() {
        std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
        if (!m_systemInfo.empty()) {
            m_systemInfo.pop_front();
        }
        SystemInfo currentStatus { };
        m_systemInfo.push_back(currentStatus);
        printf("Hello, I`m client\n");
        auto duration = std::chrono::duration<double, std::milli>(1000);
        std::this_thread::sleep_for(duration);
    }

public:
    int transferInfoToUI();
    int saveInfoToFile();
    int getCountProcess() {
        std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
        auto lastSystemInfo = m_systemInfo.rbegin();
        if (lastSystemInfo != m_systemInfo.rend()) {
            return lastSystemInfo->getProcessInfo().size();
        } else {
            return 0;
        }
    }
    size_t getFreeMemory() {
        std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
        auto lastSystemInfo = m_systemInfo.rbegin();
        if (lastSystemInfo != m_systemInfo.rend()) {
            return lastSystemInfo->getFreeMemory();
        } else {
            return 0;
        }
    }
};

