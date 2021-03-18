#pragma once

#ifdef __linux__
#include "processInfoCommon.h"
#define PID_TYPE size_t
#else
#include "../Common/processInfoCommon.h"
#define PID_TYPE DWORD
#endif

#define MAX_PROCESS_COUNT 4096

std::vector <processInfo_t> getProcessInfoVector();

int killProcess(size_t pid);