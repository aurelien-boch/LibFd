#ifndef STATEFD_HPP
#define STATEFD_HPP

namespace fdlib
{
    class StateFd
    {
        public:
            StateFd();

            ~StateFd();

            void trigger();

            void clear();

            int getFd();

        private:
            int _pipefd[2];
    };
}

#endif //STATEFD_HPP
