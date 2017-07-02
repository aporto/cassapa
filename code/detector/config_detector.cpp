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
#include "config_detector.h"

//-----------------------------------------------------------------------------

ConfigServer config;

//-----------------------------------------------------------------------------

ConfigServer::ConfigServer (void)
{
	input.useKeyboard = false;
	//cueConfiguring = false;

	//input.cueHistorySize = 10;

	defaultTableColors.lh = 50;
	defaultTableColors.ls = 0;
	defaultTableColors.lv = 38;
	defaultTableColors.uh = 131;
	defaultTableColors.us = 256;
	defaultTableColors.uv = 227;

	// madeira 
	defaultCueColors.lh = 15;
	defaultCueColors.ls = 45;
	defaultCueColors.lv = 148;
	defaultCueColors.uh = 29;
	defaultCueColors.us = 228;
	defaultCueColors.uv = 256;	

	// Azul
	defaultCueColors.lh=94;
	defaultCueColors.ls=20;
	defaultCueColors.lv=157;
	defaultCueColors.uh=105;
	defaultCueColors.us=176;
	defaultCueColors.uv=255;
	
	input.cameraId = 0;

	//configuring = false;

	input.cue.imgId = -1;

	input.cue.precisionLevel = 1;

	input.cue.erodeSize = 1;
	input.cue.dilateSize = 1;

	input.cue.hlp_rho = 1;
	input.cue.hlp_theta = 3.1415/180.0;
	input.cue.hlp_threshold = 50;
	input.cue.hlp_minLength = 50;
	input.cue.hlp_maxGap = 3;

	input.cue.hl_rho = 0.1;//1;
	input.cue.hl_theta = 0.01;//3.1415/180;
	input.cue.hl_threshold = 100;

	input.cue.canny_threshold1 = 10;
	input.cue.canny_threshold2 = 10;
	input.cue.canny_apertureSize = 5;
	input.cue.canny_L2gradient = false;
	input.cue.canny_apply = true;
};


//-----------------------------------------------------------------------------

ConfigServer::~ConfigServer ()	
{
//	Save();
}

//-----------------------------------------------------------------------------

