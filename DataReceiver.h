#pragma once

#include <QObject>
#include <QtNetwork>
#include <string>


class DataReceiver : public QObject
{
	Q_OBJECT

public slots:
	void read()
	{
		QByteArray data;
		data.resize(socket->pendingDatagramSize());
		socket->readDatagram(data.data(), data.size());
		std::string str(data.data(), data.size());
		emit dataReady(str);
	};

public:
	DataReceiver(QString ip, quint16 po,QObject *parent)
	{
		socket = new QUdpSocket();
		host = new QHostAddress(ip);//new QHostAddress("127.0.0.1");
		port = po;//6666;
		socket->bind(QHostAddress::Any, port);
		connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
	};
	~DataReceiver()
	{
		delete socket;
		delete host;
	};

signals:
	void dataReady(std::string str);//receive messages;
private:
	QUdpSocket *socket;
	QHostAddress *host;
	quint16 port;
};