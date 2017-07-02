#include "socket_viewer.h"

//-----------------------------------------------------------------------------

SocketViewer::SocketViewer(QObject *parent) :
	QObject(parent)
{
	socket = new QUdpSocket(this);
	connect(&thread, SIGNAL(SignalCheckForData()), this, SLOT(SlotCheckForData()));
	serverAddress = "127.0.0.1";
	serverPort = 32667;
}

//-----------------------------------------------------------------------------

SocketViewer::~SocketViewer()
{
	thread.terminate();
	delete socket;
}

//-----------------------------------------------------------------------------

void SocketThread::run()
{
	emit SignalCheckForData();
	msleep(5);
}

//-----------------------------------------------------------------------------

void SocketViewer::SlotCheckForData(void)
{
	while (socket->waitForReadyRead(-1)) {
		QByteArray datagram;
		datagram.resize(socket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;

		socket->readDatagram(datagram.data(), datagram.size(),
									&sender, &senderPort);
		unsigned int * command = (unsigned int * ) (datagram.data());
		char * data = datagram.data() + 1;
		if (*command == 2000) {
			if (datagram.size() == sizeof(config.output) + 4) {
				memcpy(&(config.input), data, sizeof(config.input));
			}

		}
		//    qout << "datagram received from " << sender.toString() << endl;
	}	
}

//-----------------------------------------------------------------------------

bool SocketViewer::Start(int bindPort)
{	
	if (thread.isRunning()) {
		return false;
	}

	port = bindPort;
	//socket->connectToHost("bogotobogo.com", 80);

	if (socket->bind(port, QUdpSocket::ShareAddress) == false) {
		return false;
	}

	thread.start();

	return true;
}

//-----------------------------------------------------------------------------

bool SocketViewer::SendData(const char * data, int size, QString hostaddress, int hostPort) 
{
	//if (socket->isOpen()) {
		int count = socket->writeDatagram(data, size, QHostAddress(address), port);
		if (count == size) {
			return true;
		} else {
			return false;
		}
	//} else {
	//	return false;
	//}
}

//-----------------------------------------------------------------------------

void SocketViewer::SendConfig(void)
{
	config.output.command = 1000;
	SendData((char*) &(config.input), sizeof(config.input), serverAddress, serverPort);
}

//-----------------------------------------------------------------------------

bool SocketViewer::Stop(void)
{
	if (thread.isRunning()) {
		socket->close();

		return true;
	} else {
		return false;
	}
}

//-----------------------------------------------------------------------------
