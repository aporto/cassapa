//-----------------------------------------------------------------------------

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

//-----------------------------------------------------------------------------

#include <QObject>
#include <QUdpSocket>
#include <qthread.h>

#include "config_detector.h"

//-----------------------------------------------------------------------------

class SocketServer : public QThread
{
    Q_OBJECT

signals:
	 //void SignalCheckForData(void);
public:
	//void * caller;
	//explicit SocketServer(QObject *parent = 0);
	SocketServer();
	~SocketServer();
	bool StartListening(int port);
	bool StopListening(void);
	void SendCalculations(void);
	bool SendData(const char * data, int size, QString address, int port);
private:
    void run();
	void SlotCheckForData(void);

	QUdpSocket *socket;
	int port;
	//SocketThread thread;
	QString clientAddress;
	int clientPort;	
};

//-----------------------------------------------------------------------------

/*class SocketServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketServer(QObject *parent = 0);
	~SocketServer();
	bool Start(int port);
	bool Stop(void);
	void SendCalculations(void);
	bool SendData(const char * data, int size, QString address, int port);

public slots:
	void SlotCheckForData(void);

private:
    QUdpSocket *socket;
	int port;
	SocketThread thread;
	QString clientAddress;
	int clientPort;	
};*/

//-----------------------------------------------------------------------------
#endif 
//-----------------------------------------------------------------------------