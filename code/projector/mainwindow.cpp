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
#include "mainwindow.h"
//-----------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent),
	 socket(this, false)
{
	ui.setupUi(this);

	setWindowFlags ( Qt::FramelessWindowHint); 
	
	initializationError = 0;
	lostConnectionCounter = 0;

	if (socket.Start(0, &(config.input), &(config.output))) {
		socket.PingAll(config.serverUDPPort, false, true);
	} else {
		initializationError = -1;
	}

	//imgDraw = QImage (width(), height(), QImage::Format_RGB888);
	imgProjector = QImage (width(), height(), QImage::Format_RGB888);

	//imgBack = QImage("C:/projetos/opencv/cassapa_2/Win32/Debug/bob2.jpg");
	
	//cvBack = cv::imread("C:/projetos/opencv/cassapa_2/Win32/Debug/bob2.jpg");
	//cv::cvtColor(cvBack, cvBack, cv::COLOR_BGR2RGB);
	//cv::resize(cvBack, cvBack, cv::Size(640, 480));	


	//imgBack.convertToFormat(QImage::Format_RGB888);
	cvBackMask = cv::Mat (480, 640, CV_8UC3, cv::Scalar(0,0,0));
	//imgInput = cv::Mat(width(), height(), CV_8UC3);
	//imgOutput = cv::Mat(width(), height(), CV_8UC3);

	QTimer *timerGetImage = new QTimer(this);
	connect(timerGetImage, SIGNAL(timeout()), this, SLOT(GetImage()));	
	timerGetImage->start(100);

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(RefreshScreen()));
	timer->start(100);
	propX = 1;
	propY = 1;

	QDesktopWidget desktop;
	int wd = desktop.availableGeometry(desktop.primaryScreen()).width(); 
	int hd = desktop.availableGeometry(desktop.primaryScreen()).height(); 
	setGeometry(0, 0, wd, hd);
	//resize(800, 600);
	//resize(1200, 800);
	//resize(640, 480);
}

//-----------------------------------------------------------------------------

void MainWindow::resizeEvent(QResizeEvent* event)
{
	float w = ui.imgLabel->width();
	float h = ui.imgLabel->height();

	propX = float(w) / 640.0;
	propY = float(h) / 480.0;

	float ph = h;
	float pw = w;
	float expectedRatio = 640.0 / 480.0;
	float ratio = w/h;
	if (ratio > expectedRatio) {
		ph = pw / expectedRatio;
	} else {
		pw = expectedRatio * ph;
	}

	pw = 640; ph = 480;

	//imgDraw = QImage (pw, ph, QImage::Format_RGB888);
	imgProjector = QImage (w, h, QImage::Format_RGB888);
	//imgInput = cv::Mat(pw, ph, CV_8UC3);
	//imgOutput = cv::Mat(pw, ph, CV_8UC3);
}

//-----------------------------------------------------------------------------

void MainWindow::GetImage(void)
{
	socket.PingServer(config.serverUDPPort, true); 
}

//-----------------------------------------------------------------------------

void MainWindow::RefreshScreen(void)
{	
	socketMutex.lock();

	qint64 time =  QDateTime().currentMSecsSinceEpoch();
	lostConnection = time - config.output.rxTimeTick > 1000;

	//config.output.projectorModeCopy = 2;

	if (lostConnection) {
		DrawConnectionError();	
		lostConnectionCounter++;
		if (lostConnectionCounter == 30) {
			socket.PingAll(config.serverUDPPort, false, true);
			lostConnectionCounter = 0;
		}
	} else {
		if (config.output.projectorModeCopy == 0) {
			bool ok = WarpImage();
			DrawAidsOnImage(ok);
		} else if (config.output.projectorModeCopy == 1) {
			DrawCalibrationPattern();
		} else if (config.output.projectorModeCopy == 2) {
			bool ok = WarpImage();
			DrawVisualEffects();
		}
	}
	
	socketMutex.unlock();
	
	QPixmap pixmap(QPixmap::fromImage(imgProjector));
	if (pixmap.isNull() == false) {
		int w = std::min(pixmap.width(), ui.imgLabel->maximumWidth());
		int h = std::min(pixmap.height(), ui.imgLabel->maximumHeight());
		pixmap = pixmap.scaled(QSize(w, h), Qt::IgnoreAspectRatio);//,, Qt::SmoothTransformation);
		ui.imgLabel->setPixmap(pixmap);
	}
	update();
}

