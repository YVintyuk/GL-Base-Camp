#include "Client.h"
#include "model/ProcessInfo.h"
#include "model/SystemInfo.h"
#include "uiObjects.h"
#include "Runnable.h"
#include "UserInterface.h"
#include <list>
#include <cstddef>
#include <model/LinuxImpl/SystemInfoImplLinux.h>


void Client::saveInfoToFile(const std::string &fileName, const SystemInfo &systemInfo) {
    std::ofstream output;
    output.open(fileName.c_str());
    output << systemInfo;
}

void Client::saveLastSystemInfo(const std::string &fileName) {
    printf("Saving system info to file, %s\n", fileName.c_str());
    std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
    auto lastSystemInfo = m_systemInfo.rbegin();
    if (lastSystemInfo != m_systemInfo.rend()) {
        saveInfoToFile(fileName, **lastSystemInfo);
    }
}

size_t Client::getCountProcess() {
    std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
    auto lastSystemInfo = m_systemInfo.rbegin();
    if (lastSystemInfo != m_systemInfo.rend()) {
        return (*lastSystemInfo)->getProcessInfo().size();
    } else {
        return 0;
    }
}

void Client::iteration() {
    {
        std::lock_guard<std::mutex> clientMutexGuard(clientMutex);
        if (m_systemInfo.size() == m_agingCount) {
            printf("Removed old system info from q\n");
            m_systemInfo.pop_front();
        }

        std::shared_ptr<SystemInfo> currentStatus =
#ifdef __linux__
                std::make_shared<SystemInfoImplLinux>();
#else
#endif
        currentStatus->init();
        if (m_systemInfo.size() < 2) {
            m_systemInfo.push_back(currentStatus);
        } else {
            updateSystemInfoQeue(currentStatus);
        }
    }
    saveSystemInfoPeriodically();
    aging();

    printf("Hello, I`m client\n");
    auto duration = std::chrono::duration<double, std::milli>(1000);
    std::this_thread::sleep_for(duration);
}

void Client::updateSystemInfoQeue(std::shared_ptr<SystemInfo> currentStatus) {
    auto lastSystemInfoTime = (*m_systemInfo.rbegin())->start;
    auto preLastSystemInfoTime = (*(++m_systemInfo.rbegin()))->start;
    auto timeDiff = lastSystemInfoTime - preLastSystemInfoTime;
    if (m_systemInfo.size() < m_agingCount && timeDiff > m_savingResolution) {
        printf("Adding another system info to queue\n");
        m_systemInfo.push_back(currentStatus);
    } else {
        auto last = m_systemInfo.rbegin();
        *last = currentStatus;
    }
}

size_t Client::getFreeMemory() {
    std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
    auto lastSystemInfo = m_systemInfo.rbegin();
    if (lastSystemInfo != m_systemInfo.rend()) {
        return (*lastSystemInfo)->getFreeMemory();
    } else {
        return 0;
    }
}

void Client::saveSystemInfoPeriodically() {
    static auto lastSave = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::now();
    if ((now - lastSave) > m_savingPeriod) {
        std::string fileName = "SystemInfoPeriodically";
        auto timeStamp = now.time_since_epoch().count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
        fileName += "_" + std::to_string(timeStamp) + ".log";
        saveLastSystemInfo(fileName);
        m_listOfsavedFiles.push_back(fileName);
        lastSave = now;
    }
}

int Client::aging() {
    if (m_listOfsavedFiles.size() > 3) {
        auto fileName = m_listOfsavedFiles.begin();
        printf("Remove old files %s\n", fileName->c_str());
        remove(fileName->c_str());
        m_listOfsavedFiles.pop_front();
    }
}

Client::~Client() {
    for (auto fileName : m_listOfsavedFiles) {
        remove(fileName.c_str());
    }
}
