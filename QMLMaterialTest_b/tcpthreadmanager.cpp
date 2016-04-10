#include "tcpthreadmanager.h"

void TCPThreadManager::TCPReceiveThread() {

    clientTCP.InitializeSocket(TCP_PORT, ipAddr.data());
    clientTCP.Send("1", 2);

    clientTCP.WSAEvent = WSACreateEvent();
    if(clientTCP.WSAEvent == WSA_INVALID_EVENT) {
        qDebug() << "Failed to create event";
        exit(1);
    }
    QFile file;
    while(1)
    {
        while(clientTCP.Recv()) {
            QByteArray data(clientTCP.SocketInfo.DataBuf.buf);
            if(data[0] == UpdateList) {
                emit songList(data);
            }else if(data == QString(FileBegin)) {
                file.setFileName(songName);
                file.open(QIODevice::WriteOnly);
            }else if(data == QString(FileEnd)) {
                file.close();
            } else {
                QDataStream output(&file);
                output.writeRawData(clientTCP.SocketInfo.DataBuf.buf, clientTCP.SocketInfo.BytesRECV);
            }
        }
    }

}


void TCPThreadManager::TCPThreadRequest() {
    emit TCPThreadRequested();
}

void TCPThreadManager::sendSongRequest(QByteArray& songName) {
    this->songName = songName;
    clientTCP.Send(songName.data(), songName.size() + 1);
}
