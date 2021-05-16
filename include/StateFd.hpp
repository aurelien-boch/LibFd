#ifndef STATEFD_HPP
#define STATEFD_HPP

namespace fdlib
{
    class StateFd
    {
        public:
            StateFd();

            ~StateFd();

            void trigger() const;

            void clear() const;

            [[nodiscard]] int getFd() const;

        private:
            int _pipefd[2];
    };
}

#endif //STATEFD_HPP
