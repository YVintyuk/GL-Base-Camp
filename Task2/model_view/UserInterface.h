#pragma once

#include "model/SystemInfo.h"
#include "uiObjects.h"
#include <list>
#include <vector>
#include "Runnable.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

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
    // The function we want to execute on the new thread.
    UserInterface () {
        Fl_Window *window = new Fl_Window(300,180);
        Fl_Box *box = new Fl_Box(20,40,260,100,"Hello, World!");
        box->box(FL_UP_BOX);
        box->labelsize(36);
        box->labelfont(FL_BOLD+FL_ITALIC);
        box->labeltype(FL_SHADOW_LABEL);
        window->end();
        window->show();
        Fl::run();
    }

};
