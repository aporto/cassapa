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
#include "preview.h"
//-----------------------------------------------------------------------------

Preview::Preview(QWidget *parent)
	: QGLWidget(parent),
		socket (parent, false)
{
	ui.setupUi(this);

	drawingRectangle = 0;
	mouseX = 0;
	mouseY = 0;

	menu = new Menu;
	colorConf = new ConfigureColors;
	mode = 0;
	lostConnection = true;
	initializationError = 0;
	projDragId = -1;
	setAutoBufferSwap(true);
}

//-----------------------------------------------------------------------------

Preview::~Preview()
{
	delete menu;
	menu = NULL;
	delete colorConf;
	colorConf = NULL;
}

//-----------------------------------------------------------------------------

void Preview::Initialize(void)
{
	
	//QImage img;
	//img.load(":/kassapa_viewer/Resources/images/cue.png");
	//texture = new QOpenGLTexture(img);
	//texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	//texture->setMagnificationFilter(QOpenGLTexture::Linear);
	initializationError = 0;
	
	//socket.PingAll(config.serverUDPPort); //SendConfig("255.255.255.255", config.serverUDPPort);
	
	if (socket.Start(0, &(config.input), &(config.output))) {
		socket.PingAll(config.serverUDPPort, config.isMainController, false); //SendConfig("255.255.255.255", config.serverUDPPort);		
	} else {
		initializationError = -1;
	}

	imageReceiver.Initialize();
	//GetImage();

	//QTimer *timerPing = new QTimer(this);
	//connect(timerPing, SIGNAL(timeout()), this, SLOT(pingServer()));
	//timerPing->start(100);

//	QTimer *timerPing = new QTimer(this);
	//connect(timerPing, SIGNAL(timeout()), this, SLOT(pingServer()));	
	//timerPing->start(100);

	//timerPing->setInterval(100);
	//timerPing->start();
	
	timerGetImage = new QTimer(this);
	connect(timerGetImage, SIGNAL(timeout()), this, SLOT(GetImage()));	
	timerGetImage->start(100);

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);

	
		
	lostConnectionAlpha = 1.0;
}

//-----------------------------------------------------------------------------

void Preview::PingServer(void)
{
	//socket.PingServer(config.serverUDPPort); 
}

//-----------------------------------------------------------------------------

void Preview::GetImage(void)
{
	QString address = socket.GetRemoteAddress();
	if (address != config.serverIP) {
		config.serverIP = address;
	}
	socket.PingServer(config.serverUDPPort, false); 
	imageReceiver.Get (config.serverIP, config.serverTCPPort);//("127.0.0.1", 30777);	
}

//-----------------------------------------------------------------------------

void Preview::LoadImage(QImage &img, QString name, int textureIndex)
{
	QString file = ":/kassapa_viewer/Resources/images/" + name;
	img.load(file);	
	img = QGLWidget::convertToGLFormat(img);

	glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(),
					0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 
}

//-----------------------------------------------------------------------------


/*void Preview::StartVideo(void)
{
	int index = 0;
	QString name = "";
	do {
		name = config.input.appPath + "/recvideo_" + QString::number(index) + ".avi";
		index++;
	} while (QFile::exists(name));

	//cv::Size size = cvSize(detector.frame.size().width, detector.frame.size().height);
	//string file = name.toLocal8Bit();
	//outputVideo.open(file, CV_FOURCC('Y','V','1','2'), 10, size, true);	
}

//-----------------------------------------------------------------------------

void Preview::EndVideo(void)
{
	outputVideo.release();
}*/

//-----------------------------------------------------------------------------

void Preview::Draw(bool AddAimingToVideo) //QImage * data)
{	
	/*if (outputVideo.isOpened()) {
		cv::Mat tmp;
		cv::cvtColor(detector.frame, tmp, CV_HSV2BGR);
		imshow("", tmp);
		dest = QImage((uchar*) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
		gldata = QGLWidget::convertToGLFormat(dest);
	} else {*/
		//dest = QImage((uchar*) detector.frame.data, detector.frame.cols, detector.frame.rows, detector.frame.step, QImage::Format_RGB888);
		//gldata = QGLWidget::convertToGLFormat(detector.previewdest);
	//}
	
	/*if (outputVideo.isOpened()) {
		if (AddAimingToVideo) {
			QImage dest;
			dest = grabFrameBuffer();
			dest = dest.scaled(detector.frame.size().width, detector.frame.size().height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//,Qt::KeepAspectRatio
			dest = dest.convertToFormat(QImage::Format_RGB888);			
			cv::Mat tmp(dest.height(),dest.width(),CV_8UC3,(uchar*)dest.bits(),dest.bytesPerLine());			
			outputVideo.write(tmp);
		} else {
			//outputVideo.write(detector.frame);
			QImage qtemp = detector.preview;
			qtemp = qtemp.convertToFormat(QImage::Format_RGB888);			
			//dest = dest.convertToFormat(QImage::Format_RGB888);			
			//detector.previewRGB = detector.previewRGB.convertToFormat(QImage::Format_RGB888);			
			cv::Mat tmp(qtemp.height(), qtemp.width(),CV_8UC3,(uchar*)qtemp.bits(), qtemp.bytesPerLine());			
			outputVideo.write(tmp);
		}
	}*/

	update();	
}

