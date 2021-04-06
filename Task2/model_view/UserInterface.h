#pragma once

#include "model/SystemInfo.h"
#include "uiObjects.h"
#include <list>
#include <vector>
#include "Runnable.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include "Client.h"

class UserInterface : public Runnable {
private:
    std::list <SystemInfo> systemInfoList;
    std::vector <uiObjects> uiObjectsVector;
    int saveDataNow();
    int changeSavingPeriod(int period);
    int prepareUI();
    std::string freeMemoryLabel;
    Client model_view { };
    int refreshUI(){
        int counter = 0;
        static std::string processCountLabel;
        counter = model_view.getCountProcess();
        freeMemoryLabel = std::to_string(model_view.getFreeMemory());
        processCountLabel = std::to_string(counter);
        Fl::lock();
        box->label(processCountLabel.c_str());
        freeMemoryBox->label(freeMemoryLabel.c_str());
        Fl::unlock();
        Fl::awake();
    };
    Fl_Box *box;
    Fl_Box *freeMemoryBox;

protected:
    virtual void iteration() {
        refreshUI();
        auto duration = std::chrono::duration<double, std::milli>(1000);
        std::this_thread::sleep_for(duration);
    };

public:
    // The function we want to execute on the new thread.
    UserInterface () {
        Fl::lock();
        Fl_Window *window = new Fl_Window(600,180);
        box = new Fl_Box(20,40,260,100,"Hello, World!");
        box->box(FL_UP_BOX);
        box->labelsize(36);
        box->labelfont(FL_BOLD+FL_ITALIC);
        box->labeltype(FL_SHADOW_LABEL);

        freeMemoryBox = new Fl_Box(300,40,260,100,"Hello, World:)!");
        freeMemoryBox->box(FL_UP_BOX);
        freeMemoryBox->labelsize(36);
        freeMemoryBox->labelfont(FL_BOLD+FL_ITALIC);
        freeMemoryBox->labeltype(FL_SHADOW_LABEL);
        window->end();
        window->show();
        model_view.run();
        run();
        Fl::run();
        stop ();
    }

    ~UserInterface() {
        model_view.stop();
        stop();
    }
};
