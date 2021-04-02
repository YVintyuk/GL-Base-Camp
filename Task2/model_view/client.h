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
        std::this_thread::sleep_for(2000ms);
        printf("Hello, I`m client");
    };

public:
    int transferInfoToUI();
    int saveInfoToFile();
};

