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

            [[nodiscard]] long getSecDelay() const;

            [[nodiscard]] long getDelay() const;

            [[nodiscard]] long getSecRepeatRate() const;

            [[nodiscard]] long getRepeatRate() const;

            void startTimer();

            void clearTimer() const;

            void stopTimer() const;

            void resetTimer();

            int getFd()
            { return (this->_fd); };

        private:
            itimerspec _timer;
            int _fd;
    };
}

#endif //TIMERFD_HPP
