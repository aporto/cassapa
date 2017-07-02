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

#include "configborders.h"

ConfigBorders::ConfigBorders(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ConfigBorders::~ConfigBorders()
{

}

void ConfigBorders::paintEvent(QPaintEvent *)
 {
	 /*static const QPoint hourHand[3] = {
		 QPoint(7, 8),
		 QPoint(-7, 8),
		 QPoint(0, -40)
	 };
	 static const QPoint minuteHand[3] = {
		 QPoint(7, 8),
		 QPoint(-7, 8),
		 QPoint(0, -70)
	 };*/

	 QPen penBackground;
	 penBackground.setBrush(Qt::white);
	 penBackground.setStyle(Qt::SolidLine);
	 penBackground.setWidth(1);

	 QPen penTrack;
	 penTrack.setBrush(Qt::blue);
	 penTrack.setStyle(Qt::SolidLine);
	 penTrack.setWidth(1);

	 
	 int w = width();
	 int h = height();
	 int centerX = w / 2;
	 int centerY = h / 2;

	 QRectF rectBackground(0, 0, w, h);
	 QRectF rectTrack(30, centerY - 5, w - 10, centerY + 5);

	 QPainter painter(this);
	 painter.setRenderHint(QPainter::Antialiasing);
	 painter.setBrush(Qt::SolidPattern);

	 painter.setPen(penBackground);
	 painter.drawRect(rectBackground);
	 
	 painter.setPen(penTrack);
	 
	 painter.drawRect(rectTrack);
	 //painter.translate(width() / 2, height() / 2);
	 //painter.scale(side / 200.0, side / 200.0);

	 //painter.setPen(Qt::NoPen);
	 //painter.setBrush(hourColor);

	 //painter.save();
	 //painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
	 //painter.drawConvexPolygon(hourHand, 3);
	 //painter.restore();

	 //painter.setPen(hourColor);

	/* for (int i = 0; i < 12; ++i) {
		 painter.drawLine(88, 0, 96, 0);
		 painter.rotate(30.0);
	 }*/

	 
 }
