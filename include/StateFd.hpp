#ifndef STATEFD_HPP
#define STATEFD_HPP

namespace fdlib
{
    class StateFd
    {
        public:
            StateFd();

            ~StateFd();

            void trigger() const noexcept;

            void clear() const noexcept;

            [[nodiscard]] int getFd() const noexcept
            { return this->_pipefd[0]; }

        private:
            int _pipefd[2]{};
    };
}

#endif //STATEFD_HPP
