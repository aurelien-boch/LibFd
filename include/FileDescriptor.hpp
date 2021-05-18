#ifndef FILEDESCRIPTOR_HPP
#define FILEDESCRIPTOR_HPP

namespace fdlib
{
    class FileDescriptor
    {
        public:
            explicit FileDescriptor(bool closeOnDestroy = false); // NOLINT(google-explicit-constructor)

            FileDescriptor(int fd, bool closeOnDestroy = false); // NOLINT(google-explicit-constructor)

            ~FileDescriptor();

            void bindFd(int fd);

            [[nodiscard]] int getNativeHandle() const noexcept
            { return _handle; }

            bool operator==(const int other) const noexcept
            { return (_handle == other); };

            operator int() const noexcept // NOLINT(google-explicit-constructor)
            { return _handle; };

            static const int INVALID_FD = 0;

        private:
            bool _closeOnDestroy;
            int _handle;
    };
}

#endif //FILEDESCRIPTOR_HPP
