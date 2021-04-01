#pragma once

#include <list>
#include "model/systemInfo.h"

/*
 systemInfo: list<systemInfo>
 savingPeriod: int
 agingCount: int

 */

class client {
private:
    std::list <systemInfo> m_systemInfo;
    int savingPeriod;
    int agingCount;
    int saveSystemInfoPeriodicaly();
    int aging();

public:
    void run();
    int transferInfoToUI();
    int saveInfoToFile();
};

