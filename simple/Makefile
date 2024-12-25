#include ../Make.defines

#PIC_DIR = ../pictures
SERV = server
PROG = game
OBJS = game.o SceneManager.o Scene.o StartScene.o BJScene.o C4Scene.o ErrorHandler.o StringBuff.o


all: $(OBJS)
	g++ $(OBJS) -o game -lsfml-graphics -lsfml-window -lsfml-system -lGL -L./Connect/ -lunp

server: server.o
	g++ server.o -o server -L./Connect/ -lunp

%.o: %.cpp
	g++ -I./Connect/lib -c $< 

clean:
	rm *.o
	rm $(PROG)
	rm server
