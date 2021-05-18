#include <iostream>
#include <FileDescriptor.hpp>

int main()
{
    fdlib::FileDescriptor autoClosing(open("AutoClosingFileDescriptor.cpp", O_RDONLY), true);

    read()
}
