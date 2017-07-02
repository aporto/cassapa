//-----------------------------------------------------------------------------

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

//-----------------------------------------------------------------------------

#include <QObject>
#include <QUdpSocket>
#include <qthread.h>

#include "config_viewer.h"

//-----------------------------------------------------------------------------

class SocketThread : public QThread
{
	Q_OBJECT

signals:
	 void SignalCheckForData(void);

private:
	void run();
};

//-----------------------------------------------------------------------------

class SocketViewer : public QObject
{
	Q_OBJECT
public:
	explicit SocketViewer(QObject *parent = 0);
	~SocketViewer();
	bool Start(int port);
	bool Stop(void);
	void SendConfig(void);
	bool SendData(const char * data, int size, QString address, int port);

public slots:
	void SlotCheckForData(void);

private:
	QUdpSocket *socket;
	int port;
	SocketThread thread;
	QString serverAddress;
	int serverPort;	
	//QImage image;
};

//-----------------------------------------------------------------------------
#endif 
//-----------------------------------------------------------------------------