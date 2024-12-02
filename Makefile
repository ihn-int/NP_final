#include ../Make.defines

PROG = game
SRCS = game.cpp
OBJS = game.o

all: $(OBJS)
	g++ $(OBJS) -o game -lsfml-graphics -lsfml-window -lsfml-system

%.o: %.cpp
	g++ -c $<

clean:
	rm $(PROG)
	rm *.o