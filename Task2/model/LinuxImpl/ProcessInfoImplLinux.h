#pragma once

#include "model/ProcessInfo.h"

class ProcessInfoImplLinux : public ProcessInfo {
public:
    explicit ProcessInfoImplLinux(pid_t pid) : ProcessInfo(pid) {}
};