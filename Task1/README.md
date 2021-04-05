# Yuliana Vintyuk

## WBS
Task1: task number 2:
client/Server application ​ to close processes​ on remote HOST.
* client gets processes list and sends them by server request.
* server will request to close one of the processes on the client's host.
* client has to reply with success or failure.

### Plan (high-level):
* Make architecture (diagram): draw a control-flow diagram (how the program will behave) and a class diagram (which components will be included in the program)
* Organize a build system (write a make-file)
* Choose, which protocol to usе (TCP)
* prototype the server in C/Python, and then write the server in C++
* write a client
* write a test for testing the program and use unit-tests

### Plan (implementation details):
* Understand how TCP works
* write a client to processes list
* write a server, that receives messages and sends "hello" in response to any message (prototype on C)
* extend the client to send a list of processes to the server
* rewrite the server with C++
* rewrite everything for Windows

#### client:
* get a list of processes
* create a socket
* send process list to server (message1 to server)
* get a request from the server (message2 from the server)
* see, which process needs to be closed
* close the process for killing
* send to the server information about the success or failure of closing the process (message3 to the server)

#### Server:
* create a socket
* get a list of processes from the client
* select the process to close
* send a request to the client to close the process
* get answer from client

## UML-diagram
```
       ┌────────┐                                                          ┌────────┐
       │ client │                                                          │ Server │
       └────┬───┘                                                          └───┬────┘
            │                                                                  │
            │                                                                  │
┌───────────┴───────────┐                                                      │
│get a list of process  │                                                      │
└───────────┬───────────┘                                                      │
            │                                                                  │
            │                                                                  │
┌───────────┴───────────────────────────┐                     ┌────────────────┴──────────────────────┐
│send a list of processes to the server │                     │get a list of processes from the client│
│                                       ├────────────────────►│                                       │
└───────────┬───────────────────────────┘                     └────────────────┬──────────────────────┘
            │                                                                  │
            │                                                 ┌────────────────┴────────────────┐
            │                                                 │select the process to the close  │
            │                                                 │                                 │
            │                                                 └────────────────┬────────────────┘
            │                                                                  │
┌───────────┴─────────────────┐                               ┌────────────────┴─────────────────────────────────┐
│get a request from the server│                               │send a request to close the process to the client │
│                             ◄───────────────────────────────┤                                                  │
└───────────┬─────────────────┘                               │                                                  │
            │                                                 └────────────────┬─────────────────────────────────┘
┌───────────┴─────────────────────────┐                                        │
│see which process need to bee closed │                                        │
└───────────┬─────────────────────────┘                                        │
            │                                                                  │
            │                                                                  │
┌───────────┴───────┐                                                          │
│close the process  │                                                          │
└───────────┬───────┘                                                          │
            │                                                                  │
            │                                                                  │
┌───────────┴───────────────────────────┐                     ┌────────────────┴──────┐
│send information to the server about   │                     │get answer from client │
│whether the process closed successfully├─────────────────────►                       │
└───────────────────────────────────────┘                     └───────────────────────┘
```
