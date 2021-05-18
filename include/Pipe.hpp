#ifndef PIPE_HPP
#define PIPE_HPP

#include <unistd.h>
#include "FileDescriptor.hpp"

namespace fdlib
{
    class Pipe
    {
        public:
            Pipe() noexcept;

            ~Pipe() = default;

            template <typename T>
            void writeData(const T &data) const noexcept
            {
                write(_writingFd.getNativeHandle(), &data, sizeof(T));
            }

            template <typename T>
            T readData() const noexcept
            {
                T res;

                read(_readingFd.getNativeHandle(), &res, sizeof(T));
                return (res);
            }

            [[nodiscard]] const fdlib::FileDescriptor &getReadingFd() const noexcept
            { return _readingFd; };

            [[nodiscard]] const fdlib::FileDescriptor &getWritingFd() const noexcept
            { return _writingFd; };

        private:
            fdlib::FileDescriptor _readingFd;
            fdlib::FileDescriptor _writingFd;
    };
}

#endif //PIPE_HPP
