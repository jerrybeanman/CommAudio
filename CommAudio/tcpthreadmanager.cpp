#include "tcpthreadmanager.h"

CircularBuffer cbControl;
void TCPThreadManager::TCPReceiveThread() {

    clientTCP.WSAEvent = WSACreateEvent();
    if(clientTCP.WSAEvent == WSA_INVALID_EVENT) {
        qDebug() << "Failed to create event";
        exit(1);
    }
    QFile file;
    CBInitialize(&cbControl, 10, DATA_BUFSIZE);

    while(1)
    {
        while(clientTCP.Recv()) {
            QByteArray data(clientTCP.SocketInfo.Buffer);
            if(data.startsWith(Header)) {
                CBPushBack(&cbControl, clientTCP.SocketInfo.Buffer + 7);
                emit songHeader();
            }else if(data.startsWith(UpdateList)) {
                QList<QByteArray> uiInfo = data.split('%');
                if(uiInfo.size() == 2){
                    emit songList(uiInfo[0].remove(0, 7));
                    emit songNameReceived(uiInfo[1].remove(0, 6));
                } else {
                    emit songList(data.remove(0, 7));
                }
            }else if(data == QString(FileBegin)) {
                file.setFileName(songName);
                file.open(QIODevice::WriteOnly);
            }else if(data.contains(FileEnd)) {
                file.close();
            } else {
                QDataStream output(&file);
                output.writeRawData(clientTCP.SocketInfo.Buffer, clientTCP.SocketInfo.BytesRECV);
            }
        }
        qDebug() << "Exiting thread";
        break;
    }

    CBFree(&cbControl);

}


void TCPThreadManager::TCPThreadRequest() {
    clientTCP.InitializeSocket(TCP_PORT, ipAddr.data());
    emit TCPThreadRequested();
}

void TCPThreadManager::sendSongRequest(QByteArray songName) {
    if(songName[0] == '2') {
        QByteArray temp = songName;
        this->songName = temp.remove(0, 2);
    }
    clientTCP.Send(songName.data(), songName.size() + 1);
}
