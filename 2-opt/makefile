CXX = g++-8
CXX_FLAGS = -std=c++17 -fopenmp # important flags.
CXX_FLAGS += -Wall -Wextra -Werror -pedantic -Wfatal-errors # source code quality.
CXX_FLAGS += -O3 -ffast-math # optimization.
CXX_FLAGS += -g # debugging.
CXX_FLAGS += -I./ # include paths.

SRCS = 2-opt.cpp PointSequence.cpp Optimizer.cpp \
    fileio/PointSet.cpp fileio/Tour.cpp

%.o: %.cpp; $(CXX) $(CXX_FLAGS) -o $@ -c $<

OBJS = $(SRCS:.cpp=.o)

all: $(OBJS); $(CXX) $^ -o 2-opt.out

clean: ; rm -rf 2-opt.out $(OBJS) *.dSYM