//-----------------------------------------------------------------------------

void Preview::Rectangle(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1,y2);
	glEnd();
}

//-----------------------------------------------------------------------------

void Preview::FilledRectangle(int x1, int y1, int x2, int y2)
{
	glBegin(GL_TRIANGLES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1,y2);
	glEnd();
}

//-----------------------------------------------------------------------------

void Preview::Line(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

//-----------------------------------------------------------------------------

void Preview::Circle(float x, float y, float radius)
{
   glBegin(GL_LINE_LOOP);

   float aspect = 1;//float(width()) / float(height());
 
   for (int i=0; i < 360; i++)
   {
	  float degInRad = i*DEG2RAD;
	  glVertex2f(x+ cos(degInRad)*radius, y+ sin(degInRad)*radius * aspect);
   }
 
   glEnd();
}

//-----------------------------------------------------------------------------

void Preview::drawBallMarker(float x, float y, float radius)
{
	hatchCounter+=4;
	if (hatchCounter > 20 * 56) {
		hatchCounter = 20;
	}
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glLineWidth(3.0);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//glColor4f(1,1,1,1);
	for (int i=0; i < 360; i+=18) {
		float angle1 = (i-8+hatchCounter)*DEG2RAD;
		float angle2 = (i+hatchCounter) * DEG2RAD;
		float angle3 = (i + 8+hatchCounter) * DEG2RAD;
		float x1 = cos(angle2) * radius + x;
		float y1 = sin(angle2) * radius + y;

		/*float x2 = cos(angle1) * (radius + 17) + x;
		float y2 = sin(angle1) * (radius + 5) + y;

		float x3 = cos(angle3) * (radius + 17) + x;
		float y3 = sin(angle3) * (radius + 5) + y;

		glBegin(GL_TRIANGLES);
		glVertexFix(x1,y1);
		glVertexFix(x2,y2);
		glVertexFix(x3,y3);
		glEnd();*/
		Circle(x1, y1, 0.90);
	}

	//drawCircle(x, y, radius);
}

//-----------------------------------------------------------------------------

void Preview::DrawCrossedCircleAroundBall(int ball)
{
	if (config.output.numbBalls > ball) {
		glLineWidth(3.0);
		Circle(config.output.balls[ball].x, config.output.balls[ball].y, config.input.ballSize * 2);

		glLineWidth(2.0);
		int llen = int(float(config.input.ballSize) * 0.7071067);
		Line(config.output.balls[ball].x - llen, config.output.balls[ball].y - llen, 
			 config.output.balls[ball].x + llen, config.output.balls[ball].y + llen);
		Line(config.output.balls[ball].x + llen, config.output.balls[ball].y - llen, 
			 config.output.balls[ball].x - llen, config.output.balls[ball].y + llen);
		QString text = QString().number(ball);
		DrawText(text, config.output.balls[ball].x +20 , config.output.balls[ball].y - 20 , 0, 1,1,1,1.0);
	}
}

//-----------------------------------------------------------------------------

void Preview::DrawAids(void)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glPixelZoom(1.0f, 1.0f);

	// draw table border
	glColor3f(0.5f, 0.5f, 0.5f); 
	glLineWidth(3.0);
	Rectangle(config.input.limitX1, config.input.limitY1, config.input.limitX2, config.input.limitY2);
	glColor4f(0, 0, 0, 0.45); 
	FilledRectangle(0, 0, 640, config.input.limitY1);
	FilledRectangle(0, config.input.limitY2, 640, 480);
	FilledRectangle(0, config.input.limitY1, config.input.limitX1, config.input.limitY2);
	FilledRectangle(config.input.limitX2, config.input.limitY1, 640, config.input.limitY2);
	
	// draw holes
	int x = config.input.limitX1;
	int y = config.input.limitY1;
	glColor3f(0.7f, 0.7f, 0.7f); 
	// canto superior esquerdo
	Line(x,y, x+ config.input.holeWidth, y);
	Line(x,y, x, y + config.input.holeWidth);
	// canto superior direito
	x = config.input.limitX2;
	y = config.input.limitY1;
	Line(x,y, x - config.input.holeWidth, y);
	Line(x,y, x, y + config.input.holeWidth);
	// canto inferior direito
	x = config.input.limitX2;
	y = config.input.limitY2;
	Line(x,y, x - config.input.holeWidth, y);
	Line(x,y, x, y - config.input.holeWidth);
	// canto inferior esquerdo
	x = config.input.limitX1;
	y = config.input.limitY2;
	Line(x,y, x + config.input.holeWidth, y);
	Line(x,y, x, y - config.input.holeWidth);
	// centro topo
	x = config.input.limitX1 + (config.input.limitX2 - config.input.limitX1) / 2;
	y = config.input.limitY1;
	Line(x - config.input.holeWidth/2,y, x + config.input.holeWidth/2, y);
	// centro baixo
	y = config.input.limitY2;
	Line(x - config.input.holeWidth/2,y, x + config.input.holeWidth/2, y);


	// desenha sombra do projetor
	if (config.output.hasProjector) {
		glColor4f(1.0f, 1.0f, 1.0f, 0.5); 
		glLineWidth(1.0);
		Line(config.input.projectorLimitsPoints[0].x, config.input.projectorLimitsPoints[0].y, config.input.projectorLimitsPoints[1].x, config.input.projectorLimitsPoints[1].y);
		Line(config.input.projectorLimitsPoints[1].x, config.input.projectorLimitsPoints[1].y, config.input.projectorLimitsPoints[2].x, config.input.projectorLimitsPoints[2].y);
		Line(config.input.projectorLimitsPoints[2].x, config.input.projectorLimitsPoints[2].y, config.input.projectorLimitsPoints[3].x, config.input.projectorLimitsPoints[3].y);
		Line(config.input.projectorLimitsPoints[3].x, config.input.projectorLimitsPoints[3].y, config.input.projectorLimitsPoints[0].x, config.input.projectorLimitsPoints[0].y);
	}
		
	
	// draw croessed circle around all balls	
	glColor3f(0.5f, 0.5f, 0.5f); 			
	for (unsigned int i=0; i < config.output.numbBalls; i++) {
		DrawCrossedCircleAroundBall(i);
	}
	
	// Draw crossed circle over white ball
	if (config.output.numbBalls > 0) {
		if (config.output.whiteBall >= 0) {
			glColor3f(1, 1, 1); 
			DrawCrossedCircleAroundBall(config.output.whiteBall);		
			// draw white ball trajectory
			glLineWidth(4.0);
			for (unsigned int i=0; i < config.output.whiteTrajectoryCount; i++) {
				cv::Point lp1, lp2;
				Line(config.output.whiteTrajectory[i].x1, config.output.whiteTrajectory[i].y1,
					 config.output.whiteTrajectory[i].x2, config.output.whiteTrajectory[i].y2);
				Circle(config.output.whiteTrajectory[i].x2, config.output.whiteTrajectory[i].y2, config.input.ballSize);
			}
		}

		// draw hit ball trajectory	
		if (config.output.hitBall >= 0) {
			glColor3f(1.0, 1.0, 0.0); 
			//glLineWidth(3.0);
			DrawCrossedCircleAroundBall(config.output.hitBall);		
			glLineWidth(4.0);
			for (unsigned int i=0; i < config.output.hitTrajectoryCount; i++) {			
				Line(config.output.hitTrajectory[i].x1, config.output.hitTrajectory[i].y1,
				 config.output.hitTrajectory[i].x2, config.output.hitTrajectory[i].y2);
			}
			// Draw a white circle at the position where whiteball will
			// touch the hitball				
			if (config.output.whiteTrajectoryCount > 0) {
				glColor3f(1, 1, 1); 			
				glLineWidth(2.0);
				Circle(config.output.whiteTrajectory[config.output.whiteTrajectoryCount-1].x2, 
					   config.output.whiteTrajectory[config.output.whiteTrajectoryCount-1].y2, 
					   config.input.ballSize);
			}

			// Draw a purple circle at the position where hitBall ends
			if (config.output.hitTrajectoryCount > 0) {
				glColor3f(1.0, 1.0, 0.0); 
				glLineWidth(2.0);
				Circle(config.output.hitTrajectory[config.output.hitTrajectoryCount-1].x2, 
					   config.output.hitTrajectory[config.output.hitTrajectoryCount-1].y2, 
					   config.input.ballSize);
			}

		}	
	}

	// draw cue vector trajectory
	if (config.output.cueFound) {
		if (config.showCue) {
			glLineWidth(2.0);			
			glColor3f(1, 0, 0); 
			Line(config.output.cue.x1,config.output.cue.y1, config.output.cue.x2, config.output.cue.y2);
		}
	}


	
	// draw projector box
	/*glLineWidth(1.0);
	glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
	Rectangle(config.input.projector.box.x1, config.input.projector.box.y1, config.input.projector.box.x2, config.input.projector.box.y2);
	float w,c;
	glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
	if ( (config.input.projector.angle == 0) || (config.input.projector.angle == 2)) {
		w = float(config.input.projector.box.x2 - config.input.projector.box.x1) / config.input.projector.topBottomRatio / 2;
		c = config.input.projector.box.x1 + (config.input.projector.box.x2 - config.input.projector.box.x1) / 2;
		if (config.input.projector.angle == 0) {
			Line(config.input.projector.box.x1, config.input.projector.box.y1, config.input.projector.box.x2, config.input.projector.box.y1);	
			Line(config.input.projector.box.x1, config.input.projector.box.y1, c - w, config.input.projector.box.y2);
			Line(config.input.projector.box.x2, config.input.projector.box.y1, c + w, config.input.projector.box.y2);
			Line(c - w, config.input.projector.box.y2, c + w, config.input.projector.box.y2);
		} else {
			Line(config.input.projector.box.x1, config.input.projector.box.y2, config.input.projector.box.x2, config.input.projector.box.y2);	
			Line(config.input.projector.box.x1, config.input.projector.box.y2, c - w, config.input.projector.box.y1);
			Line(config.input.projector.box.x2, config.input.projector.box.y2, c + w, config.input.projector.box.y1);
			Line(c - w, config.input.projector.box.y1, c + w, config.input.projector.box.y1);
		}
		
	} else {
		w = float(config.input.projector.box.y2 - config.input.projector.box.y1) / config.input.projector.topBottomRatio / 2;
		c = config.input.projector.box.y1 + (config.input.projector.box.y2 - config.input.projector.box.y1) / 2;
		if (config.input.projector.angle == 1) {
			Line(config.input.projector.box.x1, config.input.projector.box.y1, config.input.projector.box.x1, config.input.projector.box.y2);	
			Line(config.input.projector.box.x1, config.input.projector.box.y1, config.input.projector.box.x2, c - w);
			Line(config.input.projector.box.x1, config.input.projector.box.y2, config.input.projector.box.x2, c + w);
			Line(config.input.projector.box.x2, c - w, config.input.projector.box.x2, c + w);
		} else {
			Line(config.input.projector.box.x2, config.input.projector.box.y1, config.input.projector.box.x2, config.input.projector.box.y2);	
			Line(config.input.projector.box.x2, config.input.projector.box.y1, config.input.projector.box.x1, c - w);
			Line(config.input.projector.box.x2, config.input.projector.box.y2, config.input.projector.box.x1, c + w);
			Line(config.input.projector.box.x1, c - w, config.input.projector.box.x1, c + w);
		}
	}*/

	glPopMatrix();

}

