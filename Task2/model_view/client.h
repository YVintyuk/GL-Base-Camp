#pragma once

#include <list>
#include "model/systemInfo.h"
#include "runable.h"

class client : public runable {
private:
    std::list <systemInfo> m_systemInfo;
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

