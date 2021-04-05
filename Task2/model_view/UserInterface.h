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
    Client model_view { };
    int refreshUI(){
        static int counter = 0;
        static std::string label;
        counter = model_view.getCountProcess();
        label = std::to_string(counter);
        Fl::lock();
        box->label(label.c_str());
        Fl::unlock();
        Fl::awake();
    };
    Fl_Box *box;

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
        Fl_Window *window = new Fl_Window(300,180);
        box = new Fl_Box(20,40,260,100,"Hello, World!");
        box->box(FL_UP_BOX);
        box->labelsize(36);
        box->labelfont(FL_BOLD+FL_ITALIC);
        box->labeltype(FL_SHADOW_LABEL);
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
