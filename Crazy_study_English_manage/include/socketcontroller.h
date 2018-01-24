#ifndef SOCKETCONTROLLER_H
#define SOCKETCONTROLLER_H

#include <QObject>

#include <QString>
#include <QHostAddress>
#include <QTcpSocket>

class SocketController : public QObject
{
    Q_OBJECT
public:
    explicit SocketController(QObject *parent = 0);
    ~SocketController();

    bool send_message(const QString & send_data);
    bool recv_message();
    void set_recvData(const QString & recv_data);
    void set_sendData(const QString & send_data);
    QString get_recvData();
    QString get_sendData();

private:
    QString recv_data;
    QString send_data;
    QTcpSocket * socket;
    QHostAddress * server_ip;

    bool recv_flag;
signals:
    void signal_hadRead();

public slots:
    void slot_readData();
};

#endif // SOCKETCONTROLLER_H