//-----------------------------------------------------------------------------

void Preview::StartRectangle(int id)
{
	drawingRectangle = id;
}

//-----------------------------------------------------------------------------

void Preview::SetRectangle(ScreenLine rect)
{	
	mouseRect = rect;	
}

//-----------------------------------------------------------------------------

void Preview::GetRectangle(ScreenLine &rect)
{
	rect = mouseRect;
}

//-----------------------------------------------------------------------------

int Preview::GettingRectangle(void)
{
	return drawingRectangle;
}

//-----------------------------------------------------------------------------

void Preview::DrawImage(int x, int y, int w, int h, int id)
{
	glDepthMask(false);

	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_TEXTURE_2D_MULTISAMPLE);//GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[id]);
	//texture->bind();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(x + w, y);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(x + w, y - h);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y - h);
	glEnd();

	glDepthMask(true);
	glColor3f(1, 1, 1); 
	glDisable(GL_TEXTURE_2D);
}

//-----------------------------------------------------------------------------

void Preview::DrawText(QString text,float x,float y, float z, float red, float green, float blue, float alpha)
{
	glPushMatrix();
	glColor4f(red, green, blue, alpha);
	renderText(x, y, z, text);
	glPopMatrix();
}

//-----------------------------------------------------------------------------

void Preview::paintGL()
{
	socketMutex.lock();

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float w = width();
	float h = height();

	glViewport (0, 0, w, h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float expectedRatio = 640.0 / 480.0;
	float ratio = w/h;
	float cx = 320 * (ratio / expectedRatio);
	float cy = 240 * (expectedRatio / ratio);
	float pixelAspect = 1.0;
	float rasterX = 0.0;
	float rasterY = 0.0;
	if (ratio > expectedRatio) {	
		ortho.x1 = 320 - cx;
		ortho.x2 = 320 + cx;
		ortho.y1 = 480;
		ortho.y2 = 0;		
		//glOrtho(320 - cx, 320 + cx, 480, 0, -1.0, 1.0);
		pixelAspect = h/480.0;
	} else {		
		ortho.x1 = 0;
		ortho.x2 = 640;
		ortho.y1 = 240 + cy;
		ortho.y2 = 240 - cy;		
		//glOrtho(0, 640, 240 + cy, 240 - cy, -1.0, 1.0);
		pixelAspect = w/640.0;
	}
	glOrtho(ortho.x1, ortho.x2, ortho.y1, ortho.y2, -1.0, 1.0);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glShadeModel(GL_SMOOTH);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable( GL_BLEND );

	qint64 time =  QDateTime().currentMSecsSinceEpoch();
	qint64 dt = time - config.output.rxTimeTick;
	lostConnection = dt > 1000;
	if (initializationError == -1) {
		lostConnection = true;
	}
	/*if (dt < 5000) {
		if (dt < 100) {dt = 100;}
		if ( (timerGetImage->interval() < dt) || (timerGetImage->interval() > 2 * dt)) {
			float newInterval = float(dt) * 1.3;
			timerGetImage->setInterval(newInterval);
		}
	}*/

	if (lostConnection) {
		if (mode == 0) {
			glDepthMask(false);
			glColor3f(1, 1, 1); 
			//glEnable(GL_TEXTURE_2D);
			if (lostConnectionAlpha > 0.0) {
				glColor4f(0.0, 1.0, 0.0, lostConnectionAlpha);						
				lostConnectionAlpha = lostConnectionAlpha - 0.002;
				if (lostConnectionAlpha < 0.0) {
					lostConnectionAlpha = 0.0;
				}
			} else {
				glColor4f(0.4, 0.4, 0.4, 0.8);		
			}
			
			imageReceiver.mutex.lock();		
			DrawImage(320-75, 240+75,150,150, TEXTURE_CONNECTION);
			imageReceiver.mutex.unlock();	

			glColor4f(1.0, 1.0, 1.0, 1.0);
			if (lostConnectionAlpha > 0.0) {
				DrawImage(320-75, 400 ,150, 75, TEXTURE_CONNECTING_MSG);
			} else {
				DrawImage(320-150, 400 ,300, 75, TEXTURE_NO_CASSAPA);				
			}			
		}
	} else {		
		if ((imageReceiver.qImage.width() > 0) && imageReceiver.IsLastImageFresh(500)) {			
			glDepthMask(false);
			glColor3f(1, 1, 1); 
			imageReceiver.mutex.lock();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, imageReceiver.texture[0]);

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f( 0.0f, 480.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 640.0f, 480.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 640.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f( 0.0f, 0.0f);
			glEnd();
			
			imageReceiver.mutex.unlock();			
			glDisable(GL_TEXTURE_2D);
		} else {
			glColor4f(1, 1, 1, 0.2); 
			DrawImage(320-128,240+128,256,256, TEXTURE_NO_CAMERA);
			//DrawImage(220, 340, 200, 200, TEXTURE_CLICK_FINISH);
		}

		//glDepthMask(true);
		
		if (mode == 0) {
			
			glColor4f(1, 1, 1, 1.0); 
			DrawAids();	
		} else if (mode == 1) {
			static float alpha = 1.0;
			glDisable (GL_DEPTH_TEST);
			alpha = alpha - 0.06;
			if (alpha < 0.0) {
				alpha = 1.0;
			}
			glPixelZoom(1.0f, 1.0f);
			glColor4f(1.0f, 0.0f, 0.0f, alpha); 
			glLineWidth(1.0);
			Rectangle(configBorderX1, configBorderY1, configBorderX2, configBorderY2);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
			DrawImage(220, 340, 200, 200, TEXTURE_CLICK_FINISH);
		} else {
			static float alpha = 1.0;
			glDisable (GL_DEPTH_TEST);
			alpha = alpha - 0.06;
			if (alpha < 0.0) {
				alpha = 1.0;
			}
			glPixelZoom(1.0f, 1.0f);

			glColor4f(1.0f, 1.0f, 1.0f, alpha); 
			glLineWidth(1.0);
			Line(config.input.projectorLimitsPoints[0].x, config.input.projectorLimitsPoints[0].y, config.input.projectorLimitsPoints[1].x, config.input.projectorLimitsPoints[1].y);
			Line(config.input.projectorLimitsPoints[1].x, config.input.projectorLimitsPoints[1].y, config.input.projectorLimitsPoints[2].x, config.input.projectorLimitsPoints[2].y);
			Line(config.input.projectorLimitsPoints[2].x, config.input.projectorLimitsPoints[2].y, config.input.projectorLimitsPoints[3].x, config.input.projectorLimitsPoints[3].y);
			Line(config.input.projectorLimitsPoints[3].x, config.input.projectorLimitsPoints[3].y, config.input.projectorLimitsPoints[0].x, config.input.projectorLimitsPoints[0].y);

			glColor4f(1.0f, 0.0f, 0.0f, alpha); 
			glLineWidth(1.0);
			Line(minorProjectorLimits[0].x, minorProjectorLimits[0].y, minorProjectorLimits[1].x, minorProjectorLimits[1].y);
			Line(minorProjectorLimits[1].x, minorProjectorLimits[1].y, minorProjectorLimits[2].x, minorProjectorLimits[2].y);
			Line(minorProjectorLimits[2].x, minorProjectorLimits[2].y, minorProjectorLimits[3].x, minorProjectorLimits[3].y);
			Line(minorProjectorLimits[3].x, minorProjectorLimits[3].y, minorProjectorLimits[0].x, minorProjectorLimits[0].y);
			//glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
			for (int i=0; i < 4; i++) {
				Rectangle(minorProjectorLimits[i].x-10, minorProjectorLimits[i].y-10,
					minorProjectorLimits[i].x+10, minorProjectorLimits[i].y+10);
			}
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
			DrawImage(minorProjectorLimits[0].x-32, minorProjectorLimits[0].y, 32, 32, TEXTURE_NUMBER_1);
			DrawImage(minorProjectorLimits[1].x, minorProjectorLimits[1].y, 32, 32, TEXTURE_NUMBER_2);
			DrawImage(minorProjectorLimits[2].x, minorProjectorLimits[2].y+32, 32, 32, TEXTURE_NUMBER_3);
			DrawImage(minorProjectorLimits[3].x-32, minorProjectorLimits[3].y+32, 32, 32, TEXTURE_NUMBER_4);
		}

		QString text = "FPS:" + QString().number((1000/config.output.frameTime), 'f', 2) + " (" + QString().number(config.output.frameTime, 'f', 1) + " ms)";
		DrawText(text, 150, 15, 0, 1,1,1,0.4);

		if (config.output.cueFound) {
			glColor4f(0.0, 1.0, 0.0, 0.7);
		} else {
			glColor4f(0.4, 0.4, 0.4, 0.8);
		}
		DrawImage(10,40,32,32, TEXTURE_CUE);
		
		if (config.output.numbBalls > 0) {
			glColor4f(0.0, 1.0, 0.0, 0.7);
		} else {
			glColor4f(0.4, 0.4, 0.4, 0.8);
		}
		DrawImage(50,40,32,32, TEXTURE_BALL);

		if (config.output.errorCode == CASSAPA_ERROR_COULD_NOT_GET_CAMERA_IMAGE) {
			glColor4f(1.0, 1.0, 1.0, 0.8);
			DrawImage(90,40,32,32, TEXTURE_NO_CAMERA);
		}	
		
		/*if (config.output.licenceStatus >= 0) {
			QString text = "Demo: " + QString().number((config.output.licenceStatus)) + " days trial left";
			DrawText(text, 300, 15, 0, 1,1,1,0.4);
		} else {
			QString text = "Demo: Invalid licence. Error code (" + QString().number((config.output.licenceStatus)) + ")";
			DrawText(text, 300, 15, 0, 1,1,1,1.0);
		}*/
	}

	//glFlush();  

	socketMutex.unlock();
	
}

//-----------------------------------------------------------------------------

void Preview::initializeGL()
{
	glClearColor(0, 0, 0, 0);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable( GL_CULL_FACE );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_MULTISAMPLE);    

	glGenTextures(LAST_TEXTURE, textures);
	LoadImage(imgTextBall, "ball.png", TEXTURE_BALL);
	LoadImage(imgTextCue, "cue.png", TEXTURE_CUE);
	LoadImage(imgTextConnection, "connection.png", TEXTURE_CONNECTION);
	LoadImage(imgClickToFinish, "click_to_finish.png", TEXTURE_CLICK_FINISH);		
	LoadImage(imgConnectionMessage, "connecting_message.png", TEXTURE_CONNECTING_MSG);	
	LoadImage(imgNoCassapa, "no_cassapa.png", TEXTURE_NO_CASSAPA);			
	LoadImage(imgNumb1, "number_1.png", TEXTURE_NUMBER_1);			
	LoadImage(imgNumb2, "number_2.png", TEXTURE_NUMBER_2);			
	LoadImage(imgNumb3, "number_3.png", TEXTURE_NUMBER_3);			
	LoadImage(imgNumb4, "number_4.png", TEXTURE_NUMBER_4);		
	LoadImage(imgNoCamera, "no_camera.png", TEXTURE_NO_CAMERA);		

}

