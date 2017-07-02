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
#ifndef __CONFIG__H__
#define __CONFIG__H__
//-----------------------------------------------------------------------------

//#include "windows.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <QSettings>
#include <QtWidgets/QApplication>

//#include "INIReader.h"
#include "pool_utils.h"
#include "pool_definitions.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

/*struct ProjectorConf {
	float topBottomRatio;
	int angle;
	ScreenLine box;
	bool showCalibrationBars;
	float yGama;
	int calibBarsCount;
};

struct SoundConf {
	float volume;	
};*/

//-----------------------------------------------------------------------------


class ConfigServer {
public:
	ConfigServer (void);
	~ConfigServer (void);
	void Load(QString path);
	void Save(void);

	//bool showCue;
	//float light;
	bool configuring;
	QString appPath;
	bool loadVideoFile;

	PoolInput input;
	QString videoFilePath;
	PoolOutput output;

	QString serverIP;
	unsigned short serverUDPPort;
	unsigned short serverTCPPort;
	
	//int cueHistorySize;
	//SoundConf sound;

	//ProjectorConf projector;
	
	//bool cueConfiguring;

	HSV defaultCueColors;
	HSV defaultTableColors;

	
	//int projectorX1, projectorY1, projectorX2, projectorY2;
	
private:	
	//string iniFile;
	QString iniFile;
};

//-----------------------------------------------------------------------------

extern ConfigServer config;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------