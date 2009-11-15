CXX := g++
CXXFLAGS := -Wall -g -D DEBUGLEVEL=5

all: quatsec.o

quatsec.o: quatsec.h quatsec.cc
	${CXX} ${CXXFLAGS} -c quatsec.cc -o quatsec.o

quaternion.o: quaternion.h quaternion.cc
	${CXX} ${CXXFLAGS} -c quaternion.cc -o quaternion.o

clean:
	@rm -f *.o

.PHONY: all clear
