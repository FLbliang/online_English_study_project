#include "socketcontroller.h"

SocketController::SocketController(QObject *parent) : QObject(parent)
{
    this->socket = new QTcpSocket(this);
    this->server_ip = new QHostAddress();
    this->server_ip->setAddress("123.207.58.66");
    this->recv_flag = false;

    int port = 8686;

    this->socket->connectToHost(*this->server_ip, port);
    connect(this->socket, &QTcpSocket::readyRead, this, &this->slot_readData);


}

SocketController::~SocketController(){
    this->socket->close();
    delete this->socket;
    delete this->server_ip;
}

bool SocketController::send_message(const QString &send_data){

    this->send_data = send_data;
    this->socket->write(this->send_data.toUtf8(), 2*this->send_data.length());
  // qDebug() << "test_data: " << this->send_data;
   // qDebug() << "send all right";
    return true;
}

void SocketController::slot_readData(){

    this->recv_data = "";
    this->recv_flag = false;
    //qDebug() << "ready read";
    while(this->socket->bytesAvailable() > 0){

        QByteArray datagram;
        datagram.resize(this->socket->bytesAvailable());
        this->socket->read(datagram.data(), datagram.size());
        this->recv_data += datagram.data();
    }
    this->recv_flag = true;
    emit this->signal_hadRead();
}

bool SocketController::recv_message(){

    return this->recv_flag;
}

void SocketController::set_recvData(const QString &recv_data){

    this->recv_data = recv_data;
}

void SocketController::set_sendData(const QString &send_data){

    this->send_data = send_data;
}


QString SocketController::get_recvData(){
    return this->recv_data;
}


QString SocketController::get_sendData(){
    return this->send_data;
}














