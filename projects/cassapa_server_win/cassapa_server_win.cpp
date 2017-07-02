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

#include "cassapa_server_win.h"

cassapa_server_win::cassapa_server_win(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowTitle ("Cassapa");
	setAttribute(Qt::WA_QuitOnClose);

	ui.lblStatus1->setText("Initializing...");
	ui.lblStatus2->setText("");
	ui.lblStatus3->setText("");
	ui.lblStatus4->setText("");

	charac = "|";

	thread.start();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(OnUpdate()));
	timer->start(1000);
}

cassapa_server_win::~cassapa_server_win()
{
	thread.Finish();
}

void cassapa_server_win::OnUpdate(void)
{
	if (config.input.terminate == 0) {
		if (charac == "|") charac = "/";
		else if (charac == "/") charac = "-";
		else if (charac == "-") charac = "\\";
		else if (charac == "\\") charac = "|";

		thread.Lock();
		if (thread.GetError() == 0) {
			ui.lblStatus1->setText("camera OK");
		} else {
			ui.lblStatus1->setText("Camera ERROR!!!");
		}
		ui.lblStatus2->setText("Listening port : " + thread.GetServerStatus());
		ui.lblStatus3->setText("Clients : " + thread.GetClientStatus() + " " + QString(charac));
		ui.lblStatus4->setText("Frame rate : " + QString::number(1000.0/thread.GetFrameTime(), 'f', 1) + 
			" fps (" + QString::number(thread.GetFrameTime()) + " ms)");
		
		thread.Unlock();
	} else {
		if (config.input.terminate == 1) {
			QApplication::exit(1);
		} else {
			QApplication::exit(2);
		}
	}
	update();
}
