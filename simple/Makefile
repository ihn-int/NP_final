#include ../Make.defines

#PIC_DIR = ../pictures

PROG = game
OBJS = game.o SceneManager.o Scene.o ChatScene.o ErrorHandler.o



all: $(OBJS)
	g++ $(OBJS) -o game -lsfml-graphics -lsfml-window -lsfml-system -lGL

%.o: %.cpp
	g++ -c $<

clean:
	rm $(PROG)
	rm *.o