//-----------------------------------------------------------------------------

void Preview::resizeGL(int width, int height)
{
	//glViewport (0, 0, width, height);
	//glViewport (0, 0, 640, 480);
}

//-----------------------------------------------------------------------------

void Preview::mouseMoveEvent(QMouseEvent  *event)
{
	float ax = float(width()) / 640.0;
	float ay = float(height()) / 480.0;
	mouseX = event->x() / ax;
	mouseY = event->y() / ay;	

	ValidateMouse(mouseX, mouseY);

	if (mode == 1) {
		//ValidateMouse(mouseX, mouseY);
		if (configBorderTopLeft) {
			configBorderX1= mouseX;
			configBorderY1= mouseY;		
		} else {
			configBorderX2= mouseX;
			configBorderY2= mouseY;		
		}
	} else if (mode == 2) {
		if (projDragId >= 0) {
			minorProjectorLimits[projDragId].x = (mouseX + projDragX);
			minorProjectorLimits[projDragId].y = (mouseY + projDragY);
			if (minorProjectorLimits[projDragId].x < 0) { minorProjectorLimits[projDragId].x = 0;}
			if (minorProjectorLimits[projDragId].x > 640) { minorProjectorLimits[projDragId].x = 640;}
			if (minorProjectorLimits[projDragId].y < 0) { minorProjectorLimits[projDragId].y = 0;}
			if (minorProjectorLimits[projDragId].y > 480) { minorProjectorLimits[projDragId].y = 480;}

			ScalePolygon(minorProjectorLimits, config.input.projectorLimitsPoints, 4, 2.0);
		}
		/*for (int i=0; i < 4; i++) {
				if ( (mouseX >= minorProjectorLimits[i].x-5) && (mouseX <= minorProjectorLimits[i].x+5)) {
					if ( (mouseY >= minorProjectorLimits[i].y-5) && (mouseY <= minorProjectorLimits[i].y+5)) {
						projDragId = i;
						projDragX = mouseX;
						projDragY = mouseY;
					}
				}
			}*/
	}
}

