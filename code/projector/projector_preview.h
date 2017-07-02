#ifndef PREVIEW_H
#define PREVIEW_H

#include <QGLWidget>

#include <QTimer>
#include <QMouseEvent>
#include <qopengltexture.h>
#include "ui_preview.h"

#include "config_viewer.h"
#include "image_receiver.h"
#include "menu.h"
#include "configurecolors.h"

#include <socket_server.h>

//#include <GLES/gl.h>
//#include <GLES/glext.h>

//#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/video/background_segm.hpp"
//#include "opencv2/gpu/gpu.hpp"

//#include "pool.h"
//#include "detector.h"

//extern SocketServer socket;	

#define TEXTURE_BALL			0
#define TEXTURE_CUE				1
#define TEXTURE_CONNECTION		2
#define TEXTURE_CLICK_FINISH	3
#define	TEXTURE_CONNECTING_MSG	4
#define	TEXTURE_NO_CASSAPA		5
#define LAST_TEXTURE			6

class Preview : public QGLWidget
{
	Q_OBJECT

public:
	Preview(QWidget *parent = 0);
	~Preview();
	void Initialize(void);
	void Draw(bool AddAimingToVideo);
	void StartRectangle(int id);
	void SetRectangle(ScreenLine rect);
	void GetRectangle(ScreenLine &rect);
	int GettingRectangle(void);
	//void StartVideo(void);
	//void EndVideo(void);
	int mouseX, mouseY;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mouseMoveEvent(QMouseEvent  *event);
	void mousePressEvent(QMouseEvent  *event);
	void mouseDoubleClickEvent (QMouseEvent  *event);
	void mouseReleaseEvent(QMouseEvent  *event);

protected:
	//QImage gldata;

private:
	Ui::Preview ui;
	void Line(int x1, int y1, int x2, int y2);
	void Rectangle(int x1, int y1, int x2, int y2);
	void FilledRectangle(int x1, int y1, int x2, int y2);
	void Circle(float x, float y, float radius);
	void DrawCrossedCircleAroundBall(int ball);
	void drawBallMarker(float x, float y, float radius);
	void ValidateMouse(int &mouseX, int &mouseY);

	int hatchCounter;
	void DrawAids(void);
	void LoadImage(QImage &img, QString name, int textureIndex);
	void DrawImage(int x, int y, int w, int h, int id);
	void DrawText(QString text,float x,float y, float z, float red, float green, float blue, float alpha);
	
	int drawingRectangle;
	ScreenLine ortho;
	ScreenLine mouseRect;
	GLuint textures[LAST_TEXTURE];
	//QOpenGLTexture *texture;
	QImage imgTextBall, imgTextCue, imgTextConnection, imgClickToFinish;
	QImage imgConnectionMessage, imgNoCassapa;

	//cv::VideoWriter outputVideo;  
	//QImage frameImage;
	int mode;
	bool configBorderTopLeft;
	int configBorderX1, configBorderY1;
	int configBorderX2, configBorderY2;
	Menu * menu;
	ConfigureColors * colorConf;
	SocketServer socket;
	bool lostConnection;
	float lostConnectionAlpha;
	int initializationError;
	
	//int rectX1, rectX2, rectY1, rectY2;

private slots:
	void GetImage(void);
	void PingServer(void);

};

#endif // PREVIEW_H
