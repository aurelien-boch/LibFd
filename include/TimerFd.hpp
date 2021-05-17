#ifndef TIMERFD_HPP
#define TIMERFD_HPP

#include <sys/timerfd.h>

namespace fdlib
{
    class TimerFd
    {
        public:
            TimerFd(int clockId, int flags);

            ~TimerFd();

            void setSecDelay(long seconds) noexcept;

            void setDelay(long nanoseconds) noexcept;

            void setSecRepeatRate(long seconds) noexcept;

            void setRepeatRate(long nanoseconds) noexcept;

            [[nodiscard]] long getSecDelay() const noexcept
            { return (this->_timer.it_value.tv_sec); };

            [[nodiscard]] long getDelay() const noexcept
            { return (this->_timer.it_value.tv_nsec); };

            [[nodiscard]] long getSecRepeatRate() const noexcept
            { return (this->_timer.it_interval.tv_sec); };

            [[nodiscard]] long getRepeatRate() const noexcept
            { return (this->_timer.it_interval.tv_nsec); };

            [[nodiscard]] int getFd() const noexcept
            { return (this->_fd); };

            void startTimer() const;

            void clearTimer() const noexcept;

            void stopTimer() const;

            void resetTimer() const;

        private:
            itimerspec _timer;

            int _fd;
    };
}

#endif //TIMERFD_HPP
