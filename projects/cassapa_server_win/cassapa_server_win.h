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

#ifndef CASSAPA_SERVER_WIN_H
#define CASSAPA_SERVER_WIN_H

#include <QtWidgets/QMainWindow>
#include "ui_cassapa_server_win.h"

#include "main_thread.h"
#include <qtimer.h>

class cassapa_server_win : public QMainWindow
{
	Q_OBJECT

public:
	cassapa_server_win(QWidget *parent = 0);
	~cassapa_server_win();

private slots:
	void OnUpdate(void);

private:
	Ui::cassapa_server_winClass ui;

	DetectorThread thread;
	QString charac;
};

#endif // CASSAPA_SERVER_WIN_H
