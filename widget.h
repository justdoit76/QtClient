#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    Client client;
    static const int port;
    QString getActiveIP() const;


private slots:
    void onPb1();
    void onPb2();
    void onPb3();

public slots:
    void onConnect();
    void onDisconnect();
    void onRecvData(const QString&);
};
#endif // WIDGET_H
