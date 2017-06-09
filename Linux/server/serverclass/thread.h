#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <iostream>
#include <string>
#include "../message/message.h"
#include "../master/master.h"
#include "../room/room.h"

class User;

/*
 Thread handles communications with client (class NetClient). For each user on a server a specific thread is created. 
 */

class Thread : public QThread {
    
    Q_OBJECT
public:
    Thread(qintptr ID, Master* masterptr, QObject *parent=nullptr);
    void run();
    void sendMessage(Message);
    void reinitiate();
    void sendVector();
    void sendHistory(); 
    void requestStruct();
    
signals:
    void error(QTcpSocket::SocketError socketerror);
    
public slots:
    void readyRead();
    void disconnected();
    
protected:
    void handleMessage(QString);
    void handleInitiate(std::string);
    void handleStructure();
    void handleHistory(QString);
    
private:
    QTcpSocket* TcpSocket;
    qintptr socketDescriptor;
    Master* masterPointer;
    User* userPointer;
    QByteArray compare;
    QByteArray breaker;
};

#endif //THREAD_H
