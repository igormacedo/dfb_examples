# Define required macros here
SHELL = /bin/sh

CXXFLAGS = -Wall -g
CXX = g++
INCLUDES = `pkg-config --cflags directfb`
LIBS = `pkg-config --libs directfb`

first: ${OBJS}
	${CXX} ${CXXFLAGS} $@.cpp ${INCLUDES} -o $@ ${LIBS}

clean:
	-rm -f first

