#include <stdexcept>
#include <cstring>
#include "FdMonitor.hpp"

fdlib::FdMonitor::FdMonitor() :
    _fd(epoll_create1(0), true),
    _events{},
    _functions{},
    _data{}
{
    if (_fd.getNativeHandle() == -1)
        throw std::runtime_error(strerror(errno));
}

void fdlib::FdMonitor::addMonitorFd(const fdlib::FileDescriptor &fd, fdlib::FdMonitor::MONITOR_FLAGS events, std::any data)
{
    epoll_event epollData = {
        .events = events,
        .data = {
            .fd = fd.getNativeHandle()
        }
    };

    if (epoll_ctl(_fd.getNativeHandle(), EPOLL_CTL_ADD, fd.getNativeHandle(), &epollData) == -1)
        throw std::runtime_error(strerror(errno));
    _data.insert_or_assign(fd.getNativeHandle(), std::move(data));
    _events.insert_or_assign(fd.getNativeHandle(), events);
}

void fdlib::FdMonitor::addMonitorFd(const fdlib::FileDescriptor &fd, fdlib::FdMonitor::MONITOR_FLAGS events, const std::function<void(const FileDescriptor &)> &fun)
{
    epoll_event epollData = {
        .events = events,
        .data = {
            .fd = fd.getNativeHandle()
        }
    };

    if (epoll_ctl(_fd.getNativeHandle(), EPOLL_CTL_ADD, fd.getNativeHandle(), &epollData) == -1)
        throw std::runtime_error(strerror(errno));
    _functions.insert_or_assign(fd.getNativeHandle(), fun);
    _events.insert_or_assign(fd.getNativeHandle(), events);
}

void fdlib::FdMonitor::modMonitorFd(const fdlib::FileDescriptor &fd, fdlib::FdMonitor::MONITOR_FLAGS events)
{
    const auto eventsIt = _events.find(fd.getNativeHandle());
    epoll_event evt = {
        .events = events,
        .data = {
            .fd = fd.getNativeHandle()
        }
    };

    if (eventsIt == _events.end())
        throw std::runtime_error("Trying to edit an fd that is not monitored");
    eventsIt->second = events;
    _updateFd(fd, evt);
}

void fdlib::FdMonitor::modMonitorFd(const fdlib::FileDescriptor &fd, std::any data)
{
    if (!this->isFDMonitored(fd))
        throw std::runtime_error("Trying to edit an fd that is not monitored");
    _data.insert_or_assign(fd.getNativeHandle(), std::move(data));
}

void fdlib::FdMonitor::modMonitorFd(const fdlib::FileDescriptor &fd, const std::function<void(int)> &data)
{
    if (!this->isFDMonitored(fd))
        throw std::runtime_error("Trying to edit an fd that is not monitored");
    _functions.insert_or_assign(fd.getNativeHandle(), data);
}

void fdlib::FdMonitor::modMonitorFd(const fdlib::FileDescriptor &fd, fdlib::FdMonitor::MONITOR_FLAGS events, std::any data)
{
    this->modMonitorFd(fd, events);
    _data.insert_or_assign(fd.getNativeHandle(), std::move(data));
}

void fdlib::FdMonitor::modMonitorFd(const fdlib::FileDescriptor &fd, fdlib::FdMonitor::MONITOR_FLAGS events, const std::function<void(const FileDescriptor &)> &data)
{
    this->modMonitorFd(fd, events);
    _functions.insert_or_assign(fd.getNativeHandle(), data);
}

void fdlib::FdMonitor::delMonitorFd(const fdlib::FileDescriptor &fd)
{
    const auto functionsIt = _functions.find(fd.getNativeHandle());
    const auto dataIt = _data.find(fd.getNativeHandle());

    if (functionsIt != _functions.end())
        _functions.erase(functionsIt);
    if (dataIt != _data.end())
        _data.erase(dataIt);
    if (epoll_ctl(_fd.getNativeHandle(), EPOLL_CTL_DEL, fd.getNativeHandle(), nullptr) == -1)
        throw std::runtime_error(strerror(errno));
}

void fdlib::FdMonitor::waitEvents(int maxEvents, int timeout) const
{
    int ret;
    std::vector<epoll_event> content(maxEvents);

    ret = epoll_wait(_fd.getNativeHandle(), content.data(), maxEvents, timeout);
    if (ret == -1)
        throw std::runtime_error(strerror(errno));
    content.resize(ret);
    _callFunctions(content);
}

void fdlib::FdMonitor::waitEvents(std::vector<std::pair<int, std::any>> &data, int maxEvents, int timeout) const
{
    int ret;
    std::vector<epoll_event> content(maxEvents);

    ret = epoll_wait(_fd.getNativeHandle(), content.data(), maxEvents, timeout);
    if (ret == -1)
        throw std::runtime_error(strerror(errno));
    data.clear();
    content.resize(ret);
}

bool fdlib::FdMonitor::isFDMonitored(const fdlib::FileDescriptor &fd) const noexcept
{
    const auto eventsIt = _events.find(fd.getNativeHandle());

    if (eventsIt == _events.end())
        return (false);
    return (true);
}

void fdlib::FdMonitor::_updateFd(const fdlib::FileDescriptor &fd, epoll_event &evt)
{
    if (epoll_ctl(_fd.getNativeHandle(), EPOLL_CTL_MOD, fd.getNativeHandle(), &evt) == -1)
        throw std::runtime_error(strerror(errno));
}

void fdlib::FdMonitor::_callFunctions(const std::vector<epoll_event> &vec) const noexcept
{
    for (const auto &e : vec) {
        const auto functionsIt = _functions.find(e.data.fd);

        if (functionsIt != _functions.end())
            functionsIt->second(e.data.fd);
    }
}

void fdlib::FdMonitor::_processEvents(const std::vector<epoll_event> &vec, std::vector<std::pair<int, std::any>> &data) const noexcept
{
    for (const auto &e : vec) {
        const auto functionsIt = _functions.find(e.data.fd);
        const auto dataIt = _data.find(e.data.fd);

        if (functionsIt != _functions.end())
            functionsIt->second(e.data.fd);
        if (dataIt != _data.end())
            data.emplace_back(std::pair<int, std::any>({dataIt->first, dataIt->second}));
    }
}
