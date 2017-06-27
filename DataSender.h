#pragma once

#include <QObject>
#include <QtNetwork>

class DataSender : public QObject
{
	Q_OBJECT

public:
	DataSender(QString ip, quint16 export,QObject *parent)
	{
		socket = new QUdpSocket();
		host = new QHostAddress();
		host->setAddress(ip);
		port = export;
	};
	~DataSender()
	{
		delete socket;
		delete host;
	};
	int send(const char *p, int len)
	{
		return socket->writeDatagram(p, (qint64)len, *host, port);
	};// send a message to receiver;

private:
	QUdpSocket *socket;
	QHostAddress *host;
	quint16 port;
};