//-----------------------------------------------------------------------------

void MainWindow::DrawCalibrationPattern(void)
{
	QPainter p(&imgProjector);
	QFont font = p.font() ;
	font.setPointSize (30);			
	p.setFont(font);

	int w = imgProjector.width();
	int h = imgProjector.height();
	int w2 = w/2;
	int h2 = h/2;

	QPen pen;
	QBrush brush;
	
	brush.setColor(Qt::yellow);
	brush.setStyle(Qt::SolidPattern);
	p.setBrush(brush);
	p.drawRect(0, 0, w, h);

	brush.setColor(Qt::white);
	brush.setStyle(Qt::SolidPattern);
	p.setBrush(brush);
	pen.setColor(Qt::red);
	pen.setWidth(1);
	p.setPen(pen);	
	p.drawRect(w2-w2/2, h2-h2/2, w2, h2);

	p.setBrush(Qt::NoBrush);
	pen.setColor(Qt::red);
	pen.setWidth(1);
	p.setPen(pen);
	p.drawRect(w2-w2/2, h2-h2/2, w2, h2);
	p.drawLine(w2-w2/2,h2-h2/2, w2+w2/2, h2+h2/2);
	p.drawLine(w2+w2/2,h2-h2/2, w2-w2/2, h2+h2/2);
	
	p.drawText(0, 0, w2-w2/2, h2-h2/2, Qt::AlignRight | Qt::AlignBottom, "1");
	p.drawText(w2+w2/2, 0, w, h2-h2/2, Qt::AlignLeft | Qt::AlignBottom, "2");
	p.drawText(w2+w2/2, h2+h2/2, w, h, Qt::AlignLeft | Qt::AlignTop, "3");
	p.drawText(0, h2+h2/2, w2-w2/2, h, Qt::AlignRight | Qt::AlignTop, "4");

	p.end();
}

//-----------------------------------------------------------------------------

void MainWindow::DrawConnectionError(void)
{
	QPainter p(&imgProjector);
	QFont font = p.font() ;
	font.setPointSize (30);			
	p.setFont(font);

	int w = imgProjector.width();
	int h = imgProjector.height();

	QPen pen;
	pen.setWidth(1);
	pen.setColor(Qt::black);
	p.setPen(pen);
	p.setBrush(Qt::SolidPattern);
	
	p.drawRect(0, 0, imgProjector.width(), imgProjector.height());

	QColor color = Qt::white;
	color.setAlphaF(1.0 - (float(lostConnectionCounter)/30.0));
	pen.setColor(color);
	p.setPen(pen);

	p.drawText(imgProjector.rect(), Qt::AlignHCenter | Qt::AlignVCenter,  "No connection!");
	p.end();
}

//-----------------------------------------------------------------------------

