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
#ifndef POOL_DEFINITIONS_H
#define POOL_DEFINITIONS_H
//-----------------------------------------------------------------------------

const float DEG2RAD = 3.14159/180;

//-----------------------------------------------------------------------------

#include <qmutex.h>

extern QMutex socketMutex;

//-----------------------------------------------------------------------------

#define	CASSAPA_VERSION_TEXT			"1"
#define MAXIMUM_DAYS_TRIAL_LICENSE		60
#define MAXIMUM_LICENSE_DATE			150630

//-----------------------------------------------------------------

#define CASSAPA_ERROR_COULD_NOT_GET_CAMERA_IMAGE			-1
#define CASSAPA_ERROR_LICENCE_INSTALL_DATE_NOT_FOUND		-2
#define CASSAPA_ERROR_LICENCE_PAGEFILE_NOT_FOUND			-3
#define CASSAPA_ERROR_TODAY_IS_PAST_TO_LAST_RUN_DATE		-3
#define CASSAPA_ERROR_MAXIMUM_INSTALL_DATE_REACHED		-4
#define CASSAPA_ERROR_TODAY_IS_PAST_INSTALL_DATE			-5
#define CASSAPA_ERROR_MAXIMUM_LICENSE_PERIOD_REACHED		-6

//-----------------------------------------------------------------------------

struct Limits {
	int l, u;
};

struct ScreenPoint {
	float x, y;	
};

struct LineEquation {
	float m;
	float h;
};

#pragma pack(push, 1)

//-----------------------------------------------------------------------------

struct ScreenLine {
	float x1, y1, x2, y2;		
};

//-----------------------------------------------------------------------------

struct HSV {
	int lh, uh, ls, us, lv, uv;
};

//-----------------------------------------------------------------------------

struct CueConf {
	int imgId;

	int precisionLevel;

	int erodeSize;
	int dilateSize;

	double hlp_rho;
	double hlp_theta;
	int hlp_threshold;
	int hlp_minLength;
	int hlp_maxGap;

	double hl_rho;
	double hl_theta;
	int hl_threshold;

	int canny_threshold1;
	int canny_threshold2;
	int canny_apertureSize;
	bool canny_L2gradient;
	bool canny_apply;
};

//-----------------------------------------------------------------------------

struct PoolInput {
	unsigned int command;
	char terminate;
	int cameraId;
	char processorUsage;
	int ballSize;
	int holeWidth;
	char videoFileName[256];
	char debugMode;
	char useKeyboard;
	int initialVideoPos, finalVideoPos;
	HSV cueColors;
	HSV tableColors;
	CueConf cue;
	int limitX1, limitX2, limitY1, limitY2;
	ScreenPoint projectorLimitsPoints[4];
	int projectorMode;
};

//-----------------------------------------------------------------------------

struct PoolOutput {
	unsigned int command;
	qint64 rxTimeTick;
	int processorUsage;
	int errorCode;
	bool cueFound;
	float frameTime;
	bool updating;
	int whiteBall;
	int hitBall;
	int numbBalls;
	char hasProjector;
	ScreenLine cue;
	ScreenPoint balls[30];
	ScreenLine whiteTrajectory[3];
	int whiteTrajectoryCount;
	ScreenLine hitTrajectory[3];
	int hitTrajectoryCount;
	ScreenPoint tableLimitPoints[4];
	ScreenPoint projectorLimitsPoints[4];
	int projectorModeCopy;
	unsigned int imageRequestCount;
	unsigned int imageTransmitCount;

};

//-----------------------------------------------------------------------------


#pragma pack (pop)

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------