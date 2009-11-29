CXX = g++
CXXFLAGS = -Wall
DEFINES = -D ERRORLEVEL=2 -D WARNLEVEL=2 -D INFOLEVEL=2

debuglevel := 0

ifeq (0,${debuglevel})
	CXXFLAGS += -O2
	DEFINES += -D DEBUGLEVEL=0 -D NDEBUG
else ifeq (1,${debuglevel})
	CXXFLAGS += -g -O0
	DEFINES += -D DEBUGLEVEL=1
else
	CXXFLAGS += -g -O0
	DEFINES += -D DEBUGLEVEL=2
endif

all: quatseq.o quaternion.o safe_bool.o

quatseq.o: quatseq.h quatseq.cc quaternion.h debug_tools.h
	${CXX} ${CXXFLAGS} ${DEFINES} -c quatseq.cc -o quatseq.o

quaternion.o: quaternion.h quaternion.cc debug_tools.h safe_bool.h
	${CXX} ${CXXFLAGS} ${DEFINES} -c quaternion.cc -o quaternion.o

safe_bool.o: safe_bool.h safe_bool.cc
	${CXX} ${CXXFLAGS} ${DEFINES} -c safe_bool.cc -o safe_bool.o

package: Makefile README debug_tools.h safe_bool.h safe_bool.cc quaternion.h quaternion.cc quatseq.h quatseq.cc
	tar --lzma -cvf quatseq.tar.lzma $^

clean:
	@rm -f *.o quatseq.tar.lzma

.PHONY: all clear package
