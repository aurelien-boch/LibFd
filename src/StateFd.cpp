#include "StateFd.hpp"

fdlib::StateFd::StateFd() :
    _pipe()
{}


void fdlib::StateFd::trigger() const noexcept
{
    _pipe.writeData(true);
}

void fdlib::StateFd::clear() const noexcept
{
    _pipe.readData<bool>();
}

void fdlib::StateFd::wait() const noexcept
{
    this->clear();
}
