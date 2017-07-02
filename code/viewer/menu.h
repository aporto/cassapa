//-----------------------------------------------------------------------------
#ifndef MENU_H
#define MENU_H

//-----------------------------------------------------------------------------

#include <QWidget>
#include <QDialog>
#include "ui_menu.h"

#define BUTTON_CONFIG_CUE		0
#define BUTTON_CONFIG_BALLS		1
#define BUTTON_CONFIG_BORDER	2	
#define BUTTON_CONFIG_PROJECTOR	3
#define BUTTON_CONFIG_PROCESSOR	4
#define BUTTON_CONFIG_CANCEL	5
#define BUTTON_CONFIG_RESTART	6

//-----------------------------------------------------------------------------

class Menu : public QDialog
{
	Q_OBJECT

public:
	Menu(QWidget *parent = 0);
	int Display(void);
	void SetProcessorFireIcon(bool fire);
	~Menu();	
	Ui::Menu ui;

private:
	int result;
	QPixmap iconFire, iconIce;

public slots:
	void OnButtonConfigCue(bool checked); //QPushButton * button);
	void OnButtonConfigBalls(bool checked);
	void OnButtonConfigBorders(bool checked);
	void OnButtonConfigProjector(bool checked);
	void OnButtonCancel(bool checked);
	void OnButtonConfigProcessor(bool checked);
	void OnButtonRestart(bool checked);	
};

//-----------------------------------------------------------------------------
#endif // MENU_H
//-----------------------------------------------------------------------------
