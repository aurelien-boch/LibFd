#ifndef FDMONITOR_HPP
#define FDMONITOR_HPP

#include <any>
#include <vector>
#include <functional>
#include <cstdint>
#include <sys/epoll.h>

namespace fdlib
{
    class FdMonitor
    {
        public:
            explicit FdMonitor(int epollFlags);

            ~FdMonitor();

            void addMonitorFd(int targetFd, std::uint32_t events, std::any data);

            void addMonitorFd(int targetFd, std::uint32_t events, const std::function<void(int fd)> &data);

            void modMonitorFd(int targetFd, std::uint32_t events, std::any data);

            void modMonitorFd(int targetFd, std::uint32_t events, const std::function<void(int fd)> &data);

            void unbindFdFunction(int targetFd);

            void delMonitorFd(int targetFd);

            void waitEvents(std::vector<std::any> &events, int maxEvents, int timeout) const;

        private:
            void _treatEvents(std::vector<std::any> &res, std::vector<epoll_event> &events) const;
            void _editFdData(int targetFd, std::uint32_t events) const;

            int _epollFd;
            std::unordered_map<int, std::function<void(int fd)>> _functions;
            std::unordered_map<int, std::any> _data;
    };
}

#endif //FDMONITOR_HPP
