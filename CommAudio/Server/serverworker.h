#ifndef SERVERWORKER_H
#define SERVERWORKER_H
#include "Server/ServerUDP.h"
#include "globals.h"

class ServerWorker : public QObject
{
    Q_OBJECT
    QThread workerThread;

public slots:
    void StartUDP(const QString &parameter)
    {
        if(serverUDP.InitializeSocket(DEFAULT_PORT) < 0)
            return;

        std::cout << "Socket Created" << std::endl;


        if(serverUDP.MulticastSettings(DEAULT_MULTICAST_IP) < 0)
            return;

        while(1)
        {
            if(!serverUDP.Broadcast("Scamaz"))
            {
                std::cout << "process()::Broadcast() failed" << std::endl;
                break;
            }
        }
        emit resultReady(parameter);
    }

signals:
    void resultReady(const QString &result);

private:
    ServerUDP serverUDP;
};
#endif // SERVERWORKER_H
