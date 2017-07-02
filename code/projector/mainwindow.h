#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QDesktopWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "ui_mainwindow.h"
#include "config_projector.h"

#include <socket_server.h>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow ui;
	
	SocketServer socket;
	bool lostConnection;
	float lostConnectionAlpha;
	int initializationError;
	int lostConnectionCounter;
	//cv::Mat inputImage, outputImage;
	//QImage imgDraw;
	QImage imgProjector;
	//QImage imgBack;
	cv::Mat cvBack, cvBackMask;
	//cv::Mat imgInput, imgOutput;

	cv::Mat homo;
	//std::vector<cv::Point2f> balls;
	float propX, propY;
	std::vector<cv::Point2f> whiteTrajectory;
	std::vector<cv::Point2f> hitTrajectory;

	void DrawCalibrationPattern(void);
	void DrawConnectionError(void);
	void DrawAidsOnImage(bool ok);
	//void DrawAidsOnImage2(void);
	bool WarpImage(void);
	void DrawVisualEffects(void);
	void DrawDistortedCircle(int start, QPainter &p, float x, float y, float radius);

protected:
	void resizeEvent(QResizeEvent* event);
	void mouseDoubleClickEvent (QMouseEvent  *event);

private slots:
	void RefreshScreen(void);
	void GetImage(void);
};

#endif // MAINWINDOW_H
