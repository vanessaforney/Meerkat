# Makefile for GPIO library for Raspberry Pi                                     

CC = g++
CFLAGS = -ansi -Wall -g

all: GPIO.o life.o death.o
	$(CC) GPIO.o life.o -o life
	$(CC) GPIO.o death.o -o death 

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o life death
