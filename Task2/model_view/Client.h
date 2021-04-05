#pragma once

#include <list>
#include "model/SystemInfo.h"
#include "Runnable.h"

class Client : public Runnable {
private:
    std::list <SystemInfo> m_systemInfo;
    int savingPeriod;
    int agingCount;
    int saveSystemInfoPeriodicaly();
    int aging();


protected:
    virtual void iteration() {
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
        auto lastSystemInfo = m_systemInfo.rbegin();
        if (lastSystemInfo != m_systemInfo.rend()) {
            return lastSystemInfo->getProcessInfo().size();
        } else {
            return 0;
        }
    }
};

