#include "../room/room.h"
#include "../serverclass/thread.h"

using namespace std;

/*
 User class. Issues commands from user and holds information about user.
 */

User::~User(){
    parentRoom->removeRoom(this);
    parentRoom = nullptr;
    masterPointer = nullptr;
}

// ----------------------------------

void User::chooseRoom(Room* newRoom){
    if(this->parentRoom != nullptr){
		parentRoom->removeRoom(this);
	}
    newRoom->addRoom(this);
}

// ----------------------------------

void User::receiveMessage(Message inMessage){
    msgQueueLock.lockForWrite();

    log.push_back(inMessage); //tbd
    thread->sendMessage(inMessage);

    msgQueueLock.unlock();
}

// ----------------------------------

void User::sendMessage(Message outMessage){
    parentRoom->receiveMessage(outMessage);
    msgQueueLock.lockForWrite();
    log.push_back(outMessage);
    msgQueueLock.unlock();
}

// ----------------------------------

void User::initRoom(string name){
    parentRoom->addRoom(masterPointer->createRoom(name));
}

// ----------------------------------

void User::setThread(class Thread* inThread) {
    thread = inThread;
}

// ----------------------------------

void User::requestStruct() {
    thread->requestStruct();
}

// ----------------------------------

void User::setFilePath() {
    //fake
}

// ----------------------------------

void User::readAllFromFile() {
    //fake func for room
}

// ----------------------------------

vector<string> User::getStruct() {
    return parentRoom->getStruct();
}
