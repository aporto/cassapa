#include "socket_server.h"

//-----------------------------------------------------------------------------

//SocketServer::SocketServer(QObject *parent) :
//	QObject(parent)
SocketServer::SocketServer()
{
	socket = new QUdpSocket(this);
	//thread.caller = this;
	//connect(&thread, SIGNAL(SignalCheckForData()), this, SLOT(SlotCheckForData()));
	//connect(&thread, &SocketThread::SignalCheckForData, this, &SocketServer::SlotCheckForData);
	clientAddress = "127.0.0.1";
	clientPort = 32666;
}

//-----------------------------------------------------------------------------

SocketServer::~SocketServer()
{
	terminate();
	delete socket;
}

//-----------------------------------------------------------------------------

void SocketServer::run()
{
	//while (isRunning()) {
	//	emit SignalCheckForData();
	//	QCoreApplication::processEvents();
	//SocketServer * socket = (SocketServer *) caller;
	while (isRunning() ) {
		while (socket->waitForReadyRead(-1)) {
			QByteArray datagram;
			datagram.resize(socket->pendingDatagramSize());
			QHostAddress sender;
			quint16 senderPort;

			socket->readDatagram(datagram.data(), datagram.size(),
										&sender, &senderPort);
			unsigned int * command = (unsigned int * ) (datagram.data());
			char * data = datagram.data() + 1;
			if (*command == 1000) {
				if (datagram.size() == sizeof(config.input) + 4) {
					clientAddress = sender.toString();
					memcpy(&(config.input), data, sizeof(config.input));
				}

			}
			//    qout << "datagram received from " << sender.toString() << endl;
		}	
		msleep(5);
	}
	//SlotCheckForData();
	//msleep(5);
	//}
}

//-----------------------------------------------------------------------------

bool SocketServer::StartListening(int bindPort)
{	
	if (isRunning()) {
		return false;
	}

	port = bindPort;
	//socket->connectToHost("bogotobogo.com", 80);

	if (socket->bind(port, QUdpSocket::ShareAddress) == false) {
		return false;
	}

	start();

	return true;
}

//-----------------------------------------------------------------------------

bool SocketServer::SendData(const char * data, int size, QString address, int port) 
{
	if (socket->isOpen()) {
		int count = socket->writeDatagram(data, size, QHostAddress(address), port);
		if (count == size) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

//-----------------------------------------------------------------------------

void SocketServer::SendCalculations(void)
{
	config.output.command = 2000;
	SendData((char*) &(config.output), sizeof(config.output), clientAddress, clientPort);
}

//-----------------------------------------------------------------------------

bool SocketServer::StopListening(void)
{
	if (isRunning()) {
		terminate();
		while (isRunning()) {
			msleep(10);
		}
		socket->close();
		
		return true;
	} else {
		return false;
	}
}

//-----------------------------------------------------------------------------
