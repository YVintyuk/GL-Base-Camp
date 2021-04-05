cmake_minimum_required(VERSION 3.12)
project(yuliana_vintyuk_task2)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

link_libraries(
        fltk
        pthread
)

add_executable(task2 main.cpp model_view/Client.cpp model_view/Runnable.cpp
        model_view/UserInterface.cpp model_view/uiObjects.cpp
        model/ProcessInfo.cpp model/SystemInfo.cpp
        model/LinuxImpl/ProcessInfoImplLinux.cpp model/LinuxImpl/SystemInfoImplLinux.cpp)
