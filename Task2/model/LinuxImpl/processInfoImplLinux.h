#pragma once

#ifdef __linux__
#include "processInfoCommon.h"
#else
#include "../Common/processInfoCommon.h"
#endif

#define MAX_PROCESS_COUNT 4096

std::vector <processInfo_t> getProcessInfoVector();

int killProcess(PID_TYPE pid);

