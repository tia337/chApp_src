#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include "thread.h"
#include "../master/master.h"

/* 
creates threads which each handles connection with it's specific client. 
*/

class Server : public QTcpServer {
    
    Q_OBJECT
public:
    Server(Master* masterPointer, QObject *parent=nullptr);
    void start();

signals:
    
public slots:

protected:
    void incomingConnection(qintptr handle);
    Master* masterPointer;
    
    
};

#endif // SERVER_H
