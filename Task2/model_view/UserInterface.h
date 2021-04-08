#pragma once

#include "model/SystemInfo.h"
#include "uiObjects.h"
#include <list>
#include <vector>
#include "Runnable.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Button.H>
#include "Client.h"

class UserInterface : public Runnable {
private:
    std::string saveToFile_filename = "onDemandSystemInfo.log";
    std::list <SystemInfo> systemInfoList;
    std::vector <uiObjects> uiObjectsVector;
    int changeSavingPeriod(int period);
    int prepareUI();
    std::string freeMemoryLabel;
    static void onDemandSaveButton_onClick (Fl_Widget* w, void* userInterface);
    Client model_view { };
    int refreshUI();
    Fl_Box *processCountBox;
    Fl_Box *freeMemoryBox;

protected:
    virtual void iteration();

public:
    // The function we want to execute on the new thread.
    UserInterface ();

    ~UserInterface();
};
