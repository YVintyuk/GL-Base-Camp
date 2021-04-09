#pragma once

#include <vector>
#include "model/SystemInfo.h"
#include "model/ProcessInfo.h"

class SystemInfoImplLinux : public SystemInfo {
protected:
    size_t readFreeMemoryFromSystem() override;
    std::vector<std::shared_ptr<ProcessInfo>> readProcessListFromSystem() override;
};