//void MainWindow::DrawAidsOnImage2(void)
////{
	/*int w = imgDraw.width();
	int h = imgDraw.height();
	
	float propX = 1;//float(w) / 640.0;
	float propY = 1;//float(h) / 480.0;

	//imgDraw = QImage (w, h, QImage::Format_RGB888);	

	QPainter p(&imgDraw);

	p.setRenderHint(QPainter::Antialiasing, true);
	//p.begin();

	// Apaga a tela
	QPen pen;
	pen.setWidth(1);
	pen.setColor(Qt::black);
	p.setPen(pen);
	p.setBrush(Qt::SolidPattern);
	p.drawRect(0, 0, w, h);

	p.setBrush(Qt::NoBrush);

	/*pen.setColor(Qt::yellow);
	pen.setWidth(3);
	p.setPen(pen);
	p.drawRect(0, 0, 640 * propX, 480 * propY);
	p.drawLine(0,0, 640 * propX, 480 * propY);
	p.drawLine(640 * propX, 0, 0, 480 * propY);
	*/
	// draw table border
	//pen.setColor(Qt::gray);
	
	/*pen.setColor(Qt::yellow);
	pen.setWidth(1);
	p.setPen(pen);
	float x = config.output.tableLimitPoints[0].x * propX;
	float y = config.output.tableLimitPoints[0].y * propY;
	float w2 = abs(config.output.tableLimitPoints[2].x * propX - x);
	float h2 = abs(config.output.tableLimitPoints[2].y * propY - y);
	p.drawRect(x, y, w2, h2);
	p.drawLine(x,y, x+w2, y+h2);
	p.drawLine(x,y+h2, x+w2, y);

	pen.setColor(Qt::red);
	p.setPen(pen);
	p.drawRect(160, 100, 360, 310);

	//draw projector limits
	pen.setColor(Qt::green);
	pen.setWidth(5);
	p.setPen(pen);
	p.drawLine(config.output.projectorLimitsPoints[0].x*propX, config.output.projectorLimitsPoints[0].y*propY, config.output.projectorLimitsPoints[1].x*propX, config.output.projectorLimitsPoints[1].y*propY);
	p.drawLine(config.output.projectorLimitsPoints[1].x*propX, config.output.projectorLimitsPoints[1].y*propY, config.output.projectorLimitsPoints[2].x*propX, config.output.projectorLimitsPoints[2].y*propY);
	p.drawLine(config.output.projectorLimitsPoints[2].x*propX, config.output.projectorLimitsPoints[2].y*propY, config.output.projectorLimitsPoints[3].x*propX, config.output.projectorLimitsPoints[3].y*propY);
	p.drawLine(config.output.projectorLimitsPoints[3].x*propX, config.output.projectorLimitsPoints[3].y*propY, config.output.projectorLimitsPoints[0].x*propX, config.output.projectorLimitsPoints[0].y*propY);

	pen.setColor(Qt::blue);
	pen.setWidth(1);
	p.setPen(pen);
	p.drawRect(0,0,w-1,h-1);
*/
	
	/*for (unsigned int i=0; i < config.output.numbBalls; i++) {
		if ( i == config.output.hitBall) {
			pen.setColor(Qt::yellow);
			p.setPen(pen);
		} else if ( i == config.output.whiteBall) {
			pen.setColor(Qt::white);
			p.setPen(pen);
		} else {
			pen.setColor(Qt::darkGray);
			p.setPen(pen);
		}
		float x = float(config.output.balls[i].x) * propX;
		float y = float(config.output.balls[i].y) * propY;
		float rayx = 20.0 * propX;
		float rayy = 20.0 * propY;
		QRectF rectangle(x - rayx, y - rayy, 2 * rayx, 2 * rayy);
		p.drawEllipse(rectangle);
	}

	pen.setColor(Qt::white);
	p.setPen(pen);
	for (int i=0; i < config.output.whiteTrajectoryCount; i++) {
		float x1 = float(config.output.whiteTrajectory[i].x1) * propX;
		float y1 = float(config.output.whiteTrajectory[i].y1) * propY;
		float x2 = float(config.output.whiteTrajectory[i].x2) * propX;
		float y2 = float(config.output.whiteTrajectory[i].y2) * propY;
		p.drawLine(x1, y1, x2, y2);
	}

	pen.setColor(Qt::yellow);
	p.setPen(pen);
	for (int i=0; i < config.output.hitTrajectoryCount; i++) {
		float x1 = float(config.output.hitTrajectory[i].x1) * propX;
		float y1 = float(config.output.hitTrajectory[i].y1) * propY;
		float x2 = float(config.output.hitTrajectory[i].x2) * propX;
		float y2 = float(config.output.hitTrajectory[i].y2) * propY;
		p.drawLine(x1, y1, x2, y2);
	}
	
	p.end();*/
//}

//-----------------------------------------------------------------------------

void MainWindow::DrawDistortedCircle(int start, QPainter &p, float x, float y, float radius)
{
	std::vector<cv::Point2f> tempPoints, circlePoints;
	//for (int i=0; i < 360; i++) {
	for (int i=start; i < 360; i+=20) {
		float degInRad = i*DEG2RAD;
		tempPoints.push_back(cv::Point(x + cos(degInRad)*radius, y + sin(degInRad)*radius));	  
	}
	perspectiveTransform(tempPoints, circlePoints, homo);
	//circlePoints = tempPoints;
	for (int i=0; i < circlePoints.size(); i++) {
		//p.drawLine(circlePoints[i-1].x * propX, circlePoints[i-1].y * propY, circlePoints[i].x * propX, circlePoints[i].y * propY); 
		p.drawPoint(circlePoints[i].x * propX, circlePoints[i].y * propY); 
	}
	//QRectF rectangle(x - radius, y - radius, 2 * radius, 2 * radius);
	//p.drawRect(rectangle);
}
 
//-----------------------------------------------------------------------------

