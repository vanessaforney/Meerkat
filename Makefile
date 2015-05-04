CPPFLAGS=-O2 -g -std=c++11
SRCFILES=meerkat.cpp driver.cpp

all: meerkat

meerkat: $(SRCFILES)
	g++ $(CPPFLAGS) -o meerkat $^

clean:
	rm -rf *.o *.dSYM meerkat
