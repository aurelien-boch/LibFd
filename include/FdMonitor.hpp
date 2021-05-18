#ifndef FDMONITOR_HPP
#define FDMONITOR_HPP

#include <any>
#include <vector>
#include <functional>
#include <cstdint>
#include <sys/epoll.h>
#include "FileDescriptor.hpp"

namespace fdlib
{
    class FdMonitor
    {
        public:
            enum MONITOR_FLAGS {
                EVENT_READ = EPOLLIN,
                EVENT_WRITE = EPOLLOUT
            };

            FdMonitor();

            ~FdMonitor() = default;

            void addMonitorFd(const fdlib::FileDescriptor &fd, MONITOR_FLAGS events, std::any data);

            void addMonitorFd(const fdlib::FileDescriptor &fd, MONITOR_FLAGS events, const std::function<void (const FileDescriptor &fd)> &fun);

            void modMonitorFd(const fdlib::FileDescriptor &fd, MONITOR_FLAGS events);

            void modMonitorFd(const fdlib::FileDescriptor &fd, std::any data);

            void modMonitorFd(const fdlib::FileDescriptor &fd, const std::function<void(const FileDescriptor &fd)> &data);

            void modMonitorFd(const fdlib::FileDescriptor &fd, MONITOR_FLAGS events, std::any data);

            void modMonitorFd(const fdlib::FileDescriptor &fd, MONITOR_FLAGS events, const std::function<void(const FileDescriptor &fd)> &data);

            void delMonitorFd(const fdlib::FileDescriptor &fd);

            void waitEvents(int maxEvents, int timeout) const;

            void waitEvents(std::vector<std::pair<int, std::any>> &data, int maxEvents, int timeout) const;

            bool isFDMonitored(const fdlib::FileDescriptor &fd) const noexcept;

        private:

            void _updateFd(const fdlib::FileDescriptor &fd, epoll_event &evt);

            void _callFunctions(const std::vector<epoll_event> &vec) const noexcept;

            void _processEvents(const std::vector<epoll_event> &vec, std::vector<std::pair<int, std::any>> &data) const noexcept;

            fdlib::FileDescriptor _fd;
            std::unordered_map<int, MONITOR_FLAGS> _events;
            std::unordered_map<int, std::function<void(const FileDescriptor &fd)>> _functions;
            std::unordered_map<int, std::any> _data;
    };
}

#endif //FDMONITOR_HPP
