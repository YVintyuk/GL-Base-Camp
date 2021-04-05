#include <thread>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <model_view/UserInterface.h>
#include "model_view/Client.h"

#ifndef __linux__
#include "processInfoImplWindows.h"
#include "systemInfoImplWindows.h"
#endif

int main(int argc, char **argv) {
    Client model_view { };
    model_view.run();
    UserInterface ui { };

//TODO start data gathering thread (client)

    model_view.stop();
}
