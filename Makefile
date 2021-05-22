CPPFLAGS	:=	-Wall		\
				-Wextra		\
				-Weffc++	\
				-I include/	\
				-std=gnu++2a

SRC			:=	src/FdMonitor.cpp	\
				src/Pipe.cpp		\
				src/StateFd.cpp		\
				src/TimerFd.cpp

OBJ			=	$(SRC:.cpp=.o)

NAME		=	libfd.a

RM			= rm -rf
CXX			= g++

all: $(OBJ)
	ar rc -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all