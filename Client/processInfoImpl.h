#pragma once

#ifdef __linux__
#include "processInfoCommon.h"
#else
#include "../Common/processInfoCommon.h"
#endif

std::vector <processInfo_t> getProcessInfoVector();

int killProcess(size_t pid);