#include <iostream>
#include <StateFd.hpp>
#include <thread>

static void threadDriverProgram(fdlib::StateFd &fd)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    fd.trigger();
}

int main()
{
    fdlib::StateFd fd;
    std::thread t([&fd]() { threadDriverProgram(fd); });

    std::cout << "Waiting" << std::endl;
    fd.wait();
    t.join();
    std::cout << "Thread sent callback" << std::endl;
}
