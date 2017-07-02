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
#ifndef __POOL__H__
#define __POOL__H__
//-----------------------------------------------------------------------------

#include "detector.h"
//#include "config_detector.h"
#include "Aprofiler.h"


//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

class Pool : public QObject {
public:
	cv::Mat screen;
	int whiteBall, hitBall;
	vector <ScreenLine> whiteTrajectory;
	vector <ScreenLine> hitTrajectory;
	vector <ScreenLine> videoTrajectory;
	ScreenLine cueVector;
	ScreenLine forceAtWhiteBall;
	bool finish;

	Detector detector;

	Pool(QObject *parent);

	~Pool();
	bool Initialize(void);

	bool Sample(bool draw);
	void GoNext(void);
	void GoPrevious(void);
	cv::Mat * GetImgTable(void);
	cv::Mat * GetImgCue(void);
	int GetDistanceFromHitBallToNearestHole(void);
	//void FetchInput(void);
	//void FetchOutput(void);
	
private:
	int FindClosestBall(void);
	void CalculateBallTrajectory(int ball, ScreenLine line, vector <ScreenLine> &trajectory);
	void FindBallHitByThisBall(int ball, vector <ScreenLine> &trajectory, float &hitX, float &hitY);
	void CalculateCueVector(void);
	int CheckIfLineCrossesBall(int ball, int ray, ScreenLine line, float &hitX, float &hitY, bool fullLine);
};

//extern Pool pool;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------