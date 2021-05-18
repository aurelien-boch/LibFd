#ifndef STATEFD_HPP
#define STATEFD_HPP

#include "Pipe.hpp"

namespace fdlib
{
    class StateFd
    {
        public:
            StateFd();

            ~StateFd() = default;

            void trigger() const noexcept;

            void clear() const noexcept;

            void wait() const noexcept;

            [[nodiscard]] const FileDescriptor &getFd() const noexcept
            { return _pipe.getReadingFd(); }

        private:
            fdlib::Pipe _pipe;
    };
}

#endif //STATEFD_HPP
