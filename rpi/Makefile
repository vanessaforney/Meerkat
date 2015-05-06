# Makefile for GPIO library for Raspberry Pi                                     

CC = g++
CFLAGS = -Wall -g
OBJS = GPIO.o

all: $(OBJS)
	$(CC) $(OBJS) -o gpio

%.c: %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o gpio
