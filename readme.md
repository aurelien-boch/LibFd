# LibFd

LibFd is a C++ library that wraps some C functions and allow you to do
cool stuff with it. You can for example launch asynchronous functions,
monitor fd, create fd and trigger it manually and many other things.

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
    $(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

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

The FdMonitor class allows you to monitor fds for multiple events.
You can then bind data and or functions to events on fds

To access events list and description:
```bash
$ man 2 epoll_ctl
```

### Some examples:

Trigger function when stdin is ready for reading:
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
        std::function([](int fd) { fdTriggered(fd) })//data or function
    );
    
    while (cond)
        monitor.waitEvents(vals, 100, -1);
}
```

Fill vector with data when stdin is ready for reading:
```c++
#include "lib/libFd/include/FdMonitor.hpp"

int main()
{
    fdlib::FdMonitor monitor(0);
    std::vector<std::any> vals;
    
    monitor.addMonitorFd(
        0, //target fd
        EPOLLIN, //event
        "hello, world"
    );
    
    while (cond) {
        monitor.waitEvents(vals, 100, -1);
        auto data = std::any_cast<std::string>(vals[0]); //do stuff with data
        std::cout << "Got: " << data << std::endl;
    }
}
```

## Pipe

## StateFd

## TimerFd

## Using elements together

## License

This code is open source software licensed under the Apache 2.0 License.