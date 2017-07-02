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
#include "config_projector.h"

//-----------------------------------------------------------------------------

ConfigProjector config;

//-----------------------------------------------------------------------------

ConfigProjector::ConfigProjector (void)
{	
	output.rxTimeTick = 0;
	input.terminate = false;
};


//-----------------------------------------------------------------------------

ConfigProjector::~ConfigProjector ()	
{
	Save();
}

//-----------------------------------------------------------------------------

void ConfigProjector::Load(QString path)
{
	appPath = path;
	iniFile = path + "/cassapa_projector.ini";
	input.terminate = false;
	
	QSettings settings(iniFile, QSettings::IniFormat);
	settings.beginGroup("main");
	
	serverIP = settings.value("serverIP", "127.0.0.1").toString();
	serverUDPPort = settings.value("serverUDPPort", 30777).toInt();
	serverTCPPort = settings.value("serverTCPPort", 30778).toInt();	
	
	settings.endGroup();
}

void ConfigProjector::Save(void)
{
	QSettings settings(iniFile, QSettings::IniFormat);
	settings.beginGroup("main");

	settings.setValue("serverIP", serverIP);
	settings.setValue("serverUDPPort", serverUDPPort);
	settings.setValue("serverTCPPort", serverTCPPort);

	settings.endGroup();	
}

