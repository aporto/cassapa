#ifndef CONFIGURECOLORS_H
#define CONFIGURECOLORS_H

#include <QWidget>
//nclude <QtWidgets/QMainWindow>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QPixmap>
#include "ui_configurecolors.h"

#include "ctkDoubleRangeSlider.h"

#include "config_viewer.h"
#include "image_receiver.h"

class ConfigureColors : public QDialog
{
	Q_OBJECT

public:
	ConfigureColors(QWidget *parent = 0);
	~ConfigureColors();
	bool SetCue(void);
	bool SetBalls(void);

private:
	Ui::ConfigureColors ui;	
	cv::Mat image;
	HSV hsv;	

	QLabel * labelMin1, *labelMax1;
	QLabel * labelMin2, *labelMax2;
	QLabel * labelMin3, *labelMax3;
	ctkDoubleRangeSlider * sliderH;
	ctkDoubleRangeSlider * sliderS;
	ctkDoubleRangeSlider * sliderV;
	QLabel * labelImage;
	bool cue;
	bool ok;

	void PrepareImage(void);	
	void UpdateImage(void);

private slots:
	void OnHvaluesChanged(double minVal, double maxVal);
	void OnSvaluesChanged(double minVal, double maxVal);
	void OnVvaluesChanged(double minVal, double maxVal);
	void OnButtonApply(bool checked);
	void OnButtonDefault(bool checked);
	void OnButtonCancel(bool checked);
};

#endif // CONFIGURECOLORS_H

