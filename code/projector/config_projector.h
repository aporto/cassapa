//-----------------------------------------------------------------------------
#ifndef __CONFIG__H__
#define __CONFIG__H__
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <QSettings>
#include <QWidget>
#include <QtWidgets/QApplication>

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
};*/


class ConfigProjector {
public:
	ConfigProjector (void);
	~ConfigProjector (void);
	void Load(QString path);
	void Save(void);

	PoolInput input;
	PoolOutput output;

	QString serverIP;
	unsigned short serverUDPPort;
	unsigned short serverTCPPort;

	QString appPath;
	int DataReceiverCounter;	

private:	
	QString iniFile;
	
};

//-----------------------------------------------------------------------------

extern ConfigProjector config;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
