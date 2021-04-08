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
    if (!m_systemInfo.empty()) {
        m_systemInfo.pop_front();
    }
    SystemInfo currentStatus { };
    m_systemInfo.push_back(currentStatus);
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
