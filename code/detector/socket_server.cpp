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

#include "socket_server.h"

//-----------------------------------------------------------------------------

SocketServer::SocketServer(QObject *parent, bool isSERVER_or_client = true) :
	QObject(parent)
{
	//rxImage = NULL;
	socket = new QUdpSocket(this);	
	connect(socket, SIGNAL(readyRead()), this, SLOT(SlotCheckForData()));
	localHostString = "< NOT OPEN > ";
	
	if (isSERVER_or_client) {
		localAddress = "127.0.0.1";
		localPort = 32667;	

		remoteAddress = "127.0.0.1";		
		remotePort = 0;//32666;
	} else {		
		localAddress = "127.0.0.1";
		localPort = 32666;			
				
		remoteAddress = "127.0.0.1";
		remotePort = 32667;
	}
}

//-----------------------------------------------------------------------------

SocketServer::~SocketServer()
{
	delete socket;
}

//-----------------------------------------------------------------------------

QString SocketServer::GetRemoteHost(void)
{
	return QString().number(clientMap.size());
	/*if (remotePort == 0) {
		return "None";
	} else {
		return remoteAddress + ":" + QString::number(remotePort);
	}*/
}

//-----------------------------------------------------------------------------

QString SocketServer::GetServerAddress(void)
{
	return localHostString;	
}

//-----------------------------------------------------------------------------

QString SocketServer::GetRemoteAddress(void)
{
	return remoteAddress;	
}

//-----------------------------------------------------------------------------

void SocketServer::SlotCheckForData(void)
{
	QByteArray datagram;
	datagram.resize(socket->pendingDatagramSize());
	QHostAddress sender;
	quint16 senderPort;

	socket->readDatagram(datagram.data(), datagram.size(),
									&sender, &senderPort);
	unsigned int * command = (unsigned int * ) (datagram.data());	

	if (*command == CASSAPA_SOCKET_COMMAND_SEND_CONFIG) {
		if (datagram.size() == sizeof(*poolInput)) {
			//remoteAddress = sender.toString();
			//remotePort = senderPort;
			socketMutex.lock();
			char * data = datagram.data();
			memcpy(poolInput, data, sizeof(*poolInput));			
			//
			// end of relay arguments
			
			socketMutex.unlock();
		}
	} else if (*command == CASSAPA_SOCKET_COMMAND_SEND_CALCULATIONS) {
		if (datagram.size() == sizeof(*poolOutput)) {
			remoteAddress = sender.toString();
			remotePort = senderPort;
			socketMutex.lock();			
			char * data = datagram.data();
			memcpy(poolOutput, data, sizeof(*poolOutput));
			poolOutput->rxTimeTick = QDateTime().currentMSecsSinceEpoch();
			socketMutex.unlock();
		}
	} else if ( ( (*command == CASSAPA_SOCKET_COMMAND_PING) || (*command == CASSAPA_SOCKET_COMMAND_PING_WITH_CONFIG)) 
		|| (*command == CASSAPA_SOCKET_COMMAND_PING_PROJECTOR))
	{
		//QString addr = sender.toString() + ":" + QString().number(senderPort);
		std::map <QString, ClientDef>::iterator it;
		socketMutex.lock();
		QString sock = sender.toString() + ":" + QString().number(senderPort);
		it = clientMap.find(sock);
		if (it == clientMap.end()) {
			ClientDef client;		
			client.port = senderPort;
			client.addr = sender.toString();
			client.isProjector = (*command == CASSAPA_SOCKET_COMMAND_PING_PROJECTOR);
			client.lastTime = QDateTime().currentMSecsSinceEpoch();
			clientMap.insert(std::pair <QString, ClientDef> (sock, client));
		} else {
			qint64 t = QDateTime().currentMSecsSinceEpoch();
			int dt = t - it->second.lastTime;
			it->second.lastTime = t;
		}

		if (*command == CASSAPA_SOCKET_COMMAND_PING_WITH_CONFIG) {
			if (datagram.size() == sizeof(*poolInput)) {
				char * data = datagram.data();
				memcpy(poolInput, data, sizeof(*poolInput));
			}		
		}
		socketMutex.unlock();
		remoteAddress = sender.toString();
		remotePort = senderPort;		
	}	
}

