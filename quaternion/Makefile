CXX := g++
CXXFLAGS := -Wall -g -D DEBUGLEVEL=2

all: quatseq.o quaternion.o debug_tools.o

quatseq.o: quatseq.h quatseq.cc quaternion.o
	${CXX} ${CXXFLAGS} -c quatseq.cc -o quatseq.o

quaternion.o: quaternion.h quaternion.cc safe_bool.o
	${CXX} ${CXXFLAGS} -c quaternion.cc -o quaternion.o

debug_tools.o: debug_tools.h debug_tools.cc
	${CXX} ${CXXFLAGS} -c debug_tools.cc -o debug_tools.o

safe_bool.o: safe_bool.h safe_bool.cc
	${CXX} ${CXXFLAGS} -c safe_bool.cc -o safe_bool.o

clean:
	@rm -f *.o

.PHONY: all clear
