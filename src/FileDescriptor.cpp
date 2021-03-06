#include <stdexcept>
#include <unistd.h>
#include "FileDescriptor.hpp"

fdlib::FileDescriptor::FileDescriptor(int fd, bool closeOnDestroy) :
    _closeOnDestroy(closeOnDestroy),
    _handle(fd)
{}

fdlib::FileDescriptor::~FileDescriptor()
{
    if (_closeOnDestroy)
        close(_handle);
}

fdlib::FileDescriptor::FileDescriptor(bool closeOnDestroy) :
    _closeOnDestroy(closeOnDestroy),
    _handle(INVALID_FD)
{}

void fdlib::FileDescriptor::bindFd(int fd)
{
    if (_handle != INVALID_FD)
        throw std::runtime_error("Trying to bind a fd to a container that already contains a fd");
    _handle = fd;
}
