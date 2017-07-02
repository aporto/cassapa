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
#include "menu.h"

//-----------------------------------------------------------------------------

Menu::Menu(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags( Qt::CustomizeWindowHint );
	//setModal( true );

	QPixmap pixmap = QPixmap (":kassapa_viewer/Resources/images/config_cue.png");
	ui.button1->setIcon(pixmap);
	ui.button1->setIconSize(QSize(64, 64));
	
	pixmap = QPixmap (":kassapa_viewer/Resources/images/config_balls.png");	
	ui.button2->setIcon(pixmap);
	ui.button2->setIconSize(QSize(64, 64));
	
	pixmap = QPixmap (":kassapa_viewer/Resources/images/config_borders.png");	
	ui.button3->setIcon(pixmap);
	ui.button3->setIconSize(QSize(64, 64));

	pixmap = QPixmap (":kassapa_viewer/Resources/images/config_return.png");	
	ui.buttonCancel->setIcon(pixmap);
	ui.buttonCancel->setIconSize(QSize(64, 64));

	pixmap = QPixmap (":kassapa_viewer/Resources/images/config_projector.png");	
	ui.button4->setIcon(pixmap);
	ui.button4->setIconSize(QSize(64, 64));

	iconFire = QPixmap (":kassapa_viewer/Resources/images/fire_icon.png");	
	iconIce = QPixmap (":kassapa_viewer/Resources/images/ice_icon.png");	
	ui.button5->setIcon(iconFire);
	ui.button5->setIconSize(QSize(64, 64));

	pixmap = QPixmap (":kassapa_viewer/Resources/images/config_restart.png");	
	ui.button6->setIcon(pixmap);
	ui.button6->setIconSize(QSize(64, 64));
	
	ui.button1->setText("");
	ui.button2->setText("");
	ui.button3->setText("");
	ui.button4->setText("");
	ui.button5->setText("");
	ui.button6->setText("");
	ui.buttonCancel->setText("");

	connect(ui.button1, SIGNAL(clicked(bool)), this, SLOT(OnButtonConfigCue(bool)));
	connect(ui.button2, SIGNAL(clicked(bool)), this, SLOT(OnButtonConfigBalls(bool)));
	connect(ui.button3, SIGNAL(clicked(bool)), this, SLOT(OnButtonConfigBorders(bool)));	
	connect(ui.button4, SIGNAL(clicked(bool)), this, SLOT(OnButtonConfigProjector(bool)));	
	connect(ui.button5, SIGNAL(clicked(bool)), this, SLOT(OnButtonConfigProcessor(bool)));	
	connect(ui.button6, SIGNAL(clicked(bool)), this, SLOT(OnButtonRestart(bool)));	
	connect(ui.buttonCancel, SIGNAL(clicked(bool)), this, SLOT(OnButtonCancel(bool)));
}

//-----------------------------------------------------------------------------

Menu::~Menu()
{

}

//-----------------------------------------------------------------------------

int Menu::Display(void)
{
	result = -1;
	show();
	exec();
	return result;
}

//-----------------------------------------------------------------------------

void Menu::SetProcessorFireIcon(bool fire)
{
	if (fire) {
		ui.button5->setIcon(iconFire);
	} else {
		ui.button5->setIcon(iconIce);
	}
}

//-----------------------------------------------------------------------------

void Menu::OnButtonConfigCue(bool checked)
{
	//configImage.ConfigCue();
	result = BUTTON_CONFIG_CUE;
	close();
}

//-----------------------------------------------------------------------------

void Menu::OnButtonConfigBalls(bool checked)
{
	//configImage.ConfigBalls();
	result = BUTTON_CONFIG_BALLS;
	close();
}

//-----------------------------------------------------------------------------

void Menu::OnButtonConfigBorders(bool checked)
{
	//configImage.ConfigBorders();
	result = BUTTON_CONFIG_BORDER;
	close();
}

//-----------------------------------------------------------------------------

void Menu::OnButtonConfigProjector(bool checked)
{
	result = BUTTON_CONFIG_PROJECTOR;
	close();
}

//-----------------------------------------------------------------------------

void Menu::OnButtonCancel(bool checked)
{
	result = BUTTON_CONFIG_CANCEL;
	close();
}

//-----------------------------------------------------------------------------

void Menu::OnButtonConfigProcessor(bool checked)
{
	result = BUTTON_CONFIG_PROCESSOR;
	close();
}

//-----------------------------------------------------------------------------


void Menu::OnButtonRestart(bool checked)
{
	result = BUTTON_CONFIG_RESTART;
	close();
}

//-----------------------------------------------------------------------------

