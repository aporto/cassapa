#ifndef CONFIGBORDERS_H
#define CONFIGBORDERS_H

#include <QWidget>
#include <QPainter>
#include "ui_configborders.h"

class ConfigBorders : public QWidget
{
	Q_OBJECT

public:
	ConfigBorders(QWidget *parent = 0);
	~ConfigBorders();

private:
	Ui::ConfigBorders ui;

protected:
	 void paintEvent(QPaintEvent *event);
};

#endif // CONFIGBORDERS_H
