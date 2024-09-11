#include <QDebug>
#include "client.h"
#include "widget.h"

Client::Client(Widget* p) : parent(p)
{
    // 시그널 연결
    connect(this, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(this, &QTcpSocket::connected, this, &Client::onConnected);
    connect(this, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &Client::onError);

    connect(this, &Client::connSignal, p, &Widget::onConnect);
    connect(this, &Client::disconnSignal, p, &Widget::onDisconnect);
    connect(this, &Client::recvSignal, p, &Widget::onRecvData);
}

Client::~Client()
{
    disconnectServer();
}

void Client::connectServer(const QString& ip, int port)
{
    // 연결시도
    connectToHost(ip, port);
}

void Client::disconnectServer()
{
    //비동기적 종료 (보내던 데이터가 있다면 보내고 종료)
    disconnectFromHost();
}

void Client::sendData(const QString &msg)
{
    if(isValid())
        write(msg.toStdString().c_str());
}

void Client::onReadyRead()
{
    QByteArray data = readAll();
    qDebug() << "[received data]\n" << data;
    emit recvSignal(QString::fromUtf8(data));
}

void Client::onConnected()
{
    qDebug() << "Connected to server!";
    emit connSignal();

}

void Client::onDisconnected()
{
    qDebug() << "Disconnected from server!";
    disconnectServer();
    close();
    emit disconnSignal();
}

void Client::onError(SocketError e)
{
    qDebug() << "Socket error:" << e;

    // 에러 발생 시 처리 로직
    if (e == QAbstractSocket::HostNotFoundError)
    {
        qDebug() << "Host not found. Check the server address.";
        emit disconnSignal();
    }
    else if (e == QAbstractSocket::ConnectionRefusedError)
    {
        qDebug() << "Connection refused by the server.";
        emit disconnSignal();
    }
    else if (e == QAbstractSocket::RemoteHostClosedError)
    {
        qDebug() << "The server closed the connection.";
        emit disconnSignal();
    }
}
