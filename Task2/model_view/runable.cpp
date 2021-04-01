#include "runable.h"
#include "model/processInfo.h"
#include "model/systemInfo.h"
#include "uiObjects.h"
#include "UI.h"

void runable::stop() {
    m_stop = true;
    m_thread.join();
}