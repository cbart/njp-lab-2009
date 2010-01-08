CXX = g++
CXXFLAGS = -Wall -g

all: wyszukiwanie

wyszukiwanie: wyszukiwanie.cc double_metaphone.h double_metaphone.o
	${CXX} ${CXXFLAGS} -o wyszukiwanie wyszukiwanie.cc double_metaphone.o

double_metaphone.o: double_metaphone.h double_metaphone.cc
	${CXX} ${CXXFLAGS} -c -o double_metaphone.o double_metaphone.cc

package: double_metaphone.h double_metaphone.cc wyszukiwanie.cc Makefile
	tar --lzma -cvf wyszukiwanie.tar.lzma $^

clean:
	@rm -f *.o

.PHONY: all clean package
