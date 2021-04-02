#include <thread>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include "model_view/client.h"

#ifndef __linux__
#include "processInfoImplWindows.h"
#include "systemInfoImplWindows.h"
#endif

client model_view.run;

// The function we want to execute on the new thread.
void UIThreadFunction(int argc, char **argv)
{
    Fl_Window *window = new Fl_Window(300,180);
    Fl_Box *box = new Fl_Box(20,40,260,100,"Hello, World!");
    box->box(FL_UP_BOX);
    box->labelsize(36);
    box->labelfont(FL_BOLD+FL_ITALIC);
    box->labeltype(FL_SHADOW_LABEL);
    window->end();
    window->show(argc, argv);
    Fl::run();
}

int main(int argc, char **argv) {
// Constructs the new thread and runs it. Does not block execution.
    std::thread UIThread(UIThreadFunction, argc, argv);

//TODO start data gathering thread (client)

// Makes the main thread wait for the new thread to finish execution, therefore blocks its own execution.
    UIThread.join();
}
