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

#include "configurecolors.h"

ConfigureColors::ConfigureColors(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setAttribute( Qt::WA_QuitOnClose, false);

	QHBoxLayout* hLayout = new QHBoxLayout;
	QVBoxLayout* vLayout1 = new QVBoxLayout;
	QVBoxLayout* vLayout2 = new QVBoxLayout;
	QVBoxLayout* vLayout3 = new QVBoxLayout;
	QVBoxLayout* vLayout4 = new QVBoxLayout;

	hLayout->addLayout(vLayout1, 1);
	hLayout->addLayout(vLayout2, 1);
	hLayout->addLayout(vLayout3, 1);
	hLayout->addLayout(vLayout4, 10);

	QLabel * labelTitle1 = new QLabel(this);
	labelTitle1->setText("Hue");
	sliderH = new ctkDoubleRangeSlider(this);	
	labelMin1 = new QLabel(this);
	labelMax1 = new QLabel(this);
	//labelTitle1->setMaximumWidth(25);
	labelMin1->setMaximumWidth(25);
	labelMax1->setMaximumWidth(25);
	sliderH->setMaximumWidth(25);
	vLayout1->addWidget(labelTitle1);
	vLayout1->addWidget(sliderH);
	vLayout1->addWidget(labelMin1);
	vLayout1->addWidget(labelMax1);

	QLabel * labelTitle2 = new QLabel(this);
	labelTitle2->setText("Saturation");
	sliderS = new ctkDoubleRangeSlider(this);	
	labelMin2 = new QLabel(this);
	labelMax2 = new QLabel(this);
	//labelTitle2->setMaximumWidth(25);
	labelMin2->setMaximumWidth(25);
	labelMax2->setMaximumWidth(25);
	sliderS->setMaximumWidth(25);
	vLayout2->addWidget(labelTitle2);
	vLayout2->addWidget(sliderS);
	vLayout2->addWidget(labelMin2);
	vLayout2->addWidget(labelMax2);

	QLabel * labelTitle3 = new QLabel(this);
	labelTitle3->setText("Value");
	sliderV = new ctkDoubleRangeSlider(this);	
	labelMin3 = new QLabel(this);
	labelMax3 = new QLabel(this);
	//labelTitle3->setMaximumWidth(25);
	labelMin3->setMaximumWidth(25);
	labelMax3->setMaximumWidth(25);
	sliderV->setMaximumWidth(25);
	vLayout3->addWidget(labelTitle3);
	vLayout3->addWidget(sliderV);
	vLayout3->addWidget(labelMin3);
	vLayout3->addWidget(labelMax3);

	labelImage = new QLabel(this);
	labelImage->setText("");
	labelImage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//labelImage->stre
	vLayout4->addWidget(labelImage, 100);	
	//vLayout4->addStretch

	QPushButton *button1 = new QPushButton(this);
	button1->setText("Apply ");
	button1->setMaximumWidth(75);
	QPushButton *button2 = new QPushButton(this);
	button2->setText("Cancel");
	button2->setMaximumWidth(75);
	QPushButton *button3 = new QPushButton(this);
	button3->setText("Default");
	button3->setMaximumWidth(75);
	vLayout1->addWidget(button1);
	vLayout2->addWidget(button2);
	vLayout3->addWidget(button3);

	connect(button1, SIGNAL(clicked(bool)), this, SLOT(OnButtonApply(bool)));
	connect(button2, SIGNAL(clicked(bool)), this, SLOT(OnButtonCancel(bool)));
	connect(button3, SIGNAL(clicked(bool)), this, SLOT(OnButtonDefault(bool)));	

	setLayout(hLayout);

	sliderH->setMinimum(0);
	sliderH->setMaximum(255);
	sliderS->setMinimum(0);
	sliderS->setMaximum(255);
	sliderV->setMinimum(0);
	sliderV->setMaximum(255);	

	connect(sliderH, SIGNAL(valuesChanged(double, double)), this, SLOT(OnHvaluesChanged(double, double)));
	connect(sliderS, SIGNAL(valuesChanged(double, double)), this, SLOT(OnSvaluesChanged(double, double)));
	connect(sliderV, SIGNAL(valuesChanged(double, double)), this, SLOT(OnVvaluesChanged(double, double)));

}

