# LibFd

LibFd is a C++ library that wraps some C functions and allow you to do
cool stuff with it. You can for example launch asynchronous functions,
monitor fds, create fds and trigger it manually and many other things.

- [Installing](#installing)
- [FdMonitor](#FdMonitor)
- [Pipe](#Pipe)
- [StateFd](#StateFd)
- [TimerFd](#TimerFd)
- [Using elements together](#using-elements-together)
- [License](#license)


## Installing

This paragraph contains all instructions needed to install the library into your project
using a Makefile or CMakeLists.

### Installing repo into your project

```bash
$ mkdir lib
$ cd lib
$ git clone https://github.com/aurelien-boch/LibFd.git
```

### Writing CMakeLists

```cmake
cmake_minimum_required (VERSION 3.17)
project(example)

add_executable(your-target source.cpp)
```

### Writing Makefile

```makefile
CPPFLAGS    =   -Wall -Wextra

CXX         =   g++
RM          =   rm -rf

SRC         =   source.cpp

OBJ         =   $(SRC:.c=.o)
NAME        =   binary_name

all: $(OBJ)
    $(CXX) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
    $(RM) $(OBJ)

fclean: clean
    $(RM) $(NAME)
```
### Linking with CMakeLists

Just add theses two lines

```cmake
add_subdirectory(lib/LibFd)
target_link_libraries(your-target libfd)
```

### Linking with Makefile

Just add the library control into your rules and link with ldflags

```makefile
LDFLAGS += lib/LibFd/libfd.a

all: $(OBJ)
    make -C lib/libFd
    ...

clean:
    make -C lib/libFd clean
    ...

fclean: clean
    make -C lib/libFd fclean
    ...
```

## FdMonitor

### Description
The FdMonitor class allows you to monitor fds for multiple events.
You can then bind data and/or functions to events on fds.

To access events list and description:
```bash
$ man 2 epoll_ctl
```

### Some examples

Triggering a function when stdin is ready for reading:
```c++
#include "lib/libFd/include/FdMonitor.hpp"

static void fdTriggered(int fd)
{
    //do stuff with fd
}

int main()
{
    fdlib::FdMonitor monitor(0);
    std::vector<std::any> vals;
    
    monitor.addMonitorFd(
        0, //target fd
        EPOLLIN, //event
        std::function([](int fd) { fdTriggered(fd); })//data or function
    );
    
    while (cond)
        monitor.waitEvents(vals, 100, -1);
}
```

Filling a vector with data when stdin is ready for reading:
```c++
#include <iostream>
#include "lib/libFd/include/FdMonitor.hpp"

int main()
{
    fdlib::FdMonitor monitor(0);
    std::vector<std::any> vals;
    
    monitor.addMonitorFd(
        0, //target fd
        EPOLLIN, //event
        std::string("hello, world")
    );
    
    while (cond) {
        monitor.waitEvents(vals, 100, -1);
        auto data = std::any_cast<std::string>(vals[0]); //do stuff with data
        std::cout << "Got: " << data << std::endl;
    }
}
```

## Pipe

### Description
The Pipe class is just a wrapper of the C pipe function. It allows you to
create a pipe, read and write to it. It also gives you an access to the fd
if you want to use it with the FdMonitor class.

### Some examples
Writing and reading data in and out of a pipe:
```c++
#include <iostream>
#include "lib/libFd/include/Pipe.hpp"

int main()
{
    fdlib::Pipe p;

    p.writeData(15);
    std::cout << "Read: " << p.readData<int>() << std::endl;
}
```

## StateFd

### Description
The StateFd class is a class that allows you to create a fd and toggle its state
(triggered / non triggered) by calling methods. It can be useful when used with
the FdMonitor class to force the triggering.

### Some examples

Triggering a StateFd in a thread and wait it in another:
```c++
#include <thread>
#include <chrono>
#include <iostream>
#include "lib/libFd/include/StateFd.hpp"

static void triggerFdAfterDelay(const fdlib::StateFd &s, const std::uint64_t sec)
{
    std::this_thread::sleep_for (std::chrono::seconds(sec));
    s.trigger();
}

int main()
{
    fdlib::StateFd s;
    std::thread t([&s]() { triggerFdAfterDelay(s, 2); });

    std::cout << "Waiting" << std::endl;
    s.wait();
    std::cout << "Fd triggered" << std::endl;
    t.join();
}
```

## TimerFd

### Description
The TimerFd class is just a wrapper of the C timerfd function. It allows you to
create fd that will be triggered after a given delay and re-triggered with a given rate.

### Some examples

Creating a fd that triggers every second;
```c++
#include "lib/libFd/include/TimerFd.hpp"

int main()
{
    fdlib::TimerFd timer(CLOCK_MONOTONIC, 0);

    timer.setSecDelay(1);
    timer.setSecRepeatRate(1);
    timer.startTimer();
}
```

## Using elements together

Program that waits on stdin and triggers functions ad different delays simultaneously
using FdMonitor and TimerFd classes
```c++
#include <iostream>
#include "lib/libFd/include/FdMonitor.hpp"
#include "lib/libFd/include/TimerFd.hpp"

static void stdinHandler()
{
    std::string data;

    std::cin >> data;
    std::cout << "Stdin triggered, Data: " << data << std::endl;
}

static void firstLoop(const fdlib::TimerFd &timer)
{
    timer.clearTimer();
    std::cout << "First loop triggered" << std::endl;
}

static void secondLoop(const fdlib::TimerFd &timer)
{
    timer.clearTimer();
    std::cout << "Second loop triggered" << std::endl;
}

static void delayedTrigger(const fdlib::TimerFd &timer)
{
    timer.clearTimer();
    std::cout << "Delayed trigger has been triggered" << std::endl;
}

static void prepareTimers(
    fdlib::TimerFd &firstLoopTimer,
    fdlib::TimerFd &secondLoopTimer,
    fdlib::TimerFd &delayedTriggerTimer
) {
    firstLoopTimer.setSecDelay(1);
    firstLoopTimer.setSecRepeatRate(3);
    secondLoopTimer.setSecDelay(1);
    secondLoopTimer.setSecRepeatRate(2);
    delayedTriggerTimer.setSecDelay(1);
    firstLoopTimer.startTimer();
    secondLoopTimer.startTimer();
    delayedTriggerTimer.startTimer();
}

static void bindToMonitor(
    fdlib::FdMonitor &monitor,
    fdlib::TimerFd &firstLoopTimer,
    fdlib::TimerFd &secondLoopTimer,
    fdlib::TimerFd &delayedTriggerTimer
) {
    monitor.addMonitorFd(
        0,
        EPOLLIN,
        std::function([](int) { stdinHandler(); })
    );
    monitor.addMonitorFd(
        firstLoopTimer.getFd(),
        EPOLLIN,
        std::function([&firstLoopTimer](int) { firstLoop(firstLoopTimer); })
    );
    monitor.addMonitorFd(
        secondLoopTimer.getFd(),
        EPOLLIN,
        std::function([&secondLoopTimer](int) { secondLoop(secondLoopTimer); })
    );
    monitor.addMonitorFd(
        delayedTriggerTimer.getFd(),
        EPOLLIN,
        std::function([&delayedTriggerTimer](int) { delayedTrigger(delayedTriggerTimer); })
    );
}

int main()
{
    fdlib::FdMonitor monitor(0);
    fdlib::TimerFd firstLoopTimer(CLOCK_MONOTONIC, 0);
    fdlib::TimerFd secondLoopTimer(CLOCK_MONOTONIC, 0);
    fdlib::TimerFd delayedTriggerTimer(CLOCK_MONOTONIC, 0);
    std::vector<std::any> vals;

    prepareTimers(firstLoopTimer, secondLoopTimer, delayedTriggerTimer);
    bindToMonitor(monitor, firstLoopTimer, secondLoopTimer, delayedTriggerTimer);
    std::cout << "STARTING..." << std::endl;
    while (cond)
        monitor.waitEvents(vals, 100, -1);
}
```
## License

This code is open source software licensed under the Apache 2.0 License.
