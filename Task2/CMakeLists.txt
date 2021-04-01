cmake_minimum_required(VERSION 3.12)
project(yuliana_vintyuk_task2)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

link_libraries(
        fltk
        pthread
)

add_executable(task2 main.cpp model_view/client.cpp  model_view/runable.cpp
        model_view/UI.cpp  model_view/uiObjects.cpp
        model/processInfo.cpp  model/systemInfo.cpp
        model/LinuxImpl/processInfoImplLinux.cpp model/LinuxImpl/systemInfoImplLinux.cpp)