void MainWindow::DrawAidsOnImage(bool Ok)
{
	int w = imgProjector.width();
	int h = imgProjector.height();
	
	float radius = 20;// * float(w) / 640.0;

	QPainter p(&imgProjector);
	
	p.setRenderHint(QPainter::Antialiasing, true);
	
	// Apaga a tela
	QPen pen;
	pen.setWidth(1);
	pen.setColor(Qt::black);
	p.setPen(pen);
	p.setBrush(Qt::SolidPattern);
	p.drawRect(0, 0, w, h);
	
	if (Ok == false) {
		p.end();
		return;
	}

	//balls.clear();
	whiteTrajectory.clear();
	hitTrajectory.clear();
	std::vector<cv::Point2f> tempPoints;
	/*for (unsigned int i=0; i < config.output.numbBalls; i++) {
		tempPoints.push_back(cv::Point(config.output.balls[i].x, config.output.balls[i].y));
	}
	if (tempPoints.size() > 0) {
		perspectiveTransform(tempPoints, balls, homo);
	}*/

	tempPoints.clear();
	for (int i=0; i < config.output.whiteTrajectoryCount; i++) {
		tempPoints.push_back(cv::Point(config.output.whiteTrajectory[i].x1, config.output.whiteTrajectory[i].y1));
		tempPoints.push_back(cv::Point(config.output.whiteTrajectory[i].x2, config.output.whiteTrajectory[i].y2));
	}
	if (tempPoints.size() > 0) {
		perspectiveTransform(tempPoints, whiteTrajectory, homo);
	}

	tempPoints.clear();
	for (int i=0; i < config.output.hitTrajectoryCount; i++) {
		tempPoints.push_back(cv::Point(config.output.hitTrajectory[i].x1, config.output.hitTrajectory[i].y1));
		tempPoints.push_back(cv::Point(config.output.hitTrajectory[i].x2, config.output.hitTrajectory[i].y2));
	}
	if (tempPoints.size() > 0) {
		perspectiveTransform(tempPoints, hitTrajectory, homo);
	}

	p.setBrush(Qt::NoBrush);	

	//for (unsigned int i=0; i < balls.size(); i++) {
	pen.setWidth(5);
	static int start = 0;
	start += 5;
	if (start >= 20) {
		start = 0;
	}
	for (unsigned int i=0; i < config.output.numbBalls; i++) {
		if ( i == config.output.hitBall) {
			pen.setColor(Qt::yellow);
			p.setPen(pen);
		} else if ( i == config.output.whiteBall) {
			pen.setColor(Qt::white);
			p.setPen(pen);
		} else {
			pen.setColor(Qt::darkGray);
			p.setPen(pen);
		}
		/*float x = float(balls[i].x);
		float y = float(balls[i].y);
		float rayx = 20.0;
		float rayy = 20.0;
		QRectF rectangle(x - rayx, y - rayy, 2 * rayx, 2 * rayy);
		p.drawEllipse(rectangle);*/
			
		DrawDistortedCircle(start, p, config.output.balls[i].x, config.output.balls[i].y, radius);
	}
	pen.setWidth(2);
	pen.setColor(Qt::white);
	p.setPen(pen);
	for (int i=0; i < whiteTrajectory.size() / 2; i++) {
		int pos = i * 2;
		float x1 = float(whiteTrajectory[pos].x);
		float y1 = float(whiteTrajectory[pos].y);
		float x2 = float(whiteTrajectory[pos+1].x);
		float y2 = float(whiteTrajectory[pos+1].y);
		x1 = x1 * propX; y1 = y1 * propY;
		x2 = x2 * propX; y2 = y2 * propY;
		p.drawLine(x1, y1, x2, y2);
	}

	pen.setColor(Qt::yellow);
	p.setPen(pen);
	for (int i=0; i < hitTrajectory.size() / 2; i++) {
		int pos = i * 2;
		float x1 = float(hitTrajectory[pos].x) * propX;
		float y1 = float(hitTrajectory[pos].y) * propY;
		float x2 = float(hitTrajectory[pos+1].x) * propX;
		float y2 = float(hitTrajectory[pos+1].y) * propY;
		p.drawLine(x1, y1, x2, y2);
	}
	
	p.end();
}

//-----------------------------------------------------------------------------

