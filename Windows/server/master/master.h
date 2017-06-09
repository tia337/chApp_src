#ifndef MASTER_H
#define MASTER_H

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <QReadWriteLock>
#include "../getTime.h"
#include "../room/room.h"

/*
Class running the back end of the server, creating and controlling users and rooms.
 
 */

class Master {
public:
    Master();
    ~Master();
    
    // Rooms
    class Room* createRoom(std::string);
    void removeRoom(std::string);
    Room* getRoom(std::string);
    Room* getTop();
    
    // Users (subclass of rooms)
    class User* createUser(std::string);
    void removeUser(std::string);
    
    void updateStructForAll(); // Send info regarding users / rooms to all clients
    
    void printVector(); // testing purposes 
    
protected:
    bool userOrNot(Room* inRoom);

    unsigned int getPosOfRoom(std::string);
    std::vector<Room*> rooms;
    Room* topRoom;

    QReadWriteLock roomLock;
};

#endif //MASTER_H
