#ifndef KASSAPA_VIEWER_H
#define KASSAPA_VIEWER_H

#include <QtWidgets/QMainWindow>

#include "ui_kassapa_viewer.h"

#include <config_viewer.h>
#include "preview.h"
//#include <socket_server.h>
//#include "projector.h"
//#include "colorselector.h"
//#include "cueconfig.h"
//#include "Tone.h"

class kassapa_viewer : public QMainWindow
{
	Q_OBJECT

public:
	kassapa_viewer(QWidget *parent = 0);
	~kassapa_viewer();

private:
	Ui::kassapa_viewerClass ui;

	Preview * preview;	

};

#endif // KASSAPA_VIEWER_H
