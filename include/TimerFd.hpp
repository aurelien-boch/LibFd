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

            void setSecDelay(long seconds);

            void setDelay(long nanoseconds);

            void setSecRepeatRate(long seconds);

            void setRepeatRate(long nanoseconds);

            [[nodiscard]] long getSecDelay() const
            { return (this->_timer.it_value.tv_sec); };

            [[nodiscard]] long getDelay() const
            { return (this->_timer.it_value.tv_nsec); };

            [[nodiscard]] long getSecRepeatRate() const
            { return (this->_timer.it_interval.tv_sec); };

            [[nodiscard]] long getRepeatRate() const
            { return (this->_timer.it_interval.tv_nsec); };

            [[nodiscard]] int getFd() const
            { return (this->_fd); };

            void startTimer() const;

            void clearTimer() const;

            void stopTimer() const;

            void resetTimer() const;

        private:
            itimerspec _timer;
            int _fd;
    };
}

#endif //TIMERFD_HPP
