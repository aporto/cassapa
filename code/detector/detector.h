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
#ifndef __DETECTOR__H__
#define __DETECTOR__H__
//-----------------------------------------------------------------------------

//#include <windows.h>

#include <cmath>

#include <QDir>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/gpu/gpu.hpp"

//#include <QGLWidget>

#include <iostream>
#include <stdio.h>

#include "pool_utils.h"
//#include "config_detector.h"
#include "AProfiler.h"
#include "image_transmitter.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

//#define CUE_HISTORY 5

//#define NUM_OBJECTS 3

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//extern HSV objColors[NUM_OBJECTS];
//extern Limits objLimits[NUM_OBJECTS];

//-----------------------------------------------------------------------------

class Detector : public QObject {
public:
	Detector(QObject *parent);
	bool Initialize(void);
	bool Sample(void);	
	void AdjustLineDirectionToPoolCenter(ScreenLine &line);
	void GoNext(void);
	void GoPrevious(void);
	//void GetMostCenterPoint (int x1, int y1, int x2, int y2, int &x, int &y);

	vector <ScreenPoint> balls;
	ScreenLine cue;	
	cv::Mat frame;
	cv::Mat originalImage;
	ScreenPoint screenLimits[2];
	cv::Rect tableLimits;

	cv::Mat imgTableRemoved;
	cv::Mat imgCue;
	int pos;

	cv::Rect cueROI;
	//QImage preview;

	PoolInput configInput;
	PoolOutput configOutput;
	
private:
	bool error;
	bool fromFile;
	ImageTransmitter imageTransmitter;
	cv::VideoCapture cap;

	float currentCueAngle, lastCueAngle;
	ScreenLine cueHistory[100];
	int cueHistoryCount;

	cv::Mat imgHSV;
	cv::Mat imgThresh;	

	cv::Mat erosionBall;
	cv::Mat erosionTip;
	cv::Mat erosionCue;	
	
	cv::Rect whiteBallROI;
	bool foundWhiteBall;

	void FindCue(void);
	void FindCue2(void);
	void FindAllBalls(void);
		
};

//extern Detector detector;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
