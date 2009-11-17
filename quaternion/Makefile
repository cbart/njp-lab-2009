CXX := g++
CXXFLAGS := -Wall -g -D DEBUGLEVEL=2

all: quatsec.o quaternion.o debug_tools.o

quatsec.o: quatsec.h quatsec.cc
	${CXX} ${CXXFLAGS} -c quatsec.cc -o quatsec.o

quaternion.o: quaternion.h quaternion.cc
	${CXX} ${CXXFLAGS} -c quaternion.cc -o quaternion.o

debug_tools.o: debug_tools.h debug_tools.cc
	${CXX} ${CXXFLAGS} -c debug_tools.cc -o debug_tools.o

clean:
	@rm -f *.o

.PHONY: all clear
