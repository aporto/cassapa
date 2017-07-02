//-----------------------------------------------------------------------------

#ifndef IMAGE_RECEIVER_H
#define IMAGE_RECEIVER_H

//-----------------------------------------------------------------------------

#include <QObject>
#include <Qmutex>
#include <QTcpSocket>
//#include <qtcpserver.h>
//#include <QFuture>
//#include <QtConcurrentRun>
#include <QBuffer>
#include <QWidget>
#include <QGLWidget>
#include <QDateTime>
//#include <qthread.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <vector>

//-----------------------------------------------------------------------------

class ImageReceiver : public QObject
{
	Q_OBJECT

public:
	QMutex mutex;
	QImage qImage;
	cv::Mat cvImage;
	GLuint texture[1];	

	ImageReceiver(void);
	void Initialize(void);
	bool IsLastImageFresh(int timeOutSeconds);
	void Get(QString address, unsigned short port);	
	
private slots:
	void ReceiveImage(void);
	void SocketError(QAbstractSocket::SocketError socketError);

private:		
	QTcpSocket * socket;
	qint64 lastTime;
	//unsigned int startTime, lastTime;
};

//-----------------------------------------------------------------------------

extern ImageReceiver imageReceiver;

//-----------------------------------------------------------------------------
#endif 
//-----------------------------------------------------------------------------