void ConfigServer::Load(QString path)
{
	appPath = path;
	iniFile = path + "/cassapa_server.ini";
	input.terminate = false;
	//iniFile = "kassapa.ini";

	QSettings settings(iniFile, QSettings::IniFormat);
	settings.beginGroup("private");
	serverIP = settings.value("serverIP", "127.0.0.1").toString();
	serverUDPPort = settings.value("serverUDPPort", 30777).toInt();
	serverTCPPort = settings.value("serverTCPPort", 30778).toInt();	
	videoFilePath = settings.value("videoFilePath", videoFilePath).toString();	
	loadVideoFile = settings.value("loadVideoFile", false).toBool();	
	
	settings.endGroup();
	settings.beginGroup("generic");
	
	input.cameraId = settings.value("cameraId", 0).toInt();
	input.ballSize = settings.value("ballSize", 12).toInt();
	input.holeWidth = settings.value("holeWidth", 17).toInt();
	input.debugMode = settings.value("debugMode", false).toBool();
	input.useKeyboard = settings.value("useKeyboard", false).toBool();
	input.initialVideoPos = settings.value("initialVideoPos", -1).toInt();
	input.finalVideoPos = settings.value("finalVideoPos", -1).toInt();
	input.processorUsage = settings.value("processorUsage", 1).toInt();
	
	QString text = settings.value("videoFileName", "").toString();
	strcpy(input.videoFileName, text.toLocal8Bit());

	input.limitX1 = settings.value("limitX1", 0).toInt();
	input.limitY1 = settings.value("limitY1", 0).toInt();
	input.limitX2 = settings.value("limitX2", 640).toInt();
	input.limitY2 = settings.value("limitY2", 480).toInt();
	
	input.tableColors.lh = settings.value("tableColors.lh", 50).toInt();
	input.tableColors.ls = settings.value("tableColors.ls", 0).toInt();
	input.tableColors.lv = settings.value("tableColors.lv", 38).toInt();
	input.tableColors.uh = settings.value("tableColors.uh", 131).toInt();
	input.tableColors.us = settings.value("tableColors.us", 256).toInt();
	input.tableColors.uv = settings.value("tableColors.uv", 227).toInt();
	
	input.cueColors.lh = settings.value("cueColors.lh", 15).toInt();
	input.cueColors.ls = settings.value("cueColors.ls", 45).toInt();
	input.cueColors.lv = settings.value("cueColors.lv", 148).toInt();
	input.cueColors.uh = settings.value("cueColors.uh", 29).toInt();
	input.cueColors.us = settings.value("cueColors.us", 228).toInt();
	input.cueColors.uv = settings.value("cueColors.uv", 256).toInt();
	
	settings.endGroup();

	settings.beginGroup("cue");
	input.cue.precisionLevel= settings.value("precisionLevel", input.cue.precisionLevel).toInt();

	input.cue.erodeSize= settings.value("erodeSize", input.cue.erodeSize).toInt();
	input.cue.dilateSize= settings.value("dilateSize", input.cue.dilateSize).toInt();
	

	input.cue.hlp_rho= settings.value("hlp_rho", input.cue.hlp_rho).toDouble();
	input.cue.hlp_theta= settings.value("hlp_theta", input.cue.hlp_theta).toDouble();
	input.cue.hlp_threshold= settings.value("hlp_threshold", input.cue.hlp_threshold).toInt();
	input.cue.hlp_minLength= settings.value("hlp_minLength", input.cue.hlp_minLength).toInt();
	input.cue.hlp_maxGap= settings.value("hlp_maxGap", input.cue.hlp_maxGap).toInt();	
	
	input.cue.hl_rho= settings.value("hl_rho", input.cue.hl_rho).toDouble();
	input.cue.hl_theta= settings.value("hl_theta", input.cue.hl_theta).toDouble();
	input.cue.hl_threshold= settings.value("hl_threshold", input.cue.hl_threshold).toInt();

	input.cue.canny_threshold1= settings.value("canny_threshold1", input.cue.canny_threshold1).toInt();
	input.cue.canny_threshold2= settings.value("canny_threshold2", input.cue.canny_threshold2).toInt();
	input.cue.canny_apertureSize= settings.value("canny_apertureSize", input.cue.canny_apertureSize).toInt();
	input.cue.canny_L2gradient= settings.value("canny_L2gradient", input.cue.canny_L2gradient).toBool();
	input.cue.canny_apply= settings.value("canny_apply", input.cue.canny_apply).toBool();
	settings.endGroup();

	settings.beginGroup("projector");
	input.projectorLimitsPoints[0].x = settings.value("projectorLimitsPoints0x", 50).toFloat();
	input.projectorLimitsPoints[0].y = settings.value("projectorLimitsPoints0y", 50).toFloat();
	input.projectorLimitsPoints[1].x = settings.value("projectorLimitsPoints1x", 600).toFloat();
	input.projectorLimitsPoints[1].y = settings.value("projectorLimitsPoints1y", 50).toFloat();
	input.projectorLimitsPoints[2].x = settings.value("projectorLimitsPoints2x", 600).toFloat();
	input.projectorLimitsPoints[2].y = settings.value("projectorLimitsPoints2y", 430).toFloat();
	input.projectorLimitsPoints[3].x = settings.value("projectorLimitsPoints3x", 50).toFloat();
	input.projectorLimitsPoints[3].y = settings.value("projectorLimitsPoints3y", 430).toFloat();
	
	settings.endGroup();
}

