#include "image_receiver.h"

//#include <Windows.h>
//-----------------------------------------------------------------------------

ImageReceiver imageReceiver;

//-----------------------------------------------------------------------------

ImageReceiver::ImageReceiver(void)
{
	//imageId = -1
	startTime = 0; 
	lastTime = -1;
}

//-----------------------------------------------------------------------------

void ImageReceiver::Get(QString address, unsigned short port)	
{
	socket = new QTcpSocket(this);			
	connect(socket, SIGNAL(readyRead()), this, SLOT(ReceiveImage()));
	socket->abort();		
	startTime = QDateTime().toTime_t ();	
	socket->connectToHost(address, port);	
}

//-----------------------------------------------------------------------------

bool ImageReceiver::IsLastImageFresh(int timeOutSeconds) 
{
	int t = (lastTime - startTime);

	return ((t >= 0) && (t < timeOutSeconds));
}

//-----------------------------------------------------------------------------

void ImageReceiver::ReceiveImage(void)
{
	char buffer[2048];
	int count = socket->read(buffer, 2048);
    //unsigned int t = timeGetTime();

	unsigned int imgExpectedSize, expectedCRC;
	memcpy(&imgExpectedSize, &(buffer[4]), 4);
	memcpy(&expectedCRC, &(buffer[8]), 4);
	
	std::vector<unsigned char> imgBuffer;	
	int received = count - 12;	
	bool error = false;

	imgBuffer.clear();
	for (int i=0; i < received; i++) {
		imgBuffer.push_back(buffer[i+12]);
	}

	while (count > 0) { //blockCount < expectedBlocks) {
		socket->waitForReadyRead(5000);
		count = socket->read(buffer, 2048);
		if (count > 0) { 
			for (int i=0; i < count; i++) {
				imgBuffer.push_back(buffer[i]);
			}
			received += count;
			if (received == imgExpectedSize) {
				break;
			}
		} else {
			error = (received != imgExpectedSize);
			break;
		}
	}

	qDebug() << " Fim ";
	if (error == false) {
		unsigned int crc = 0;
			for (int i=0; i < imgBuffer.size(); i++) {
			crc += imgBuffer[i];
		}
		if (crc == expectedCRC) {
			//cv::Mat mat;
			qDebug() << " decoding";
			mutex.lock();	
			cv::imdecode(imgBuffer, CV_LOAD_IMAGE_COLOR, &cvImage);
			qImage = QImage((uchar*) cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_RGB888);
			qImage = QGLWidget::convertToGLFormat(qImage);
			lastTime = QDateTime().toTime_t () - startTime;
			mutex.unlock();	
			//unsigned t2 = timeGetTime() - t;
			cv::imshow("Teste", cvImage);
		}
	} 

	socket->disconnect();
}
