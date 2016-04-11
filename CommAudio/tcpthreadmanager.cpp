#include "tcpthreadmanager.h"

void TCPThreadManager::TCPReceiveThread() {

    clientTCP.WSAEvent = WSACreateEvent();
    if(clientTCP.WSAEvent == WSA_INVALID_EVENT) {
        qDebug() << "Failed to create event";
        exit(1);
    }
    QFile file;
    while(1)
    {
        while(clientTCP.Recv()) {
            QByteArray data(clientTCP.SocketInfo.Buffer);
            if(data.startsWith(UpdateList)) {
                emit songList(data.remove(0, 7));
            }else if(data == QString(FileBegin)) {
                file.setFileName(songName);
                file.open(QIODevice::WriteOnly);
            }else if(data == QString(FileEnd)) {
                file.close();
            } else {
                QDataStream output(&file);
                output.writeRawData(clientTCP.SocketInfo.Buffer, clientTCP.SocketInfo.BytesRECV);
            }
        }
        qDebug() << "Exiting thread";
        break;
    }

}


void TCPThreadManager::TCPThreadRequest() {
    clientTCP.InitializeSocket(TCP_PORT, ipAddr.data());
    emit TCPThreadRequested();
}

void TCPThreadManager::sendSongRequest(QByteArray songName) {
    this->songName = songName;
    clientTCP.Send(songName.data(), songName.size() + 1);
}