void ConfigServer::Save(void)
{
	QSettings settings(iniFile, QSettings::IniFormat);
	settings.beginGroup("private");
	settings.setValue("serverIP", serverIP);
	settings.setValue("serverUDPPort", serverUDPPort);
	settings.setValue("serverTCPPort", serverTCPPort);
	settings.setValue("videoFilePath", videoFilePath);
	settings.setValue("loadVideoFile", loadVideoFile);
	settings.endGroup();

	settings.beginGroup("generic");
	settings.setValue("cameraId", input.cameraId);	
	settings.setValue("ballSize", input.ballSize);
	settings.setValue("holeWidth", input.holeWidth);
	settings.setValue("debugMode", input.debugMode);
	settings.setValue("useKeyboard", input.useKeyboard);	
	settings.setValue("initialVideoPos", input.initialVideoPos);
	settings.setValue("finalVideoPos", input.finalVideoPos);	
	settings.setValue("processorUsage", input.processorUsage);	
	
	settings.setValue("videoFileName", input.videoFileName);

	settings.setValue("limitX1", input.limitX1);
	settings.setValue("limitY1", input.limitY1);
	settings.setValue("limitX2", input.limitX2);
	settings.setValue("limitY2", input.limitY2);

	
	/*settings.setValue("projector.X1", QString::number(projector.box.x1,'f',5));
	settings.setValue("projector.Y1", QString::number(projector.box.y1,'f',5));
	settings.setValue("projector.X2", QString::number(projector.box.x2,'f',5));
	settings.setValue("projector.Y2", QString::number(projector.box.y2,'f',5));

	settings.setValue("projector.topBottomRatio", QString::number(projector.topBottomRatio,'f',5));
	settings.setValue("projector.showCalibrationBars", projector.showCalibrationBars);
	settings.setValue("projector.yGama", QString::number(projector.yGama,'f',5));
	settings.setValue("projector.calibBarsCount", projector.calibBarsCount);
	settings.setValue("projector.angle", projector.angle);
	*/

	settings.setValue("tableColors.lh", input.tableColors.lh);
	settings.setValue("tableColors.ls", input.tableColors.ls );
	settings.setValue("tableColors.lv", input.tableColors.lv );
	settings.setValue("tableColors.uh", input.tableColors.uh);
	settings.setValue("tableColors.us", input.tableColors.us);
	settings.setValue("tableColors.uv", input.tableColors.uv );

	settings.setValue("cueColors.lh", input.cueColors.lh);
	settings.setValue("cueColors.ls", input.cueColors.ls);
	settings.setValue("cueColors.lv", input.cueColors.lv);
	settings.setValue("cueColors.uh", input.cueColors.uh);
	settings.setValue("cueColors.us", input.cueColors.us);
	settings.setValue("cueColors.uv", input.cueColors.uv);
	
	settings.endGroup();
	
	settings.beginGroup("cue");
	settings.setValue("precisionLevel", input.cue.precisionLevel);
	settings.setValue("erodeSize", input.cue.erodeSize);
	settings.setValue("dilateSize", input.cue.dilateSize);

	settings.setValue("hlp_rho", input.cue.hlp_rho);
	settings.setValue("hlp_theta", input.cue.hlp_theta);
	settings.setValue("hlp_threshold", input.cue.hlp_threshold);
	settings.setValue("hlp_minLength", input.cue.hlp_minLength);
	settings.setValue("hlp_maxGap", input.cue.hlp_maxGap);
	settings.setValue("hl_rho", input.cue.hl_rho);
	settings.setValue("hl_theta", input.cue.hl_theta);
	settings.setValue("hl_threshold", input.cue.hl_threshold);
	
	settings.setValue("canny_threshold1", input.cue.canny_threshold1);
	settings.setValue("canny_threshold2", input.cue.canny_threshold2);
	settings.setValue("canny_apertureSize", input.cue.canny_apertureSize);
	settings.setValue("canny_L2gradient", input.cue.canny_L2gradient);
	settings.setValue("canny_apply", input.cue.canny_apply);
	settings.endGroup();

	settings.beginGroup("projector");
	settings.setValue("projectorLimitsPoints0x", QString::number(input.projectorLimitsPoints[0].x,'f',5));
	settings.setValue("projectorLimitsPoints0y", QString::number(input.projectorLimitsPoints[0].y,'f',5));
	settings.setValue("projectorLimitsPoints1x", QString::number(input.projectorLimitsPoints[1].x,'f',5));
	settings.setValue("projectorLimitsPoints1y", QString::number(input.projectorLimitsPoints[1].y,'f',5));
	settings.setValue("projectorLimitsPoints2x", QString::number(input.projectorLimitsPoints[2].x,'f',5));
	settings.setValue("projectorLimitsPoints2y", QString::number(input.projectorLimitsPoints[2].y,'f',5));
	settings.setValue("projectorLimitsPoints3x", QString::number(input.projectorLimitsPoints[3].x,'f',5));
	settings.setValue("projectorLimitsPoints3y", QString::number(input.projectorLimitsPoints[3].y,'f',5));

	
	settings.endGroup();	
}


