#pragma once

#include <list>
#include <mutex>
#include <fstream>
#include "model/SystemInfo.h"
#include "Runnable.h"

class Client : public Runnable {
private:
    std::list <SystemInfo> m_systemInfo;
    const std::chrono::minutes m_savingPeriod = std::chrono::minutes(1);
    const std::chrono::seconds m_savingResolution = std::chrono::seconds(30);
    const unsigned int m_agingCount = 10;
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

