# Define required macros here
SHELL = /bin/sh

OBJS = window.o event.o canvas.o freeDraw.o squareDraw.o
CXXFLAGS = -Wall -g -std=c++11
CXX = g++
INCLUDES = `pkg-config --cflags directfb`
LIBS = `pkg-config --libs directfb`

dfb_draw: ${OBJS}
	${CXX} ${CXXFLAGS} ${INCLUDES} -o $@ $@.cpp ${OBJS} ${LIBS}

clean:
	-rm -f *.o
	-rm -f dfb_draw

.cpp.o:
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $<