#include <QtCore/QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    
    QCoreApplication app(argc,argv);
    
    Server mserver(master);
    mserver.startServer();
    
    return app.exec();
}