void Preview::mouseDoubleClickEvent (QMouseEvent  *event)
{
	if (event->type() != QEvent::MouseButtonDblClick) {
		return;
	}

	ValidateMouse(mouseX, mouseY);

	if (mode == 0) {

		if (lostConnection) {
			if (lostConnectionAlpha <= 0.01) {
				lostConnectionAlpha = 1.0;
				socket.PingAll(config.serverUDPPort, config.isMainController, false);
			}
			return;
		}
		
		if (config.output.processorUsage == 0) {
			menu->SetProcessorFireIcon(false);
		} else {
			menu->SetProcessorFireIcon(true);
		}
		int res = menu->Display();
		
		if (res == BUTTON_CONFIG_BORDER) {
			configBorderX1 = config.input.limitX1;
			configBorderX2 = config.input.limitX2;
			configBorderY1 = config.input.limitY1;
			configBorderY2 = config.input.limitY2;
			mode = 1;
		} else if (res == BUTTON_CONFIG_CUE) {
			if (colorConf->SetCue()) {
				socket.SendConfig();
			}
		} else if (res == BUTTON_CONFIG_BALLS) {
			if (colorConf->SetBalls()) {
				socket.SendConfig();
			}
		} else if (res == BUTTON_CONFIG_PROJECTOR) {
			config.input.projectorMode = 1;
			ScalePolygon(config.input.projectorLimitsPoints, minorProjectorLimits, 4, 0.5);
			
			socket.SendConfig();
			mode = 2;
		} else if (res == BUTTON_CONFIG_PROCESSOR) {
			if (config.output.processorUsage == 0) {
				config.input.processorUsage = 1;
			} else {
				config.input.processorUsage = 0;
			}
			socket.SendConfig();
		} else if (res == BUTTON_CONFIG_RESTART) {
			config.input.terminate = 1;
			socket.SendConfig();
			config.input.terminate = 0;
		}
		
	} else if (mode == 2) {
		for (int i=0; i < 4; i++) {
			if ( (mouseX >= minorProjectorLimits[i].x-10) && (mouseX <= minorProjectorLimits[i].x+10)) {
				if ( (mouseY >= minorProjectorLimits[i].y-10) && (mouseY <= minorProjectorLimits[i].y+10)) {
					return;
				}
			}
		}
		mode = 0;
		config.input.projectorMode = 0;
		socket.SendConfig();
	}
}

