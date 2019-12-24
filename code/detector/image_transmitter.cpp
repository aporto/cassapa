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

#include "image_transmitter.h"

#include <Windows.h>
//-----------------------------------------------------------------------------

//ImageTransmitter imageTransmitter;

//-----------------------------------------------------------------------------

ImageTransmitter::ImageTransmitter(QObject *parent) :
	QObject(parent)
{
	image = NULL;
	//imageId = -1;
	//occupied = false;		
}

//-----------------------------------------------------------------------------

void ImageTransmitter::Initialize(cv::Mat * cvImage)
{
	image = cvImage;

	server = new QTcpServer(this);
	
	//QHostAddress addr = QHostAddress("127.0.0.1"); //QHostAddress::Any;
	//listen(addr, 30777);
	int loport = config.serverTCPPort;//30777;

	connect(server, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketError(QAbstractSocket::SocketError)));

	bool ok = server->listen(QHostAddress::Any, loport);
	//server->listen(addr, loport);
	
	connect(server, SIGNAL(newConnection()), this, SLOT(SendImage()));
	//qDebug() << " Listening ";
	
	//remoteAddress = address;
	//remotePort = port;
	//image = const_cast <cv::Mat *> (Image);
	//imageId = id;

	//return true;
}

//-----------------------------------------------------------------------------

void ImageTransmitter::SocketError(QAbstractSocket::SocketError socketError)
{
	//QDebug::qDebug << "Error";
	int err = socketError;
	static int t = 0;
	t++;
}

//-----------------------------------------------------------------------------


void ImageTransmitter::SendImage(void)
{
	++(config.output.imageRequestCount);
	if (image == NULL) {
		return;
	}

	if ( (image->cols == 0) || (image->rows == 0)) {
		return;
	}
	//qDebug() << " enviando ";

	QTcpSocket *clientConnection = server->nextPendingConnection();
	
	std::vector <unsigned char> buffer;
	std::vector <int> params;
	mutex.lock();	
	bool ok = cv::imencode(".jpg", *image, buffer, params);
	mutex.unlock();

	QByteArray qbuffer;
	//QDataStream out (); //qbuffer);
	unsigned int sz = buffer.size();
	unsigned char ptrSz[4];
	memcpy(ptrSz, &sz, 4);
	//out << int(0);              // placeholder for info about bytes for the binary data
	for (int i=0; i < 4; i++) {
		qbuffer.push_back(ptrSz[i]);
	}
	for (int i=0; i < buffer.size(); i++) {
		qbuffer.push_back(buffer[i]);
	}
	
	//out.device()->seek(0); // go back to start
	//out << buffer.size();    // info about bytes for the size value (int) and binary image data (image)

	int count = clientConnection->write(qbuffer);// < buffer.size()
	if (count < 0) {
		QString error = clientConnection->errorString();
		qDebug() << error;
	}

	++(config.output.imageTransmitCount);
}
/*
 


	
	char txBuffer[2048];
	unsigned int * command = (unsigned int *)&(txBuffer[0]);
	*command = 3000;
	unsigned int * len = (unsigned int *)&(txBuffer[4]);
	*len = buffer.size(); 		
	unsigned int * crc = (unsigned int *)&(txBuffer[8]);
	*crc = 0;
	for (int i=0; i < buffer.size(); i++) {
		*crc += buffer[i];
	}

	unsigned int expectedToSend = buffer.size();
	char * data = (char *)&(buffer[0]);	 
	memcpy(&(txBuffer[12]), data, 2036);
	int sent = clientConnection->write(txBuffer, 2048) - 12;
	clientConnection->flush();

	while (sent < expectedToSend) {
		int size = expectedToSend - sent;
		if (size > 2048) {
			size = 2048;
		}
		
		char * data = (char *)&(buffer[sent]);	 
		memcpy(txBuffer, data, size);

		int count = clientConnection->write(txBuffer, size);
		clientConnection->flush();
		SleepEx(200, true);
		if (count != size) {
			break;
		}
		sent += count;		
	}
	
	
	//clientConnection->disconnectFromHost();

	//delete clientConnection;

//	image = NULL;
}*/

//-----------------------------------------------------------------------------

/*bool ImageTransmitter::ScheduleTransmission(cv::Mat const * Image, int id, QString address, unsigned short port)
{
	server = new QTcpServer(this);
	
	QHostAddress addr = QHostAddress("127.0.0.1"); //QHostAddress::Any;
	//listen(addr, 30777);
	int loport = 30777;
	server->listen(QHostAddress::Any, loport);
	qDebug() << " Listening ";
	
	remoteAddress = address;
	remotePort = port;
	image = const_cast <cv::Mat *> (Image);
	imageId = id;

	return true;
}
*/
//-----------------------------------------------------------------------------

/*void ImageTransmitter::ProcessSchedule(void)
{
	qDebug() << " connect_qt ";
	connect(server, SIGNAL(newConnection()), this, SLOT(SendImage()));
	if (occupied) {
		return;
	} 
	if (image != NULL) {
		occupied = true;
		//start();
		//QFuture<void> result = QtConcurrent::run ( LongFunction );
	}
}*/

//-----------------------------------------------------------------------------

/*void ImageTransmitter::run()
{
	//QUdpSocket socket;
	/*QUdpSocket socket;
	socket.bind(0);
	std::vector <unsigned char> buffer;
	std::vector <int> params;
	bool ok = cv::imencode(".jpg", *image, buffer, params);
	unsigned int crc = 0;
	for (int i=0; i < buffer.size(); i++) {
		crc += buffer[i];
	}
	
	//unsigned char header[8];
	
	//for (int i=0; i < 8; i++) {
		//buffer.insert(buffer.begin(), header[7-i]);
	//}

	int sent = 0;
	char txBuffer[2048];
	unsigned int * command = (unsigned int *)&(txBuffer[0]);
	unsigned int * len = (unsigned int *)&(txBuffer[4]);
	*command = 3000;
	*len = buffer.size(); 	
	while (sent < buffer.size()) {
		int size = buffer.size() - sent;
		if (size > 2040) {
			size = 2040;
		}
		char * data = (char *)&(buffer[sent]);	 
		memcpy(&(txBuffer[8]), data, size);
		int count = socket.writeDatagram(txBuffer, size + 8, QHostAddress(remoteAddress), remotePort) - 8;
		if (count != size) {
			break;
		}
		sent += count;
		++(*command);

		while (socket.hasPendingDatagrams() == false) {			
			SleepEx(1, true);
		}
		while (socket.hasPendingDatagrams()) {			
			count = socket.readDatagram(txBuffer, 2048);		
		}
	}

	/*QBuffer buffer;
	image.save(&buffer, "PNG", 100); //can change the compression level to suit the application - see http://qt-project.org/doc/qt-4.8/qimage.html#save
qint64 length = sizeof(quint32) + buffer.data().size(); //http://doc.qt.digia.com/4.7/datastreamformat.html
stream << length;
stream << buffer.data();*/

	/*image = NULL;
	occupied = false;*/
//}

//-----------------------------------------------------------------------------

/*void ImageTransmitter::StartTransmission(cv::Mat &Image)
{
	if (
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
}

//-----------------------------------------------------------------------------

bool SocketServer::Start(int bindPort)
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

bool SocketServer::Stop(void)
{
	if (thread.isRunning()) {
		socket->close();

		return true;
	} else {
		return false;
	}
}

//-----------------------------------------------------------------------------
*/