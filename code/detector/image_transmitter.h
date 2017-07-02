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

#ifndef IMAGE_TRANSMITTER_H
#define IMAGE_TRANSMITTER_H

//-----------------------------------------------------------------------------

#include <QObject>
#include <QTcpSocket>
#include <qtcpserver.h>
#include <QMutex>
#include <qthread.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "config_detector.h"

#include <vector>

//-----------------------------------------------------------------------------

class ImageTransmitter : public QObject 
{
	Q_OBJECT

public:
	QMutex mutex;
	ImageTransmitter(QObject *parent);
	//void SetImage(cv::Mat * cvImage);
	void Initialize(cv::Mat * cvImage);
	//bool ScheduleTransmission(cv::Mat const * Image, int id, QString address, unsigned short port);	
	//v//oid ProcessSchedule(void);

private slots:
	void SendImage(void);
	void SocketError(QAbstractSocket::SocketError socketError);

private:
	cv::Mat * image;
	//int imageId;
	//bool occupied;
	//QString remoteAddress;
	//unsigned short remotePort;
	QTcpServer * server;

	//void StartTransmission(cv::Mat &Image);
};

//-----------------------------------------------------------------------------

//extern ImageTransmitter imageTransmitter;

//-----------------------------------------------------------------------------
#endif 
//-----------------------------------------------------------------------------