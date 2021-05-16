#include <Pipe.hpp>

fdlib::Pipe::Pipe()
{
    pipe(this->_pipefd);
}

fdlib::Pipe::~Pipe()
{
    close(this->_pipefd[0]);
    close(this->_pipefd[1]);
}
