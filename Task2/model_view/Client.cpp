#include "Client.h"
#include "model/ProcessInfo.h"
#include "model/SystemInfo.h"
#include "uiObjects.h"
#include "Runnable.h"
#include "UserInterface.h"
#include <list>
#include <cstddef>


void Client::saveInfoToFile(const std::string &fileName, const SystemInfo &systemInfo) {
    std::ofstream output;
    output.open(fileName.c_str());
    output << systemInfo;
}

void Client::saveLastSystemInfo(const std::string &fileName) {
    std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
    auto lastSystemInfo = m_systemInfo.rbegin();
    if (lastSystemInfo != m_systemInfo.rend()) {
        saveInfoToFile(fileName, *lastSystemInfo);
    }
}

size_t Client::getCountProcess() {
    std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
    auto lastSystemInfo = m_systemInfo.rbegin();
    if (lastSystemInfo != m_systemInfo.rend()) {
        return lastSystemInfo->getProcessInfo().size();
    } else {
        return 0;
    }
}

void Client::iteration() {
    std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
    if (m_systemInfo.size() == m_agingCount) {
        printf("Removed old system info from q\n");
        m_systemInfo.pop_front();
    }

    SystemInfo currentStatus { };
    if (m_systemInfo.size() < 2) {
        m_systemInfo.push_back(currentStatus);
    } else {
        auto lastSystemInfoTime = m_systemInfo.rbegin()->start;
        auto preLastSystemInfoTime = (++m_systemInfo.rbegin())->start;
        auto timeDiff = lastSystemInfoTime - preLastSystemInfoTime;
        if (m_systemInfo.size() < m_agingCount && timeDiff > m_savingResolution) {
            printf("Adding another system info to queue\n");
            m_systemInfo.push_back(currentStatus);
        } else {
            auto last = m_systemInfo.rbegin();
            *last = currentStatus;
        }
    }

    printf("Hello, I`m client\n");
    auto duration = std::chrono::duration<double, std::milli>(1000);
    std::this_thread::sleep_for(duration);
}

size_t Client::getFreeMemory() {
    std::lock_guard<std::mutex> clientMutexGuard (clientMutex);
    auto lastSystemInfo = m_systemInfo.rbegin();
    if (lastSystemInfo != m_systemInfo.rend()) {
        return lastSystemInfo->getFreeMemory();
    } else {
        return 0;
    }
}
