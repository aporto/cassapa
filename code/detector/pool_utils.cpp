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
#include "pool_utils.h"
//-----------------------------------------------------------------------------

//PoolData poolData;

//-----------------------------------------------------------------------------

int PointsDistance(int x1, int y1, int x2, int y2)
{
	float d1 = float(x1 - x2);
	float d2 = float(y1 - y2);
	int length = (int)sqrt(pow(d1, 2.0f) + pow(d2, 2.0f));
	
	return length;
}

//-----------------------------------------------------------------------------

void GetLineEquation (ScreenLine line, LineEquation &eq)
{
	float dx = float(line.x2 - line.x1);
	float dy = float(line.y2 - line.y1);

	if (line.x1 == line.x2) {
		dx += 0.000001f;
	}
	eq.m = dy/dx;
	eq.h = line.y1 - int(eq.m * float(line.x1));		
}

//-----------------------------------------------------------------------------

void GetLineNormal(ScreenLine line, LineEquation &nEq)
{
	LineEquation eq;
	GetLineEquation (line, eq);
	float angle = atan(eq.m);
	float deltaAngle = 3.1415f - 2.0 * angle;
	angle = angle + (deltaAngle * 1.00);
	nEq.m = tan(angle);
	nEq.h = line.y2 - nEq.m * line.x2;
}
