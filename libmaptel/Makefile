CXX = g++
CFLAGS = -Wall

debuglevel := 0

ifeq (0,${debuglevel})
	CFLAGS += -O2 -D MAPTEL_DEBUG_LEVEL=0
else ifeq (1,${debuglevel})
	CFLAGS += -g -O0 -D MAPTEL_DEBUG_LEVEL=1
else
	CFLAGS += -g -O0 -D MAPTEL_DEBUG_LEVEL=2
endif


all: maptel.o

maptel.o: maptel.cc maptel.h
	${CXX} ${CFLAGS} -c maptel.cc -o maptel.o

test: maptel.o
	${CXX} ${CFLAGS} maptel.o -o maptel

clean:
	@rm -f maptel.o *~

mrproper: clean

.PHONY: all clean mrproper

.SUFFIXES: .cc .o