ConfigureColors::~ConfigureColors()
{

}

void ConfigureColors::OnButtonApply(bool checked)
{
	if (cue) {
		config.input.cueColors = hsv;
	} else {
		config.input.tableColors = hsv;
	}
	ok = true;
	close();
}

void ConfigureColors::OnButtonDefault(bool checked)
{
	if (cue) {
		hsv = config.defaultCueColors;
	} else {
		hsv = config.defaultTableColors;
	}

	sliderH->setPositions(hsv.lh, hsv.uh);
	sliderS->setPositions(hsv.ls, hsv.us);
	sliderV->setPositions(hsv.lv, hsv.uv);
	
	UpdateImage();
}

void ConfigureColors::OnButtonCancel(bool checked)
{
	ok = false;
	close();
}

void ConfigureColors::OnHvaluesChanged(double minVal, double maxVal)
{
	hsv.lh = minVal;
	hsv.uh = maxVal;
	UpdateImage();
}

void ConfigureColors::OnSvaluesChanged(double minVal, double maxVal)
{
	hsv.ls = minVal;
	hsv.us = maxVal;
	UpdateImage();
}

void ConfigureColors::OnVvaluesChanged(double minVal, double maxVal)
{
	hsv.lv = minVal;
	hsv.uv = maxVal;
	UpdateImage();
}

void ConfigureColors::UpdateImage(void)
{
	if (image.size().width == 0) {
		return;
	}
	labelMin1->setText(QString().number(hsv.lh));
	labelMax1->setText(QString().number(hsv.uh));
	labelMin2->setText(QString().number(hsv.ls));
	labelMax2->setText(QString().number(hsv.us));
	labelMin3->setText(QString().number(hsv.lv));
	labelMax3->setText(QString().number(hsv.uv));

	//QImage qTemp = image;
	//qTemp = qTemp.convertToFormat(QImage::Format_RGB888);			
	//cv::Mat cvTemp(qTemp.height(), qTemp.width(),CV_8UC3,(uchar*)qTemp.bits(), qTemp.bytesPerLine());		

	cv::Mat cvTemp;	
	image.copyTo(cvTemp);
	//cv::cvtColor(image, cvTemp, CV_RGB2HSV); 
	cv::inRange(cvTemp, cv::Scalar(hsv.lh, hsv.ls, hsv.lv), 
				 cv::Scalar(hsv.uh, hsv.us, hsv.uv), cvTemp);
	if (cue == false) {
		cv::bitwise_not (cvTemp, cvTemp );
	}
	cv::cvtColor(cvTemp, cvTemp, CV_GRAY2RGB); 
	QImage qTemp = QImage((uchar*) cvTemp.data, cvTemp.cols, cvTemp.rows, cvTemp.step, QImage::Format_RGB888);
	int w = cvTemp.size().width;
	int h = cvTemp.size().height;
	//labelImage->setScaledContents(false);
	qTemp = qTemp.scaled(w, h, Qt::IgnoreAspectRatio, Qt::FastTransformation);//, Qt::SmoothTransformation);

	labelImage->setPixmap(QPixmap::fromImage(qTemp));//;QPixmap::fromImage(Hellkite_Tyrant));
	//imshow("Teste", cvTemp);
	labelImage->show();
}

void ConfigureColors::PrepareImage(void)
{
	sliderH->setPositions(hsv.lh, hsv.uh);
	sliderS->setPositions(hsv.ls, hsv.us);
	sliderV->setPositions(hsv.lv, hsv.uv);
	imageReceiver.mutex.lock();

	cv::cvtColor(imageReceiver.cvImage, image, CV_RGB2HSV);
	
	imageReceiver.mutex.unlock();

	UpdateImage();
}

bool ConfigureColors::SetCue(void)
{
	ok = false;
	cue = true;
	hsv = config.input.cueColors;
	PrepareImage();

	if (image.size().width == 0) {
		return false;
	}
	showMaximized();
	exec();

	return ok;
}

bool ConfigureColors::SetBalls(void)
{
	ok = false;

	cue = false;
	hsv = config.input.tableColors;
	PrepareImage();
	if (image.size().width == 0) {
		return false;
	}
	showMaximized();
	exec();

	return ok;
}
