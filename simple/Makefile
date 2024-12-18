#include ../Make.defines

#PIC_DIR = ../pictures

PROG = game
OBJS = game.o SceneManager.o Scene.o StartScene.o ErrorHandler.o



all: $(OBJS)
	g++ $(OBJS) -o game -lsfml-graphics -lsfml-window -lsfml-system -lGL -L./Connect/libunp.a

%.o: %.cpp
	g++ -I./Connect/lib -c $< 

clean:
	rm *.o
	rm $(PROG)
