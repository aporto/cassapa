/*	
	Cassapa 2 
	Copyright (C) 2017 Alex Rocha Porto	

	Licensing information:	

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation,
	Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You can contact the author at alex@alexporto.com

*/

//-----------------------------------------------------------------------------

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

//-----------------------------------------------------------------------------

#include <QObject>
#include <QUdpSocket>
#include <QMutex>
#include <QImage>
#include <QBuffer>
#include <qdatetime.h>
#include <qthread.h>

#include "opencv2/imgproc/imgproc.hpp"

#include "pool_definitions.h"

#include <map>

//-----------------------------------------------------------------------------

#define CASSAPA_SOCKET_COMMAND_SEND_CONFIG			1000
#define CASSAPA_SOCKET_COMMAND_SEND_CALCULATIONS	2000
#define CASSAPA_SOCKET_COMMAND_PING					3000
#define CASSAPA_SOCKET_COMMAND_PING_WITH_CONFIG		3001
#define CASSAPA_SOCKET_COMMAND_PING_PROJECTOR		3002

//-----------------------------------------------------------------------------

class SocketThread : public QThread
{
	Q_OBJECT

signals:
	 void SignalCheckForData(void);

private:
	void run();
};

struct ClientDef {
	int port;
	QString addr;
	qint64 lastTime;
	bool isProjector;
};

#pragma pack(push, 1)
struct SocketServerSimpleMessage {
	unsigned int command;
	unsigned int data;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

class SocketServer : public QObject
{
	Q_OBJECT
public:
	QMutex mutex;

	SocketServer(QObject *parent, bool isSERVER_or_client);
	~SocketServer();
	bool Start(int port, PoolInput * input, PoolOutput * output);//, QImage * image);
	bool Stop(void);
	void SendCalculations(void);
	//bool SendImage(cv::Mat &image);
	void SendConfig(void);//QString address, int port);	
	void PingAll(int port, bool withConfig, bool isProjector);//QString address, int port);	
	void PingServer(int port, bool isProjector);//QString address, int port);	
	QString GetRemoteHost(void);	
	QString GetServerAddress(void);
	QString GetRemoteAddress(void);

public slots:
	void SlotCheckForData(void);

private:
	QUdpSocket *socket;
	//QImage * rxImage;
	int port;
	std::map<QString, ClientDef> clientMap;
	//SocketThread thread;
	QString localAddress;
	int localPort;	
	QString remoteAddress;
	QString localHostString;
	PoolOutput * poolOutput;
	PoolInput * poolInput;
	int remotePort;	
	unsigned int GetCounter;
	bool SendData(const char * data, int size, QString address, int port);
};

//-----------------------------------------------------------------------------
#endif 
//-----------------------------------------------------------------------------