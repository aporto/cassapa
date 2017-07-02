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

#include "image_receiver.h"

//#define NOMINMAX
//#include <Windows.h>
//-----------------------------------------------------------------------------

ImageReceiver imageReceiver;

//-----------------------------------------------------------------------------

ImageReceiver::ImageReceiver(void)
{
	//imageId = -1
	lastTime = 0;
	glGenTextures(1, texture);
}

//-----------------------------------------------------------------------------

void ImageReceiver::Initialize(void)
{
	socket = new QTcpSocket(this);	
	//connect(socket, SIGNAL(readyRead()), this, SLOT(ReceiveImage()));
	//connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketError(QAbstractSocket::SocketError)));	
}

//-----------------------------------------------------------------------------

void ImageReceiver::Get(QString address, unsigned short port)	
{	
	socket->abort();		
	connect(socket, SIGNAL(readyRead()), this, SLOT(ReceiveImage()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketError(QAbstractSocket::SocketError)));
	socket->connectToHost(address, port);	
}

//-----------------------------------------------------------------------------

bool ImageReceiver::IsLastImageFresh(int timeOutSeconds) 
{
	qint64 time =  QDateTime().currentMSecsSinceEpoch();
	qint64 delta = time - lastTime;	
	return (delta <= timeOutSeconds);
}

//-----------------------------------------------------------------------------

void ImageReceiver::SocketError(QAbstractSocket::SocketError socketError)
{
	//QDebug::qDebug << "Error";
	int err = socketError;
	static int t = 0;
	t++;
}

//-----------------------------------------------------------------------------

void ImageReceiver::ReceiveImage(void)
{
	unsigned int msgSize = -1;
	QDataStream in(socket);

	if ( (socket->bytesAvailable() > 0) && (msgSize = -1) ) {
		socket->read((char *)&msgSize, 4);	
	}
 
	while (socket->bytesAvailable() < msgSize) {
		if (!socket->waitForReadyRead(100)) {
			socket->disconnectFromHost();
			return;
			break;
		}
	}

	QByteArray qbuffer;
	qbuffer = socket->read(msgSize);
	socket->disconnectFromHost();

	int imgSize = qbuffer.size();

	if (imgSize != msgSize) {
		return;
	}

	std::vector<unsigned char> imgBuffer;	
	imgBuffer.clear();
	for (int i=0; i < imgSize; i++) {
		imgBuffer.push_back(qbuffer[i]);
	}

	mutex.lock();	
	cv::imdecode(imgBuffer, CV_LOAD_IMAGE_COLOR, &cvImage);
	qImage = QImage((uchar*) cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_RGB888);
	if (qImage.isNull() == false) {
		qImage = QGLWidget::convertToGLFormat(qImage);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, qImage.width(), qImage.height(),
					0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits() );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 	
		lastTime = QDateTime().currentMSecsSinceEpoch();
	}

	mutex.unlock();		
}

//-----------------------------------------------------------------------------