//-----------------------------------------------------------------------------

void Preview::mousePressEvent(QMouseEvent  *event)
{
	float ax = float(width()) / 640.0;
	float ay = float(height()) / 480.0;
	mouseX = event->x() / ax;
	mouseY = event->y() / ay;

	if (lostConnection) {
		return;
	}

	if (mode == 0) {
		return;
	}

	//	float expectedRatio = 640.0 / 480.0;
	//float ratio = float(width())/(height());

	//if (ratio > expectedRatio) {	

	ValidateMouse(mouseX, mouseY);

	//if (ortho.x1 != 0) {
//		mouseX += +ortho.x1;
	//}
	//if (ortho.y2 != 0) {
		//mouseY += ortho.y2 + 240;
	//}

	if (event->type() != QEvent::MouseButtonDblClick) {
		if (mode == 1) {
			if ( ( (mouseX > 220) && (mouseX < 420)) && ( (mouseY > 140) && (mouseY < 340)))  {
				if (configBorderX1 > configBorderX2) {
					int aux = configBorderX1;
					configBorderX1 = configBorderX2;
					configBorderX2 = aux; 
				}
				if (configBorderY1 > configBorderY2) {
					int aux = configBorderY1;
					configBorderY1 = configBorderY2;
					configBorderY2 = aux; 
				}

				config.input.limitX1 = configBorderX1;
				config.input.limitX2 = configBorderX2;
				config.input.limitY1 = configBorderY1;
				config.input.limitY2 = configBorderY2;
				mode = 0;
				socket.SendConfig();
			} else {
				float dist1 = sqrt(pow(abs(mouseX - configBorderX1), 2.0f) + pow(abs(mouseY - configBorderY1), 2.0f));
				float dist2 = sqrt(pow(abs(mouseX - configBorderX2), 2.0f) + pow(abs(mouseY - configBorderY2), 2.0f));

				configBorderTopLeft = dist1 < dist2;

				if (configBorderTopLeft) {
					configBorderX1= mouseX;
					configBorderY1= mouseY;		
				} else {
					configBorderX2= mouseX;
					configBorderY2= mouseY;		
				}
			}
		} else { // mode == 2
			projDragId = -1;
			for (int i=0; i < 4; i++) {
				if ( (mouseX >= minorProjectorLimits[i].x-10) && (mouseX <= minorProjectorLimits[i].x+10)) {
					if ( (mouseY >= minorProjectorLimits[i].y-10) && (mouseY <= minorProjectorLimits[i].y+10)) {
						projDragId = i;
						projDragX = minorProjectorLimits[i].x - mouseX;
						projDragY = minorProjectorLimits[i].y - mouseY;
					}
				}
			}
			ScalePolygon(minorProjectorLimits, config.input.projectorLimitsPoints, 4, 2.0);
		}
	}
}

