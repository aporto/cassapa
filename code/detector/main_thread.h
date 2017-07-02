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
#ifndef QMAIN_TREAD_H22__
#define QMAIN_TREAD_H22__
//-----------------------------------------------------------------------------

#include <qthread.h>
#include <QElapsedTimer>

#include "Pool.h"
#include "socket_server.h"

//-----------------------------------------------------------------------------

class DetectorThread : public QThread
{
	Q_OBJECT
public:
	DetectorThread(void);
	void Lock(void);
	void Unlock(void);
	float GetFrameTime(void);
	QString GetClientStatus(void);
	QString GetServerStatus(void);	
	void Finish(void);
	int GetError(void);
	int Terminated(void);

private:    
	bool cont;
	QString remoteHost, localHost;
	SocketServer server;
	Pool pool;
	bool ok;
	
	void run();
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------