//-----------------------------------------------------------------------------

bool SocketServer::Start(int bindPort, PoolInput * input, PoolOutput * output)//, QImage * image)
{	
	poolOutput = output;
	poolInput = input;

	localPort = bindPort;
	localHostString = "< NOT OPEN > ";

	if (socket->bind(localPort) == false) { 
		return false;
	}

	localHostString = QString().number(socket->localPort());
	
	return true;
}

//-----------------------------------------------------------------------------

bool SocketServer::SendData(const char * data, int size, QString address, int port) 
{
	int count = socket->writeDatagram(data, size, QHostAddress(address), port);
	if (count == size) {
		return true;
	} else {
		return false;
	}
}

//-----------------------------------------------------------------------------

void SocketServer::SendCalculations(void)
{
	// Envia os calculos a todos os clientes, e aproveita para limpar os clientes que nao
	// deram ping nos ultimos segundos
	qint64 time = QDateTime().currentMSecsSinceEpoch();
	std::map <QString, ClientDef>::const_iterator it = clientMap.begin();
	
	// identifica se tem algum projetor entre os clientes
	poolOutput->hasProjector = 0;
	for (it = clientMap.begin(); it != clientMap.end(); it++) {
		if (it->second.isProjector) {
			poolOutput->hasProjector = 1;
			break;
		}
	}

	it = clientMap.begin();
	while (it != clientMap.end()) {
		int dt = (time - it->second.lastTime);
		if (dt > 2000) {
			it = clientMap.erase(it);
		} else {
			poolOutput->command = CASSAPA_SOCKET_COMMAND_SEND_CALCULATIONS;
			SendData((char*) poolOutput, sizeof(*poolOutput), it->second.addr, it->second.port);//remoteAddress, remotePort);
			it++;
		}
	}	
}

//-----------------------------------------------------------------------------

void SocketServer::SendConfig(void) 
{
	poolInput->command = CASSAPA_SOCKET_COMMAND_SEND_CONFIG;
	SendData((char*) poolInput, sizeof(*poolInput), remoteAddress, remotePort);
}

//-----------------------------------------------------------------------------

void SocketServer::PingAll(int port, bool withConfig, bool isProjector)
{
	if (withConfig) {
		if (isProjector) {
			poolInput->command = CASSAPA_SOCKET_COMMAND_PING_PROJECTOR;
		} else{
			poolInput->command = CASSAPA_SOCKET_COMMAND_PING_WITH_CONFIG;
		}		
		SendData((char*) poolInput, sizeof(*poolInput), "255.255.255.255", port);
		//SendData((char*) poolInput, sizeof(*poolInput), "192.168.1.52", port);
	} else {
		SocketServerSimpleMessage msg;
		if (isProjector) {
		msg.command = CASSAPA_SOCKET_COMMAND_PING_PROJECTOR;
	} else{
		msg.command = CASSAPA_SOCKET_COMMAND_PING;
	}
		SendData((char*) &msg, sizeof(SocketServerSimpleMessage), "255.255.255.255", port);	
	}	
}

//-----------------------------------------------------------------------------

void SocketServer::PingServer(int port, bool isProjector)
{

	//poolInput->command = CASSAPA_SOCKET_COMMAND_PING;
	//SendData((char*) poolInput, sizeof(*poolInput), remoteAddress, port);
	SocketServerSimpleMessage msg;
	if (isProjector) {
		msg.command = CASSAPA_SOCKET_COMMAND_PING_PROJECTOR;
	} else{
		msg.command = CASSAPA_SOCKET_COMMAND_PING;
	}
	SendData((char*) &msg, sizeof(SocketServerSimpleMessage), remoteAddress, port);	
}

//-----------------------------------------------------------------------------

bool SocketServer::Stop(void)
{
	socket->close();
	return true;
}

//-----------------------------------------------------------------------------
