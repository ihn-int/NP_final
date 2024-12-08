# ifndef __room__
# define __room__

# include "unp.h"

class Room{
public:
    int getCounter() {return counter;}
    int getRoomID() {return roomID;}
private:
    int counter;
    int roomID;
}

class Lobby: Room{

}

# endif