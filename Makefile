CPPFLAGS=-O2 -g -Wall -Werror
SRCFILES=meerkat.cpp

all: meerkat

meerkat: $(SRCFILES)
	g++ $(CPPFLAGS) -o meerkat $^

clean:
	rm -rf *.o *.dSYM meerkat
