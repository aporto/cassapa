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

#include "main_thread.h"

//-----------------------------------------------------------------------------

DetectorThread::DetectorThread(void)
	: server(this, true),
	pool(this)

{
	//SocketServer server(this->parent(), true);
	cont = server.Start(config.serverUDPPort, &(config.input), &(config.output));//;, NULL);
	memcpy(&(pool.detector.configInput), &(config.input), sizeof(PoolInput));
	memcpy(&(pool.detector.configOutput), &(config.output), sizeof(PoolOutput));
	ok = pool.Initialize();
}

void DetectorThread::run()
{
	
	int statCounter = 0;	
	
	QElapsedTimer timerDataFlow, timerStats, timerImage;
	timerDataFlow.start();    		
	timerStats.start();    		
	timerImage.start();   

	while (cont) {
		socketMutex.lock();
		memcpy(&(pool.detector.configInput), &(config.input), sizeof(PoolInput));
		socketMutex.unlock();

		if (pool.detector.configInput.processorUsage == 0) {
			msleep(100);	
		}

		remoteHost = server.GetRemoteHost();
		localHost = server.GetServerAddress();

		cont = cont && (pool.finish == false) && (config.input.terminate == 0);
		if (cont) {
			cont = cont && pool.Sample(true);
		}

		socketMutex.lock();
		
		++statCounter;		
		if (statCounter == 20) {
			pool.detector.configOutput.frameTime = float (timerStats.elapsed()) / 20.0;
			timerStats.start();    		
			statCounter = 0;
		}	

		memcpy(&(config.output), &(pool.detector.configOutput), sizeof(PoolOutput));		


		// relay arguments

		memcpy(&(config.output.projectorLimitsPoints), &(config.input.projectorLimitsPoints), sizeof(ScreenPoint) * 4); 
		config.output.projectorModeCopy = config.input.projectorMode;
		config.output.tableLimitPoints[0].x = config.input.limitX1;
		config.output.tableLimitPoints[0].y = config.input.limitY1;			

		config.output.tableLimitPoints[1].x = config.input.limitX2;
		config.output.tableLimitPoints[1].y = config.input.limitY1;			

		config.output.tableLimitPoints[2].x = config.input.limitX2;
		config.output.tableLimitPoints[2].y = config.input.limitY2;			

		config.output.tableLimitPoints[3].x = config.input.limitX1;
		config.output.tableLimitPoints[3].y = config.input.limitY2;	

		config.output.processorUsage = config.input.processorUsage;

		// send output data
		if (timerDataFlow.elapsed() >= 5) { 
			timerDataFlow.start();
			server.SendCalculations();
		}
		socketMutex.unlock();		
		
	}	
	msleep(100);	

	config.Save();
}

//-----------------------------------------------------------------------------

void DetectorThread::Finish(void)
{
	cont = false;
	config.input.terminate = 2;
	msleep(500);
}

//-----------------------------------------------------------------------------

int DetectorThread::Terminated(void)
{
	return config.input.terminate;
}

//-----------------------------------------------------------------------------

float DetectorThread::GetFrameTime(void)
{
	return config.output.frameTime;
}

//-----------------------------------------------------------------------------

QString DetectorThread::GetServerStatus(void)
{
	return localHost;
}

//-----------------------------------------------------------------------------

QString DetectorThread::GetClientStatus(void)
{
	return remoteHost;
}

//-----------------------------------------------------------------------------

int DetectorThread::GetError(void)
{
	if (ok) {
		return 0;
	} else {
		return -1;
	}
}

//-----------------------------------------------------------------------------

void DetectorThread::Lock(void)
{
	socketMutex.lock();
}

//-----------------------------------------------------------------------------

void DetectorThread::Unlock(void)
{
	socketMutex.unlock();
}

//-----------------------------------------------------------------------------
