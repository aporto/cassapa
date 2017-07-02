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

#include "AProfiler.h"

AProfiler profiler;

AProfiler::AProfiler(void)
{
	limit = 50;
	Clear();
}


AProfiler::~AProfiler(void)
{
}


void AProfiler::Clear()
{
	//timerCount = 0;
	memset(timers, 0, sizeof(timers));
	memset(cumTimers, 0, sizeof(cumTimers));
	memset(timersCounters, 0, sizeof(cumTimers));
}

void AProfiler::StartTimer(int timer)
{
	//cumTimers[timer] = timeGetTime();
	/*if (timerCount < timer) {
		timerCount = timer;
	}*/
}

void AProfiler::EndTimer(int timer)
{
	//;;timers[timer] += timeGetTime() - cumTimers[timer];	
	++timersCounters[timer];
	if (timer == 0) {
		if (timersCounters[timer] == limit) {
			GenerateReport();
		}
	}
}

void AProfiler::GenerateReport()
{
	ofstream file;
	file.open ("c:\\temp\\aprofiler.txt");
	for (int i=0; i < MAX_PROFILERS; i++) {
		if (timers[i] > 0) {
			file << i << ":" << float(timers[i]) / 1000.0 << 
				", " << timersCounters[i] << " vezes. Media: " <<
				float(timers[i]) / 50 << " milisecs/chamada." << endl;
		}
	}
	file.close();
}