#ifndef SERVERTHREADCONTROLLER_H
#define SERVERTHREADCONTROLLER_H
#include "globals.h"
#include "serverworker.h"

class ServerThreadController : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    ServerThreadController()
    {
        serverWorker = new ServerWorker();
        serverWorker->moveToThread(&workerThread);
        connect(&workerThread, SIGNAL(finished()), serverWorker, SLOT(deleteLater()));
        connect(this, SIGNAL(operate(QString)), serverWorker, SLOT(StartUDP()));
        workerThread.start();
    }
    ~ServerThreadController()
    {
        workerThread.quit();
        workerThread.wait();
    }
public slots:

public:
    ServerWorker * serverWorker;
signals:
    void operate(const QString &);
};

#endif // SERVERTHREADCONTROLLER_H
