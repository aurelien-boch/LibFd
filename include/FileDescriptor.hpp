#ifndef FILEDESCRIPTOR_HPP
#define FILEDESCRIPTOR_HPP

namespace fdlib
{
    class FileDescriptor
    {
        public:
            FileDescriptor(bool closeOnDestroy = false); // NOLINT(google-explicit-constructor)

            FileDescriptor(int fd, bool closeOnDestroy = false); // NOLINT(google-explicit-constructor)

            ~FileDescriptor();

            void bindFd(int fd);

            [[nodiscard]] int getNativeHandle() const noexcept
            { return _handle; }

        private:
            bool _closeOnDestroy;
            int _handle;
    };
}

#endif //FILEDESCRIPTOR_HPP
