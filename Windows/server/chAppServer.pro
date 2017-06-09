
# Use GNU compilers - not clang
QMAKE_CXX	= g++
QMAKE_CC	= gcc

QMAKE_CXXFLAGS	+= -g

# Qt components required. Server does not use GUI. 
QT		+= core
QT		+= network
QT		-= gui
RC_FILE		= chApp_serv.rc
ICON		= icon/icon/icon.ico

TARGET		= chAppServer

# Use C++11 standard
CONFIG		+= console c++11
CONFIG		-= app_bundle

TEMPLATE	= app

# Files to include while building
SOURCES		+= main.cpp getTime.cpp master/master.cpp room/room.cpp user/User.cpp message/message.cpp serverclass/server.cpp serverclass/thread.cpp

HEADERS		+= master/master.h room/room.h message/message.h serverclass/thread.h serverclass/server.h

RESOURCES += \
    myresource.qrc