void Preview::mouseReleaseEvent(QMouseEvent  *event)
{
/*	float ax = float(width()) / 640.0;
	float ay = float(height()) / 480.0;
	mouseX = event->x() / ax;
	mouseY = event->y() / ay;

	if (drawingRectangle > 0) {
		mouseRect.x2 = mouseX;
		mouseRect.y2 = mouseY;
		if (mouseRect.x1 > mouseRect.x2) {
			int aux = mouseRect.x1;
			mouseRect.x1 = mouseRect.x2;
			mouseRect.x2 = aux;
		}
		if (mouseRect.y1 > mouseRect.y2) {
			int aux = mouseRect.y1;
			mouseRect.y1 = mouseRect.y2;
			mouseRect.y2 = aux;
		}

		drawingRectangle = -drawingRectangle;
	}
*/
}

void Preview::ValidateMouse(int &mouseX, int &mouseY)
{
	if (ortho.x1 != 0) {
		float w = ortho.x2 - ortho.x1;
		float m = w / 640.0;
		mouseX = float(mouseX) * m + ortho.x1;
	} else {
		float h = ortho.y1 - ortho.y2;
		float m = h / 480.0;
		mouseY = float(mouseY) * m + ortho.y2;
	}

	if (mouseX < 0) {
		mouseX = 0;
	}
	if (mouseX > 639) {
		mouseX = 639;
	}
	if (mouseY < 0) {
		mouseY = 0;
	}
	if (mouseY > 479) {
		mouseY = 479;
	}
}

