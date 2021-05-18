#include "Pipe.hpp"

fdlib::Pipe::Pipe() noexcept :
    _readingFd(true),
    _writingFd(true)
{
    int pipeFds[2];

    pipe(pipeFds);
    _readingFd.bindFd(pipeFds[0]);
    _writingFd.bindFd(pipeFds[1]);
}
