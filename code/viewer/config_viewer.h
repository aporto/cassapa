//-----------------------------------------------------------------------------
#ifndef __CONFIG__H__
#define __CONFIG__H__
//-----------------------------------------------------------------------------

//#define NOMINMAX
//#include "windows.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <QSettings>
#include <QWidget>
#include <QDir>
#include <QtWidgets/QApplication>

#include "pool_definitions.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

struct ProjectorConf {
	float topBottomRatio;
	int angle;
	ScreenLine box;
	bool showCalibrationBars;
	float yGama;
	int calibBarsCount;
};

struct SoundConf {
	float volume;	
};

class ConfigViewer {
public:
	ConfigViewer (void);
	~ConfigViewer (void);
	void Load(QString path);
	void Save(void);

	PoolInput input;
	PoolOutput output;
	bool isMainController;
	
	HSV defaultCueColors;
	HSV defaultTableColors;

	QString serverIP;
	unsigned short serverUDPPort;
	unsigned short serverTCPPort;

	bool showCue;
	float light;
	bool configuring;
	QString appPath;
	int DataReceiverCounter;
	
	SoundConf sound;
	ProjectorConf projector;	

	//QImage rxImage;
	
private:	
	QString iniFile;
	
};

//-----------------------------------------------------------------------------

extern ConfigViewer config;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