//-----------------------------------------------------------------------------

void Preview::ScalePolygon(ScreenPoint * input, ScreenPoint * output, int numPoints, float scale)
{
	float centerX = 0.0;
	float centerY = 0.0;

	// Primeiro, encontra o centro do poligono
	double signedArea = 0.0;
	double x0 = 0.0; // Current vertex X
	double y0 = 0.0; // Current vertex Y
	double x1 = 0.0; // Next vertex X
	double y1 = 0.0; // Next vertex Y
	double a = 0.0;  // Partial signed area

	// For all vertices except last
	int i=0;
	for (i=0; i<numPoints-1; ++i)
	{
		x0 = input[i].x;
		y0 = input[i].y;
		x1 = input[i+1].x;
		y1 = input[i+1].y;
		a = x0*y1 - x1*y0;
		signedArea += a;
		centerX += (x0 + x1)*a;
		centerY += (y0 + y1)*a;
	}

	// Do last vertex
	x0 = input[i].x;
	y0 = input[i].y;
	x1 = input[0].x;
	y1 = input[0].y;
	a = x0*y1 - x1*y0;
	signedArea += a;
	centerX += (x0 + x1)*a;
	centerY += (y0 + y1)*a;

	signedArea *= 0.5;
	centerX /= (6.0*signedArea);
	centerY /= (6.0*signedArea);
	

	// Escala
	for (i=0; i < numPoints; i++) {
		// Passo 1: move o centro do poligono para (0,0)
		output[i].x = input[i].x - centerX;
		output[i].y = input[i].y - centerY;

		// Passo 2: Escala
		output[i].x = output[i].x * scale;
		output[i].y = output[i].y * scale;

		// Passo 3: Retorna ao ponto original
		output[i].x = output[i].x + centerX;
		output[i].y = output[i].y + centerY;
	}
}
