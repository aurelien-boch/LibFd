#include <stdexcept>
#include <cstring>
#include <utility>
#include <unistd.h>
#include "FdMonitor.hpp"

fdlib::FdMonitor::FdMonitor(int epollFlags) :
    _epollFd(epoll_create1(epollFlags)),
    _functions{},
    _data{}
{
    if (this->_epollFd == -1)
        throw std::runtime_error(strerror(errno));
}

fdlib::FdMonitor::~FdMonitor()
{
    close(this->_epollFd);
}

void fdlib::FdMonitor::addMonitorFd(int targetFd, std::uint32_t events, std::any data)
{
    epoll_event epollData = {
        .events = events,
        .data = {
            .fd = targetFd
        }
    };

    if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, targetFd, &epollData) == -1)
        throw std::runtime_error(strerror(errno));
    this->_data.insert({targetFd, std::move(data)});
}

void fdlib::FdMonitor::addMonitorFd(int targetFd, std::uint32_t events, const std::function<void(int)> &data)
{
    epoll_event epollData = {
            .events = events,
            .data = {
                .fd = targetFd
            }
    };

    if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, targetFd, &epollData) == -1)
        throw std::runtime_error(strerror(errno));
    this->_functions.insert({targetFd, data});
}

void fdlib::FdMonitor::modMonitorFd(int targetFd, std::uint32_t events, std::any data) noexcept
{
    this->_editFdData(targetFd, events);
    this->_data.insert_or_assign(targetFd, std::move(data));
}

void fdlib::FdMonitor::modMonitorFd(int targetFd, std::uint32_t events, const std::function<void(int)> &data) noexcept
{
    this->_editFdData(targetFd, events);
    this->_functions.insert_or_assign(targetFd, data);
}

void fdlib::FdMonitor::delMonitorFd(int targetFd)
{
    const auto functionsIt = this->_functions.find(targetFd);
    const auto dataIt = this->_data.find(targetFd);

    if (functionsIt != this->_functions.end())
        this->_functions.erase(functionsIt);
    else if (dataIt != this->_data.end())
        this->_data.erase(dataIt);
    if (epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, targetFd, nullptr) == -1)
        throw std::runtime_error(strerror(errno));
}

void fdlib::FdMonitor::waitEvents(std::vector<std::any> &events, int maxEvents, int timeout) const
{
    int ret;
    std::vector<epoll_event> content(maxEvents);

    ret = epoll_wait(this->_epollFd, content.data(), maxEvents, timeout);
    if (ret == -1)
        throw std::runtime_error(strerror(errno));
    events.clear();
    content.resize(ret);
    this->_treatEvents(events, content);
}

void fdlib::FdMonitor::_treatEvents(std::vector<std::any> &res, std::vector<epoll_event> &events) const noexcept
{
    for (const auto &e : events) {
        const auto functionsIt = this->_functions.find(e.data.fd);
        const auto dataIt = this->_data.find(e.data.fd);

        if (functionsIt != this->_functions.end())
            functionsIt->second(e.data.fd);
        if (dataIt != this->_data.end())
            res.emplace_back(dataIt->second);
    }
}

void fdlib::FdMonitor::_editFdData(int targetFd, std::uint32_t events) const
{
    epoll_event epollData = {
        .events = events,
        .data = {
            .fd = targetFd
        }
    };

    if (epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, targetFd, &epollData) == -1)
        throw std::runtime_error(strerror(errno));
}

void fdlib::FdMonitor::unbindFdFunction(int targetFd) noexcept
{
    const auto functionIt = this->_functions.find(targetFd);

    if (functionIt != this->_functions.end())
        this->_functions.erase(functionIt);
}
