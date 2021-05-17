#ifndef PIPE_HPP
#define PIPE_HPP

#include <unistd.h>

namespace fdlib
{
    class Pipe
    {
        public:
            Pipe() noexcept;

            ~Pipe();

            template <typename T>
            void sendData(const T &data) const noexcept
            {
                write(this->_pipefd[1], &data, sizeof(T));
            }


            template <typename T>
            T readData() const noexcept
            {
                T res;

                read(this->_pipefd[0], &res, sizeof(T));
                return (res);
            }

            [[nodiscard]] int getReadingFd() const noexcept
            { return (this->_pipefd[0]); };

            [[nodiscard]] int getWritingFd() const noexcept
            { return (this->_pipefd[0]); };

        private:
            int _pipefd[2]{};
    };
}

#endif //PIPE_HPP
