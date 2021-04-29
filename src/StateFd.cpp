#include <StateFd.hpp>
#include <unistd.h>

fdlib::StateFd::StateFd()
{
    pipe(this->_pipefd);
}

fdlib::StateFd::~StateFd()
{
    close(this->_pipefd[0]);
    close(this->_pipefd[1]);
}

void fdlib::StateFd::trigger()
{
    char i = 1;

    write(this->_pipefd[1], &i, sizeof(char));
}

void fdlib::StateFd::clear()
{
    char i;

    read(this->_pipefd[0], &i, sizeof(char));
}

int fdlib::StateFd::getFd()
{
    return (this->_pipefd[0]);
}
