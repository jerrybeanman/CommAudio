#include "tcpthreadmanager.h"

DWORD WINAPI AcceptThread(LPVOID lpParam);

void TCPThreadManager::TCPReceiveThread() {

    clientTCP.InitializeSocket(TCP_PORT, ipAddr.data());
    clientTCP.Send("1", 2);

    WSAEVENT EventArray[1];
    DWORD index;
    clientTCP.WSAEvent = WSACreateEvent();
    if(clientTCP.WSAEvent == WSA_INVALID_EVENT) {
        qDebug() << "Failed to create event";
        exit(1);
    }
    EventArray[0] = clientTCP.WSAEvent;
    bool sendFile = true;
    QFile file;
    QByteArray test("2 02_-_I_Will_Beat_On_Your_Behind.wav");
    while(1)
    {
        clientTCP.Recv();
        while(clientTCP.SocketInfo.BytesRECV != 0) {
            std::cout << "Recieved message: " << clientTCP.SocketInfo.DataBuf.buf <<
                         "Size: " << clientTCP.SocketInfo.BytesRECV << std::endl;
            if(sendFile == true) {
                clientTCP.Send(test.data(), test.size() + 1);
                sendFile = false;
            }
            clientTCP.Recv();
            QByteArray data(clientTCP.SocketInfo.DataBuf.buf);
            if(data == QString("FileS:")) {
                file.setFileName(test);
                file.open(QIODevice::WriteOnly);
            }else if(data == QString("FileE:")) {
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
