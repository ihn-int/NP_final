include ../Make.defines

PROG = main_serv test_serv
OBJS = main_serv.o RoomManager.o User.o StringBuff.o

all: $(PROG)

main_serv: $(OBJS)
	g++ $(CFLAGS) -o $@ $^ $(LIBS)

test_serv: test_serv.o StringBuff.o
	g++ $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	g++ $(CFLAGS) -c $^

clean:
	rm $(PROG)
	rm *.o