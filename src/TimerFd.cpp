#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include "TimerFd.hpp"

fdlib::TimerFd::TimerFd(int clockId) :
    _timer{},
    _fd(timerfd_create(clockId, 0), true)
{
    if (_fd.getNativeHandle() == FileDescriptor::INVALID_FD)
        throw std::runtime_error(strerror(errno));
}

void fdlib::TimerFd::setSecDelay(long seconds) noexcept
{
    _timer.it_value.tv_sec = seconds;
}

void fdlib::TimerFd::setDelay(long nanoseconds) noexcept
{
    std::int64_t seconds = nanoseconds / 1000000000;

    if (seconds > 0) {
        nanoseconds -= seconds * 1000000000;
        _timer.it_value.tv_sec = seconds;
    }
    _timer.it_value.tv_nsec = nanoseconds;
}

void fdlib::TimerFd::setSecRepeatRate(long seconds) noexcept
{
    _timer.it_interval.tv_sec = seconds;
}

void fdlib::TimerFd::setRepeatRate(long nanoseconds) noexcept
{
    std::int64_t seconds = nanoseconds / 1000000000;

    if (seconds > 0) {
        nanoseconds -= seconds * 1000000000;
        _timer.it_interval.tv_sec = seconds;
    }
    _timer.it_interval.tv_nsec = nanoseconds;
}

void fdlib::TimerFd::startTimer() const
{
    if (timerfd_settime(_fd.getNativeHandle(), 0, &_timer, nullptr) == -1)
       throw std::runtime_error(strerror(errno));
}

void fdlib::TimerFd::clearTimer() const noexcept
{
    uint64_t data;

    read(_fd.getNativeHandle(), &data, sizeof(data));
}

void fdlib::TimerFd::stopTimer() const
{
    itimerspec timer{};

    if (timerfd_settime(_fd.getNativeHandle(), 0, &timer, nullptr) == -1)
        throw std::runtime_error(strerror(errno));
}

void fdlib::TimerFd::restartTimer() const
{
    this->startTimer();
}

void fdlib::TimerFd::resetTimer()
{
    std::memset(&_timer, 0, sizeof(_timer));
    this->startTimer();
}
