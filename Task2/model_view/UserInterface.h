#pragma once

#include "model/SystemInfo.h"
#include "uiObjects.h"
#include <list>
#include <vector>
#include "Runnable.h"

class UserInterface : public Runnable {
private:
    std::list <SystemInfo> systemInfoList;
    std::vector <uiObjects> uiObjectsVector;
    int saveDataNow();
    int changeSavingPeriod(int period);
    int prepareUI();
    int reareshUI();

public:
    void run();
};
