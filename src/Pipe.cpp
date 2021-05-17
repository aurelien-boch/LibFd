#include "Pipe.hpp"

fdlib::Pipe::Pipe() noexcept :
    _pipefd()
{
    pipe(this->_pipefd);
}

fdlib::Pipe::~Pipe()
{
    close(this->_pipefd[0]);
    close(this->_pipefd[1]);
}
