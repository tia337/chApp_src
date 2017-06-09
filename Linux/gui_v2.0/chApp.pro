QMAKE_CXX	= g++
QMAKE_CC	= gcc

QT		+= core gui network widgets multimedia
RC_FILE		= chApp.rc

TARGET		= chApp
TEMPLATE	= app

CONFIG		+= c++11
CONFIG		+= release

# ---- Files ----

SOURCES += main.cpp chatwindow.cpp logindialog.cpp NetClient.cpp \
    gui.cpp \
    aboutwindow.cpp

HEADERS  += chatwindow.h logindialog.h NetClient.h \
    gui.h \
    aboutwindow.h

TRANSLATIONS += \
    Translations/chApp_ru.ts\
    Translations/chApp_en.ts\
    Translations/chApp_ua.ts

tr.commands = lupdate $$_PRO_FILE_ && lrelease $$_PRO_FILE_

PRE_TARGETDEPS += tr

QMAKE_EXTRA_TARGETS += tr

FORMS    += chatwindow.ui logindialog.ui \
    aboutwindow.ui

RESOURCES += myResources.qrc