bool MainWindow::WarpImage(void)
{
	//imgInput = cv::Mat(imgDraw.height(), imgDraw.width(), CV_8UC3, imgDraw.bits(), imgDraw.bytesPerLine()).clone();    

	vector<cv::Point2f> src_p, dst_p;
	
	src_p.push_back(cv::Point2f(config.output.projectorLimitsPoints[0].x, config.output.projectorLimitsPoints[0].y)); //Point2f(142, 170);
	src_p.push_back(cv::Point2f(config.output.projectorLimitsPoints[1].x, config.output.projectorLimitsPoints[1].y)); //cv::Point2f(546, 169);
	src_p.push_back(cv::Point2f(config.output.projectorLimitsPoints[2].x, config.output.projectorLimitsPoints[2].y)); //cv::Point2f(445, 409);
	src_p.push_back(cv::Point2f(config.output.projectorLimitsPoints[3].x, config.output.projectorLimitsPoints[3].y)); //cv::Point2f(275, 408);
 
	dst_p.push_back(cv::Point2f(0, 0));
	dst_p.push_back(cv::Point2f(640, 0));
	dst_p.push_back(cv::Point2f(640, 480));
	dst_p.push_back(cv::Point2f(0, 480));

	homo = cv::getPerspectiveTransform(src_p, dst_p);	
	
	//QImage imgDraw  = QImage(640, 480);
	//int iw = imgDraw.width();
	//int ih = imgDraw.height();
	//cv::warpPerspective(imgInput, imgOutput , homo, cv::Size(iw, ih));	
	//imgDraw = QImage((uchar*) imgOutput.data, imgOutput.cols, imgOutput.rows, imgOutput.step, QImage::Format_RGB888);
	//imgProjector = imgDraw.scaled(imgProjector.size(), Qt::IgnoreAspectRatio);//, Qt::SmoothTransformation);

	return true;
	
}

//-----------------------------------------------------------------------------

void MainWindow::DrawVisualEffects(void)
{
	//QImage image1 (640, 480, QImage::Format_ARGB32_Premultiplied);
	cv::Mat imgBalls (480, 640, CV_8UC3, cv::Scalar(0,0,0));
	cv::Mat imgProj (480, 640, CV_8UC3, cv::Scalar(0,0,0));
	//cv::Mat cvBack = cv::Mat(imgBack.height(), imgBack.width(), CV_8UC3, imgBack.bits(), 
							 //imgBack.bytesPerLine());//.clone();    	
	//cvtColor(cvBack, cvBack,CV_BGR2RGB); 
	//cv::resize(cvBack, cvBack, cv::Size(640, 480));	
	//cv::imshow("ttt", cvBack);	
	
	for (unsigned int i=0; i < config.output.numbBalls; i++) {
		cv::circle(imgBalls, cv::Point(config.output.balls[i].x, config.output.balls[i].y), 25, cv::Scalar(100,100,100), -1);
		cv::circle(imgBalls, cv::Point(config.output.balls[i].x, config.output.balls[i].y), 23, cv::Scalar(150,150,150), -1);
		cv::circle(imgBalls, cv::Point(config.output.balls[i].x, config.output.balls[i].y), 22, cv::Scalar(200,200,200), -1);
		cv::circle(imgBalls, cv::Point(config.output.balls[i].x, config.output.balls[i].y), 20, cv::Scalar(255,255,255), -1);
	}
	
	cv::Mat cvTemp;
	cv::addWeighted(imgBalls, 0.5, cvBackMask, 0.5, 0.0, cvBackMask, -1);
	cv::bitwise_and(cvBackMask,cvBack,cvTemp);

	cv::Mat cvOutput;
	cv::warpPerspective(cvTemp, cvOutput , homo, cv::Size(640, 480));	
	QImage imgDraw = QImage((uchar*) cvOutput.data, cvOutput.cols, cvOutput.rows, cvOutput.step, QImage::Format_RGB888);
	imgProjector = imgDraw.scaled(imgProjector.size(), Qt::IgnoreAspectRatio);
}

//-----------------------------------------------------------------------------

void MainWindow::mouseDoubleClickEvent (QMouseEvent  *event)
{
	int ww = width();
	QDesktopWidget desktop;
	int wd = desktop.availableGeometry(desktop.primaryScreen()).width(); 
	int hd = desktop.availableGeometry(desktop.primaryScreen()).height(); 
	
	if (ww > wd * 0.67) {
		wd = wd / 2;
		hd = hd / 2;
	}

	imgProjector = QImage (wd, hd, QImage::Format_RGB888);	
	setGeometry(0, 0, wd, hd);
	ui.imgLabel->resize(wd, hd);
}

//-----------------------------------------------------------------------------

MainWindow::~MainWindow()
{

}

//-----------------------------------------------------------------------------
