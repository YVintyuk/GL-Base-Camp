# Yuliana Vintyuk

## WBS
Task2: task number 9: System loadability chart runtime
Implement System loadability chart runtime application. Language/libraries: C/C++, STL, usage
of additional libraries or 3rd party code is prohibited. For threading should be used
platform-dependent API (unless specified otherwise by mentor). For UI might be used winAPI,
Qt etc. The application should be written with using OOP.

Application should:
* Ability to display chosen system loadability in visual mode(CPU usage, memory usage,
network usage).
* Ability to write current loadability data in text file with specific period.
* Ability to chose period of saving data into text file
* Compile for Linux and Windows.
* Non-blocking UI – it should not freeze while using the app.
* Documentation: requirements specification, software design documentation (application
modules description, UML diagrams).

### Plan (high-level), worktime - 23.03.2021 - 09.04.2024 - 14 days = 112 hours:
* Make a WBS, draw a UML diagram of classes - 12 hours
* Organize a build-system (write a make-file) - 8 hours
* Write a client to get information about system boot on Linux - 24 hours
* Create a UI chart that will visualize the system load - 16 hours
* Add the ability to record current data - 8 hours
* Add the ability to select a specific data period in a text file - 8 hours
* Organize a "non-freezing" interface - 4 hours 
* Write Unit-tests - 16 hours

### Plan (implementation details):
* Write WBS - 4 hours
* Write a UML-class diagram - 8 hours
* Check if it is possible to use UDC socket for Windows (internal socket, internal for the system) - 4 hours
* Choose a graphics library, read it and make sure its cross-platform for Windows - 4 hours

#### client (Thread 1, 24 hours):
* Get the following data from the system (8 hours):
	list of processes
	memory status
	network status
	CPU status
* Data transfer to UI-thread - 8 hours
* Ability to record current data - 2 hours
* Periodically write data to a text file - 1 hour
* Organize the "life" of data in a text file (aging) - 2 hours
* Monitor whether processes are not blocking interface - 1 hour
 
#### UI (Thread 2, 16 hours):
* Write structures that will contain data for display (design UI) - 8 hours
* Receive and process a request from a user - 1 hour
* Display data - 4 hours
* Write current data to “already” - 2 hours
* Periodicity settings - 1 hour
