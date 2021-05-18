#include <iostream>
#include <FdMonitor.hpp>

int main()
{
    std::vector<std::pair<int, std::any>> data;
    fdlib::FdMonitor monitor;

    monitor.addMonitorFd(
        0,
        fdlib::FdMonitor::EVENT_READ,
        "hi"
    );
    while (true) {
        monitor.waitEvents(data, 100, -1);
        for (const auto &e : data)
            if (e.first == 0) {
                std::string data;

                std::cin >> data;
                std::cout << "Got (from values): " << std::any_cast<const char *>(e.second) << std::endl;
                std::cout << "Got (from stdin): " << data << std::endl;
            }
    }
}
