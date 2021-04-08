#pragma once

#include <list>
#include <mutex>
#include <fstream>
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
    void saveInfoToFile(const std::string &fileName, const SystemInfo& systemInfo);

protected:
    virtual void iteration();

public:
    int transferInfoToUI();
    void saveLastSystemInfo(const std::string &fileName);
    size_t getCountProcess();
    size_t getFreeMemory();
};

