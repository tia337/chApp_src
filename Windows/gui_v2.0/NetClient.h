#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QString>
#include <iostream>
#include <stdexcept>
#include <QHostInfo>
//#include <QDebug>
#include <string>

class Gui;

class NetClient : public QObject
{
    Q_OBJECT
public:
    NetClient(QString, QString, Gui*, QObject *parent = 0);
    void start();
    void sendMessage(QString, QString, QString);
    void setName(QString);
    void getStruct();
    void getHistory(unsigned int);
    
signals:
    
public slots:
    void connected();
    void disconnected();
    void readyRead();

private:
    QString incompleteCommand = "";
    Gui* guiPointer;
    QTcpSocket *TcpSocket;
    QString name;
    QString address;
    QByteArray compare;
    QByteArray breaker;
    void handleHistory(QString);
    void handleMessage(QString);
    void handleStructure(QString);
    void handleRequestStruct();
    void handleOldHistory(QString);
};




#endif // NETCLIENT_H
