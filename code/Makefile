include ../Make.defines

#PIC_DIR = ../pictures

PROG = game
OBJS = game.o SceneManager.o Scene.o GameObject.o ErrorHandler.o
# OBJS = test.o


all: $(OBJS)
	g++ $(OBJS) -o game -lsfml-graphics -lsfml-window -lsfml-system -lGL
#		${CC} ${CFLAGS} -o test.o -c test.cpp
#		${CC} ${CFLAGS} -o test  test.o ${LIBS}


%.o: %.cpp
	g++ -c $<

clean:
	rm $(PROG)
	rm *.o
