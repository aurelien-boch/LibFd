#include <iostream>
#include <Pipe.hpp>


int main()
{
    fdlib::Pipe p;

    std::cout << "sent \"hi\" to pipe" << std::endl;
    p.writeData<const char *>("hi");
    std::cout << "got: \""<< p.readData<const char *>() << "\" from pipe" << std::endl;
    std::cout << "sent 15 to pipe" << std::endl;
    p.writeData(15);
    std::cout << "got: " << p.readData<int>() << " from pipe" << std::endl;
}
