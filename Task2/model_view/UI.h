#pragma once

#include "model/systemInfo.h"
#include "uiObjects.h"
#include <list>
#include <vector>
#include "runnable.h"

class ui : public runnable {
private:
    std::list <systemInfo> systemInfoList;
    std::vector <uiObjects> uiObjectsVector;
    int saveDataNow();
    int changeSavingPeriod(int period);
    int prepareUI();
    int reareshUI();

public:
    void run();
};
