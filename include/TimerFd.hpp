#ifndef TIMERFD_HPP
#define TIMERFD_HPP

#include <sys/timerfd.h>
#include "FileDescriptor.hpp"

namespace fdlib
{
    class TimerFd
    {
        public:
            TimerFd(int clockId, int flags);

            void startTimer() const;

            void clearTimer() const noexcept;

            void stopTimer() const;

            void restartTimer() const;

            void resetTimer();

            void setSecDelay(long seconds) noexcept;

            void setDelay(long nanoseconds) noexcept;

            void setSecRepeatRate(long seconds) noexcept;

            void setRepeatRate(long nanoseconds) noexcept;

            [[nodiscard]] long getSecDelay() const noexcept
            { return _timer.it_value.tv_sec; };

            [[nodiscard]] long getDelay() const noexcept
            { return _timer.it_value.tv_nsec; };

            [[nodiscard]] long getSecRepeatRate() const noexcept
            { return _timer.it_interval.tv_sec; };

            [[nodiscard]] long getRepeatRate() const noexcept
            { return _timer.it_interval.tv_nsec; };

            [[nodiscard]] const fdlib::FileDescriptor &getFd() const noexcept
            { return _fd; };

        private:
            itimerspec _timer;
            fdlib::FileDescriptor _fd;
    };
}

#endif //TIMERFD_HPP
