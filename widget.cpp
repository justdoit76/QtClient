#include "widget.h"
#include "ui_widget.h"
#include <QNetworkInterface>

const int Widget::port = 5614;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), client(this)
{
    ui->setupUi(this);
    setWindowTitle("Client (Ocean Coding School)");
    ui->pb1->setCheckable(true);

    // ip, port 기본값 설정
    QString ip = getActiveIP();
    ui->le1->setText(ip);
    ui->le2->setText(QString::number(port));

    // signal, slot 연결
    connect(ui->pb1, &QPushButton::clicked, this, &Widget::onPb1);
    connect(ui->pb2, &QPushButton::clicked, this, &Widget::onPb2);
    connect(ui->pb3, &QPushButton::clicked, this, &Widget::onPb3);
}

Widget::~Widget()
{
    delete ui;
    client.disconnectServer();
}

QString Widget::getActiveIP() const
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &interface : interfaces) {
        // 네트워크 인터페이스가 UP 상태인지 확인
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack) &&
            !interface.flags().testFlag(QNetworkInterface::IsPointToPoint)) {

            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    return entry.ip().toString();
                }
            }
        }
    }

    return QString();
}


void Widget::onPb1()
{
    if(ui->pb1->isChecked())
    {
        ui->pb1->setText("disconnect");
        // 현재 pc ip 얻기
        QString ip = getActiveIP();
        qDebug() << "IP:" << ip;
        client.connectServer(ip, port);
    }
    else
    {
        ui->pb1->setText("connect");
        client.disconnectServer();
    }
}

void Widget::onPb2()
{
    ui->lw->clear();
}

void Widget::onPb3()
{
    QString msg = ui->le3->text();
    client.sendData(msg);
    ui->le3->clear();
}

void Widget::onConnect()
{
    ui->pb1->setChecked(true);
    ui->pb1->setText("disconnect");
}

void Widget::onDisconnect()
{
    ui->pb1->setChecked(false);
    ui->pb1->setText("connect");
}

void Widget::onRecvData(const QString& msg)
{
    ui->lw->addItem(msg);
    int n = ui->lw->count();
    ui->lw->setCurrentRow(n-1);
}
