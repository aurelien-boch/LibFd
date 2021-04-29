#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <TimerFd.hpp>

fdlib::TimerFd::TimerFd(int clockId, int flags)
{
    this->_fd = timerfd_create(clockId, flags);

    if (this->_fd == -1)
        throw std::runtime_error(strerror(errno));
    this->_timer = {
        .it_interval = {
            .tv_sec = 0,
            .tv_nsec = 0
        },
        .it_value = {
            .tv_sec = 0,
            .tv_nsec = 0
        }
    };
}

fdlib::TimerFd::~TimerFd()
{
    close(this->_fd);
}

void fdlib::TimerFd::setSecDelay(long seconds)
{
    this->_timer.it_value.tv_sec = seconds;
}

void fdlib::TimerFd::setDelay(long nanoseconds)
{
    std::int64_t seconds = nanoseconds / 1000000000;

    if (seconds > 0) {
        nanoseconds -= seconds * 1000000000;
        this->_timer.it_value.tv_sec = seconds;
    }
    this->_timer.it_value.tv_nsec = nanoseconds;
}

void fdlib::TimerFd::setSecRepeatRate(long seconds)
{
    this->_timer.it_interval.tv_sec = seconds;
}

void fdlib::TimerFd::setRepeatRate(long nanoseconds)
{
    std::int64_t seconds = nanoseconds / 1000000000;

    if (seconds > 0) {
        nanoseconds -= seconds * 1000000000;
        this->_timer.it_interval.tv_sec = seconds;
    }
    this->_timer.it_interval.tv_nsec = nanoseconds;
}

long fdlib::TimerFd::getSecDelay() const
{
    return (this->_timer.it_value.tv_sec);
}

long fdlib::TimerFd::getDelay() const
{
    return (this->_timer.it_value.tv_nsec);
}

long fdlib::TimerFd::getSecRepeatRate() const
{
    return (this->_timer.it_interval.tv_sec);
}

long fdlib::TimerFd::getRepeatRate() const
{
    return (this->_timer.it_interval.tv_nsec);
}

void fdlib::TimerFd::startTimer()
{
    if (timerfd_settime(this->_fd, 0, &(this->_timer), nullptr) == -1)
       throw std::runtime_error(strerror(errno));
}

void fdlib::TimerFd::clearTimer() const
{
    uint64_t data;

    read(this->_fd, &data, sizeof(data));
}

void fdlib::TimerFd::stopTimer() const
{
    itimerspec timer = {
        .it_interval = {
            .tv_sec = 0,
            .tv_nsec = 0
        },
        .it_value = {
            .tv_sec = 0,
            .tv_nsec = 0
        }
    };

    if (timerfd_settime(this->_fd, 0, &timer, nullptr) == -1)
        throw std::runtime_error(strerror(errno));
}

void fdlib::TimerFd::resetTimer()
{
    this->startTimer();
}
