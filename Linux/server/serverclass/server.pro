QMAKE_CXX	= g++
QMAKE_CC	= gcc

# Qt components required. Server does not use GUI. 
QT		+= core
QT		+= network
QT		-= gui

TARGET		= server

# Use C++11 standard
CONFIG		+= console c++11
CONFIG		-= app_bundle

TEMPLATE	= app

# Files to include while building
SOURCES		+= main.cpp server.cc thread.cpp ../master/master.cpp ../user/User.cpp
HEADERS		+= server.h thread.h ../master/master.h ../room/room.h

