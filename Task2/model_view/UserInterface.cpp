#include "UserInterface.h"
#include "model/ProcessInfo.h"
#include "model/SystemInfo.h"
#include "Runnable.h"
#include "uiObjects.h"


int UserInterface::refreshUI() {
    int counter = 0;
    static std::string processCountLabel;
    counter = model_view.getCountProcess();
    freeMemoryLabel = std::to_string(model_view.getFreeMemory());
    processCountLabel = std::to_string(counter);
    Fl::lock();
    processCountBox->label(processCountLabel.c_str());
    freeMemoryBox->label(freeMemoryLabel.c_str());
    Fl::unlock();
    Fl::awake();
}

void UserInterface::onDemandSaveButton_onClick(Fl_Widget *w, void *userInterface) {
    auto ui = static_cast <UserInterface*> (userInterface);
    ui->model_view.saveLastSystemInfo(ui->saveToFile_filename);
}

void UserInterface::iteration() {
    refreshUI();
    auto duration = std::chrono::duration<double, std::milli>(1000);
    std::this_thread::sleep_for(duration);
}

UserInterface::UserInterface() {
    Fl::lock();
    Fl_Window *window = new Fl_Window(600,360);
    processCountBox = new Fl_Box(20,40,260,100,"Hello");
    processCountBox->box(FL_UP_BOX);
    processCountBox->labelsize(36);
    processCountBox->labelfont(FL_BOLD+FL_ITALIC);
    processCountBox->labeltype(FL_SHADOW_LABEL);

    Fl_Box *processCountBoxDescription = new Fl_Box(20,140,260,100,"Running processes");
    processCountBoxDescription->box(FL_UP_BOX);
    processCountBoxDescription->labelsize(14);
    processCountBoxDescription->labelfont(FL_BOLD+FL_ITALIC);
    processCountBoxDescription->labeltype(FL_SHADOW_LABEL);

    Fl_Box *freeMemoryBoxDescription = new Fl_Box(300,140,260,100,"Free memory (bytes)");
    freeMemoryBoxDescription->box(FL_UP_BOX);
    freeMemoryBoxDescription->labelsize(14);
    freeMemoryBoxDescription->labelfont(FL_BOLD+FL_ITALIC);
    freeMemoryBoxDescription->labeltype(FL_SHADOW_LABEL);

    freeMemoryBox = new Fl_Box(300,40,260,100,"Hello");
    freeMemoryBox->box(FL_UP_BOX);
    freeMemoryBox->labelsize(36);
    freeMemoryBox->labelfont(FL_BOLD+FL_ITALIC);
    freeMemoryBox->labeltype(FL_SHADOW_LABEL);

    Fl_Button* onDemandSaveButton = new Fl_Button (340,250,160,50,"Save to file");
    onDemandSaveButton->box(FL_UP_BOX);
    onDemandSaveButton->labelsize(14);
    onDemandSaveButton->labelfont(FL_BOLD+FL_ITALIC);
    onDemandSaveButton->labeltype(FL_SHADOW_LABEL);
    onDemandSaveButton->callback((Fl_Callback*)&UserInterface::onDemandSaveButton_onClick, this);

    window->end();
    window->show();
    model_view.run();
    run();
    Fl::run();
    stop ();
}

UserInterface::~UserInterface() {
    model_view.stop();
    stop();
}
