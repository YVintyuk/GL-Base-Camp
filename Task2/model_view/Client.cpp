#include "Client.h"
#include "model/ProcessInfo.h"
#include "model/SystemInfo.h"
#include "uiObjects.h"
#include "Runnable.h"
#include "UserInterface.h"
#include <list>
#include <cstddef>

/*
 systemInfo: list<systemInfo>
 savingPeriod: int
 agingCount: int

 run(): void
 saveSystemInfoPeriodicaly(): int
 transferInfoToUI(): int
 saveInfoToFile(): int
 aging(): int
 */

