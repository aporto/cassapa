//-----------------------------------------------------------------------------

#ifndef IMAGE_RECEIVER_H
#define IMAGE_RECEIVER_H

//-----------------------------------------------------------------------------

#include <QObject>
#include <Qmutex>
#include <QTcpSocket>
#include <QBuffer>
#include <QWidget>
#include <QGLWidget>
#include <QDateTime>
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

	ImageReceiver(void);
	bool IsLastImageFresh(int timeOutSeconds);
	void Get(QString address, unsigned short port);	
	
private slots:
	void ReceiveImage(void);

private:
	cv::Mat cvImage;
	
	QTcpSocket * socket;

	unsigned int startTime, lastTime;
};

//-----------------------------------------------------------------------------

extern ImageReceiver imageReceiver;

//-----------------------------------------------------------------------------
#endif 
//-----------------------------------------------------------------------------