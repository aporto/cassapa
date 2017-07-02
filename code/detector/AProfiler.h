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

#pragma once

#include <Windows.h>
#include <MMSystem.h>
#include <fstream>

using namespace std;


#define MAX_PROFILERS	200


class AProfiler
{
public:
	AProfiler(void);
	~AProfiler(void);
	void Clear();
	void StartTimer(int timer);
	void EndTimer(int timer);
	void GenerateReport();
private:
	unsigned int timers[MAX_PROFILERS];
	unsigned int cumTimers[MAX_PROFILERS];
	unsigned int timersCounters[MAX_PROFILERS];
	unsigned int limit;
	//int timerCount;
};

extern AProfiler profiler;

