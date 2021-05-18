#include <iostream>
#include <FdMonitor.hpp>
#include <TimerFd.hpp>

static void stdinCb(const fdlib::FileDescriptor &fd)
{
    std::string data;

    std::cout << "Fd: " << fd.getNativeHandle() << " has been triggered" << std::endl;
    std::cin >> data;
    std::cout << "Data: " << data << std::endl;
}

static void timeredCb(const fdlib::FileDescriptor &fd, fdlib::TimerFd &timer)
{
    timer.clearTimer();
    std::cout << "Fd: " << fd.getNativeHandle() << " has been triggered" << std::endl;
}

int main()
{
    fdlib::FdMonitor monitor;
    fdlib::TimerFd timer;

    timer.setSecDelay(1);
    timer.setSecRepeatRate(1);
    timer.startTimer();
    monitor.addMonitorFd(
        0,
        fdlib::FdMonitor::EVENT_READ,
        std::function([](const fdlib::FileDescriptor &fd) { stdinCb(fd); })
    );
    monitor.addMonitorFd(
        timer.getFd(),
        fdlib::FdMonitor::EVENT_READ,
        std::function([&timer](const fdlib::FileDescriptor &fd) { timeredCb(fd, timer); })
    );
    while (1)
        monitor.waitEvents(100, -1);
}