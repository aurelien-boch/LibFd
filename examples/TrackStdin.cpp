#include <iostream>
#include <FdMonitor.hpp>

static void callback(const fdlib::FileDescriptor &fd)
{
    std::string data;

    std::cout << "Fd: " << fd.getNativeHandle() << " has been triggered" << std::endl;
    std::cin >> data;
    std::cout << "Data: " << data << std::endl;
}

int main()
{
    std::cout << "Hello, world!" << std::endl;
    fdlib::FdMonitor monitor;

    monitor.addMonitorFd(
        0,
        fdlib::FdMonitor::EVENT_READ,
        std::function([](const fdlib::FileDescriptor &fd) { callback(fd); })
    );
    while (1)
        monitor.waitEvents(100, -1);
}