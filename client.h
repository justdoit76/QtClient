#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
class Widget;

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    Client(Widget*);
    virtual ~Client();

public:
    void connectServer(const QString&, int);
    void disconnectServer();
    void sendData(const QString &);

private:
    Widget* parent;

signals:
    void connSignal();
    void disconnSignal();
    void recvSignal(const QString&);

protected slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
};

#endif // CLIENT_H
