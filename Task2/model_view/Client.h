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
        printf("Hello, I`m client\n");
        auto duration = std::chrono::duration<double, std::milli>(1000);
        std::this_thread::sleep_for(duration);
    };

public:
    int transferInfoToUI();
    int saveInfoToFile();
};

