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

#include "pool.h"

//-----------------------------------------------------------------------------

Pool::Pool(QObject *parent) :
	QObject(parent),
	detector(parent)
{
	
}
//-----------------------------------------------------------------------------

Pool::~Pool()
{
	//
}

bool Pool::Initialize(void)
{
	finish = false;
	/*if (config.input.debugMode) {
		cv::namedWindow("Camera", cv::WINDOW_NORMAL);// | cv::WND_PROP_ASPECT_RATIO);
	}

	if (config.input.showCue) {
		cv::namedWindow("Cue", cv::WINDOW_NORMAL);// | cv::WND_PROP_ASPECT_RATIO);
	}*/

	return detector.Initialize();

	/*string txt = cv::getBuildInformation();
	ofstream file;
	file.open ("c:\\temp\\debug.txt");
	file << txt  << endl;
	file.close();
	*/
	//sinuca 8
	/*ScreenLine sl;
	sl.x1 = 254; sl.y1 = 201;sl.x2 = 502;sl.y2 =  391;
	videoTrajectory.push_back(sl);
	sl.x1 = 502; sl.y1 = 391;sl.x2 = 597;sl.y2 = 339;
	videoTrajectory.push_back(sl);
	sl.x1 = 597; sl.y1 = 339;sl.x2 = 232;sl.y2 =82;
	videoTrajectory.push_back(sl);*/

	//sinuca 9
	/*ScreenLine sl;
	sl.x1 = 222; sl.y1 = 278;sl.x2 = 255;sl.y2 =  391;
	videoTrajectory.push_back(sl);
	sl.x1 = 255; sl.y1 = 391;sl.x2 = 350;sl.y2 = 80;
	videoTrajectory.push_back(sl);
	sl.x1 = 350; sl.y1 = 80;sl.x2 = 395;sl.y2 =388;
	videoTrajectory.push_back(sl);
	*/	

	//sinuca 10
	/*ScreenLine sl;
	sl.x1 = 241; sl.y1 = 224;sl.x2 = 438;sl.y2 =  370;
	videoTrajectory.push_back(sl);
	sl.x1 = 438; sl.y1 = 370;sl.x2 = 593;sl.y2 = 269;
	videoTrajectory.push_back(sl);
	sl.x1 = 593; sl.y1 = 269;sl.x2 = 394;sl.y2 =59;
	videoTrajectory.push_back(sl);
	*/
	//sinuca 11
	/*ScreenLine sl;
	sl.x1 = 210; sl.y1 = 88;sl.x2 = 598;sl.y2 =  148;
	videoTrajectory.push_back(sl);
	sl.x1 = 598; sl.y1 = 148;sl.x2 = 23;sl.y2 = 233;
	videoTrajectory.push_back(sl);
	sl.x1 = 23; sl.y1 = 233;sl.x2 = 593;sl.y2 = 323 ;
	videoTrajectory.push_back(sl);
	*/
}

//-----------------------------------------------------------------------------

void Pool::GoNext(void)
{
	detector.GoNext();
}

//-----------------------------------------------------------------------------

void Pool::GoPrevious(void)
{
	detector.GoPrevious();
}

//-----------------------------------------------------------------------------

bool Pool::Sample(bool draw)
{
	if (finish) {		
		return false;
	}

	profiler.StartTimer(1);	
	bool calculate = true;
	if (detector.Sample() == false) {
		return true; //false;
	}
	profiler.EndTimer(1);	
	if (detector.configOutput.cueFound == false) {
		calculate = false;
	}
	if (detector.balls.size() == 0) {
		calculate = false;
	}

	whiteBall = -1;
	hitBall = -1;
	whiteTrajectory.clear();
	hitTrajectory.clear();

	//detector.frame.copyTo(screen);
	
	if (calculate) {		
		// calcula o vetor que atravessa o taco
		// indicando a direcao de aplicacao da forca
		CalculateCueVector();
		
		// Clear all previous calculations		
		whiteBall = FindClosestBall();	
		if (whiteBall >= 0) {			
			// Calcula o vetor "force" aplicado no centro da bola branca,
			// movendo o ponto x2,y2 do vetor cue para o centro da bola, 
			// e movento o ponto x1,y1 na mesma direcao
			//ScreenLine force = detector.cue;
			forceAtWhiteBall = cueVector;
			
			LineEquation wEq1;
			GetLineEquation (forceAtWhiteBall, wEq1);

			float xb = detector.balls.at(whiteBall).x;
			int yb = int(xb * wEq1.m) + wEq1.h;
			int dx = detector.balls.at(whiteBall).x - forceAtWhiteBall.x2;
			int dy = detector.balls.at(whiteBall).y - forceAtWhiteBall.y2;
			//int dx = 0;
			//int dy = detector.balls.at(whiteBall).y - yb;
			forceAtWhiteBall.x1 = forceAtWhiteBall.x1 + dx;
			forceAtWhiteBall.y1 = forceAtWhiteBall.y1 + dy;
			forceAtWhiteBall.x2 = forceAtWhiteBall.x2 + dx;
			forceAtWhiteBall.y2 = forceAtWhiteBall.y2 + dy;

			LineEquation wEq2;
			GetLineEquation (forceAtWhiteBall, wEq2);
			CalculateBallTrajectory(whiteBall, forceAtWhiteBall, whiteTrajectory);
			for (unsigned int i=0; i < whiteTrajectory.size(); i++) {
				detector.configOutput.whiteTrajectory[i] = whiteTrajectory.at(i);
			}
			detector.configOutput.whiteTrajectoryCount = whiteTrajectory.size();
			float hitX, hitY;
			FindBallHitByThisBall(whiteBall, whiteTrajectory, hitX, hitY);
			if (hitBall >= 0) {				
				ScreenLine hitLine;
				hitLine.x1 = int(hitX);
				hitLine.y1 = int(hitY);
				hitLine.x2 = detector.balls.at(hitBall).x;
				hitLine.y2 = detector.balls.at(hitBall).y;
				//detector.AdjustLineDirectionToPoolCenter(hitLine);
				CalculateBallTrajectory(hitBall, hitLine, hitTrajectory);
				
				// Verifica se a hitBall atinge outra bola no caminho
				// Se sim, elimina essas linhas da trajetoria
				float trashX, trashY;
				FindBallHitByThisBall(hitBall, hitTrajectory, trashX, trashY);								
			}		
			
		}
		detector.configOutput.numbBalls = 0;		
	}

	detector.configOutput.updating = true;
	for (unsigned int i=0; i < detector.balls.size(); i++) {
		detector.configOutput.balls[i] = detector.balls.at(i);
	}
	detector.configOutput.numbBalls = detector.balls.size();
	for (unsigned int i=0; i < whiteTrajectory.size(); i++) {
		detector.configOutput.whiteTrajectory[i] = whiteTrajectory.at(i);
	}
	detector.configOutput.whiteTrajectoryCount = whiteTrajectory.size();
	for (unsigned int i=0; i < hitTrajectory.size(); i++) {
		detector.configOutput.hitTrajectory[i] = hitTrajectory.at(i);
	}
	detector.configOutput.hitTrajectoryCount = hitTrajectory.size();	
	detector.configOutput.whiteBall = whiteBall;
	detector.configOutput.hitBall = hitBall;
	detector.configOutput.updating = false;

	return true;
}

//-----------------------------------------------------------------------------

/*bool Pool::WhiteBallHit(int xs, int ys, float m, int h, int &x,int &y)
{
	return true;
}
*/
//-----------------------------------------------------------------------------

int Pool::FindClosestBall(void)
{
	int sm = 1000000;
	int ret = -1;
	bool invertCue = false;
	for (unsigned int i=0; i < detector.balls.size(); i++) {
		int l1 = PointsDistance (detector.balls.at(i).x, detector.balls.at(i).y, 
								detector.cue.x2, detector.cue.y2);
		int l2= PointsDistance (detector.balls.at(i).x, detector.balls.at(i).y, 
								detector.cue.x1, detector.cue.y1);
		int l = l1;
		invertCue = false;
		if (l2 < l) {
			l = l2;
			invertCue = true;
		}
		if ((l < sm) && (l < config.input.ballSize*15)) { //  distancia minima entre taco e bola para ligar o "aids"
			float hitX, hitY;
			int dist = CheckIfLineCrossesBall(i, config.input.ballSize, cueVector, hitX, hitY, true);
			if (dist >= 0) {
				sm = l;
				ret = i;
				if (invertCue) {
					float aux = detector.cue.x1;
					detector.cue.x1 = detector.cue.x2;
					detector.cue.x2 = aux;
					aux = detector.cue.y1;
					detector.cue.y1 = detector.cue.y2;
					detector.cue.y2 = aux;

					aux = cueVector.x1;
					cueVector.x1 = cueVector.x2;
					cueVector.x2 = aux;
					aux = cueVector.y1;
					cueVector.y1 = cueVector.y2;
					cueVector.y2 = aux;
				}
			}
		}
	}

	/*if (ret >= 0) {
		float hitX, hitY;
		int dist = CheckIfLineCrossesBall(ret, 10, cueVector, hitX, hitY, true);
		if (dist < 0) {
			return -1;
		}
	}*/

	return ret;
}

//-----------------------------------------------------------------------------

/*void Pool::DrawCrossedCircleAroundBall(int ball, cv::Scalar color)
{
	cv::circle(screen, 
		cv::Point(detector.balls.at(ball).x, detector.balls.at(ball).y), 
				20, color, 2, CV_AA);	
	int llen = int(20.0 * 0.7071067);
	cv::Point lp1(detector.balls.at(ball).x - llen , detector.balls.at(ball).y - llen);
	cv::Point lp2(detector.balls.at(ball).x + llen , detector.balls.at(ball).y - llen);
	cv::Point lp3(detector.balls.at(ball).x + llen , detector.balls.at(ball).y + llen);
	cv::Point lp4(detector.balls.at(ball).x - llen , detector.balls.at(ball).y + llen);
	cv::line(screen, lp1, lp3, color, 2, CV_AA);		
	cv::line(screen, lp2, lp4, color, 2, CV_AA);

	//char msg[20];
	//sprintf(msg, "%d", ball);
	//cv::putText(screen, msg, cv::Point(detector.balls.at(ball).x + 20, detector.balls.at(ball).y), 
//					cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(255,255,255));
}*/

//-----------------------------------------------------------------------------

/*void Pool::DrawAids(void)
{
	return;
	// Clear the screen (previous camera images)
	cv::Point lp1 = cv::Point(0,0);
	cv::Point lp2 = cv::Point(screen.cols,screen.rows);
	//cv::rectangle (screen, lp1, lp2, cv::Scalar(0,0,0), CV_FILLED);

	// draw table border
	cv::rectangle(screen, detector.tableLimits, cv::Scalar(170, 170, 170), 1);		

	// draw holes
	cv::Scalar hc = cv::Scalar(255, 255, 255);
	lp1 = cv::Point(detector.screenLimits[0].x,detector.screenLimits[0].y);
	lp2 = cv::Point(lp1.x + config.input.holeWidth, lp1.y);
	cv::line(screen, lp1, lp2, hc, 1);		
	lp2 = cv::Point(lp1.x, lp1.y + config.input.holeWidth);
	cv::line(screen, lp1, lp2, hc, 1);		

	lp1 = cv::Point(detector.screenLimits[1].x,detector.screenLimits[0].y);
	lp2 = cv::Point(lp1.x - config.input.holeWidth, lp1.y);
	cv::line(screen, lp1, lp2, hc, 1);		
	lp2 = cv::Point(lp1.x, lp1.y + config.input.holeWidth);
	cv::line(screen, lp1, lp2, hc, 1);		

	lp1 = cv::Point(detector.screenLimits[1].x,detector.screenLimits[1].y);
	lp2 = cv::Point(lp1.x - config.input.holeWidth, lp1.y);
	cv::line(screen, lp1, lp2, hc, 1);		
	lp2 = cv::Point(lp1.x, lp1.y - config.input.holeWidth);
	cv::line(screen, lp1, lp2, hc, 1);	

	lp1 = cv::Point(detector.screenLimits[0].x,detector.screenLimits[1].y);
	lp2 = cv::Point(lp1.x + config.input.holeWidth, lp1.y);
	cv::line(screen, lp1, lp2, hc, 1);		
	lp2 = cv::Point(lp1.x, lp1.y - config.input.holeWidth);
	cv::line(screen, lp1, lp2, hc, 1);	

	int center = detector.screenLimits[0].x + (detector.screenLimits[1].x - detector.screenLimits[0].x) / 2;
	lp1 = cv::Point(center-config.input.holeWidth,detector.screenLimits[0].y);
	lp2 = cv::Point(center+config.input.holeWidth,detector.screenLimits[0].y);
	cv::line(screen, lp1, lp2, hc, 1);		
	lp1 = cv::Point(center-config.input.holeWidth,detector.screenLimits[1].y);
	lp2 = cv::Point(center+config.input.holeWidth,detector.screenLimits[1].y);
	cv::line(screen, lp1, lp2, hc, 1);	
	
	// draw croessed circle around all balls
	for (unsigned int i=0; i < detector.balls.size(); i++) {
		DrawCrossedCircleAroundBall(i, cv::Scalar(100, 255, 100));
		//cv::Point bp = cv::Point(detector.balls.at(i).x+20, detector.balls.at(i).y);
		//char msg[100];
		//sprintf(msg,"%d",i);
		//putText(screen, msg, bp, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,255,0));
	}
	
	// Draw crossed circle over white ball
	if (whiteBall >= 0) {
		DrawCrossedCircleAroundBall(whiteBall, cv::Scalar(255, 255, 255));		
	}
	// draw white ball trajectory
	for (unsigned int i=0; i < whiteTrajectory.size(); i++) {
		cv::Point lp1, lp2;
		lp1.x = whiteTrajectory.at(i).x1;
		lp1.y = whiteTrajectory.at(i).y1;
		lp2.x = whiteTrajectory.at(i).x2;
		lp2.y = whiteTrajectory.at(i).y2;
		cv::line(screen, lp1, lp2, cv::Scalar(255, 255, 255), 2, CV_AA);		
		cv::circle(screen, lp2, config.input.ballSize, cv::Scalar(255, 255, 255), 1);		
	}

	// draw video trajectory
	for (unsigned int i=0; i < videoTrajectory.size(); i++) {
		cv::Point lp1, lp2;
		lp1.x = videoTrajectory.at(i).x1;
		lp1.y = videoTrajectory.at(i).y1;
		lp2.x = videoTrajectory.at(i).x2;
		lp2.y = videoTrajectory.at(i).y2;
		cv::line(screen, lp1, lp2, cv::Scalar(255, 255, 0), 1, CV_AA);		
	}

	// draw hit ball trajectory
	if (hitBall >= 0) {
		DrawCrossedCircleAroundBall(hitBall, cv::Scalar(200, 100, 255));		
		cv::Point lp1, lp2;
		for (unsigned int i=0; i < hitTrajectory.size(); i++) {			
			
			// draw hitball trajectory
			lp1.x = hitTrajectory.at(i).x1;
			lp1.y = hitTrajectory.at(i).y1;
			lp2.x = hitTrajectory.at(i).x2;
			lp2.y = hitTrajectory.at(i).y2;
			cv::line(screen, lp1, lp2, cv::Scalar(200, 100, 255), 2, CV_AA);		
		}

		// Draw a white circle at the position where whiteball will
		// touch the hitball
		lp1.x = whiteTrajectory.at(whiteTrajectory.size()-1).x2;
		lp1.y = whiteTrajectory.at(whiteTrajectory.size()-1).y2;
		cv::circle(screen, lp1, 10, cv::Scalar(255,255,255), -1, CV_AA);				

		// Draw a purple circle at the position where hitBall ends
		lp1.x = hitTrajectory.at(hitTrajectory.size()-1).x2;
		lp1.y = hitTrajectory.at(hitTrajectory.size()-1).y2;
		cv::circle(screen, lp1, 10, cv::Scalar(200,100,255), -1, CV_AA);	
	}	
	
	if (detector.cueFound) {
		putText(screen, "Cue:Ok", cv::Point(4, 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,255,0));
		cv::line(screen, cv::Point(detector.cue.x1,detector.cue.y1), 
				cv::Point(detector.cue.x2, detector.cue.y2), cv::Scalar(250,200,100), 2, CV_AA);

	} else {
		putText(screen, "Cue:Not found", cv::Point(4, 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0,0,255));
	}
	char msg[20];
	sprintf(msg, "frame:%d", detector.pos);
	putText(screen, msg, cv::Point(4, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(255,0,0));
		
	// draw cue vector trajectory
	lp1.x = cueVector.x1;
	lp1.y = cueVector.y1;
	lp2.x = cueVector.x2;
	lp2.y = cueVector.y2;	
	cv::line(screen, lp1, lp2, cv::Scalar(150, 100, 255), 1, CV_AA);	

	// draw projector box
	lp1.x = config.input.projector.box.x1;
	lp1.y = config.input.projector.box.y1;
	lp2.x = config.input.projector.box.x2;
	lp2.y = config.input.projector.box.y2;	
	cv::rectangle (screen, lp1, lp2,cv::Scalar(100,100,100), 1);			

	lp1.x = config.input.projector.box.x1;
	lp1.y = config.input.projector.box.y1;
	lp2.x = config.input.projector.box.x2;
	lp2.y = config.input.projector.box.y1;	
	cv::line(screen, lp1, lp2, cv::Scalar(200,200,200), 1, CV_AA);	

	cv::Point lp3(config.input.projector.box.x1, config.input.projector.box.y2); 
	cv::Point lp4(config.input.projector.box.x2, config.input.projector.box.y2); 
	float w = float(config.input.projector.box.x2 - config.input.projector.box.x1) / config.input.projector.topBottomRatio / 2;
	lp3.x = config.input.projector.box.x1 + (config.input.projector.box.x2 - config.input.projector.box.x1) / 2 - w;
	lp4.x = config.input.projector.box.x1 + (config.input.projector.box.x2 - config.input.projector.box.x1) / 2 + w;
	cv::line(screen, lp1, lp3, cv::Scalar(200,200,200), 1, CV_AA);	
	cv::line(screen, lp2, lp4, cv::Scalar(200,200,200), 1, CV_AA);	
	cv::line(screen, lp4, lp4, cv::Scalar(200,200,200), 1, CV_AA);		
}
*/
//-----------------------------------------------------------------------------

void Pool::CalculateBallTrajectory(int ball, ScreenLine line, vector <ScreenLine> &trajectory)
{
	float yc, xc;

	trajectory.clear();
	int hitSide = -1;	

	ScreenLine newLine;
	newLine.x1 = detector.balls.at(ball).x;
	newLine.y1 = detector.balls.at(ball).y;

	LineEquation lineEq2;
	ScreenLine l2;
	l2.x1 = newLine.x1;
	l2.y1 = newLine.y1;

	while (true) {
		LineEquation lineEq;
		GetLineEquation (line, lineEq);

		if (abs(line.x1 - line.x2) > 0) {		
			// Encontra o ponto de contado da direcao do taco com a borda da mesa
			
			if (line.x1 < line.x2) { // vai pra direita
				xc = float(detector.screenLimits[1].x);
				hitSide = 1; // direita
			} else {
				xc = float(detector.screenLimits[0].x);
				hitSide = 3; // esqueda
			}
			yc = lineEq.m * xc + lineEq.h;
			l2.x2 = xc; l2.y2 = yc; GetLineEquation (l2, lineEq2);

			if (yc < detector.screenLimits[0].y) { // calculou-se a borda lateral, mas acertou a borda superior antes
				yc = float(detector.screenLimits[0].y);
				xc = float(yc - lineEq.h)/lineEq.m;
				hitSide = 0; 
			} else if (yc > detector.screenLimits[1].y) { // ... acertou a borda inferior antes
				yc = float(detector.screenLimits[1].y);
				xc = float(yc - lineEq.h)/lineEq.m;
				hitSide = 2;
			}
		} else {
			// If direction-line is fully vertical, we have to avoid calculating the line equation
			// because we'll get gradient == 0
			xc = float(line.x1);
			if (line.y1 < line.y2) {//detector.screenLimits[0].y + 20) {			
				yc = float(detector.screenLimits[1].y);	
				hitSide = 2;
			} else {
				yc = float(detector.screenLimits[0].y);	
				hitSide = 0;
			}		
		}
		l2.x2 = xc; l2.y2 = yc; GetLineEquation (l2, lineEq2);

		
		newLine.x2 = int(xc);
		newLine.y2 = int(yc);	


		// Pega o proximo ponto de ricochete para calcular a proxima linha de trajetoria		
		line.x1 = newLine.x2;
		line.y1 = newLine.y2;		

		// bx e by sao usados para calcular o ponto de contato da bola com a borda da mesa
		// ja que a bola tem raio > 1, ela nao toca na mesa exatamente em (x2,y2)
		float by = float(config.input.ballSize);///2.0f;
		float bx = float(config.input.ballSize);///2.0f;
		float hipotenusa = (float) PointsDistance (newLine.x1, newLine.y1, newLine.x2, newLine.y2);		
		float cateto = 0.0;

		int dx = newLine.x2 - newLine.x1;
		int dy = newLine.y2 - newLine.y1;

		if ( (hitSide == 0) || (hitSide == 2) ) {// ricocheteou em cima						
			if (hitSide == 0) {
				cateto = float(newLine.y1 - detector.screenLimits[0].y);
			} else {
				cateto = float(detector.screenLimits[1].y - newLine.y1);
			}
			float angle = asin(cateto / hipotenusa);
			bx = by / tan(angle);//hipotenusa * cos(angle);
			line.x2 = line.x1 + dx;
			line.y2 = newLine.y1;
		} else {			
			if (hitSide == 3) {
				cateto = float(newLine.x1 - detector.screenLimits[0].x);
			} else {
				cateto = float(detector.screenLimits[1].x - newLine.x1);
			}
			float angle = asin(cateto / hipotenusa);
			by = hipotenusa * cos(angle);
			by = bx / tan(angle);//hipotenusa * cos(angle);
			line.y2 = line.y1 + dy;
			line.x2 = newLine.x1;
		}

		// Forca o arredondamento, ao inves de truncamento, 
		// quando somar bx e by aos valores finais
		bx += 0.5;
		by += 0.5;

		if (dx > 0) {
			bx = -bx;
		}
		if (dy > 0) {
			by = -by;
		}

		newLine.x2 += int(bx);
		newLine.y2 += int(by);

		trajectory.push_back(newLine);	 

		

		// Cada vetor <trajectory> pode conter ate 3 linhas (2 ricochetes)
		// Nao imagino que a precisao desse sistema permita calculos com mais de 2 ricochetes
		if (trajectory.size() > 2) {
			break;
		}

		// Se o final dessa linha de trajetoria for uma cacapa, entao encerra o calculo das linhas
		// de trajetoria	
		if ( (hitSide == 0) || (hitSide == 2) ) {
			if (newLine.x2 < detector.screenLimits[0].x + config.input.holeWidth) { 
				break;
			}
			if (newLine.x2 > detector.screenLimits[1].x - config.input.holeWidth) { 
				break;
			}
			int center = detector.screenLimits[0].x + (detector.screenLimits[1].x - detector.screenLimits[0].x) / 2;
			if ( (newLine.x2 > center - config.input.holeWidth) && (newLine.x2 < center + config.input.holeWidth) ) { 
				break;
			}
		} else {			
			if (newLine.y2 < detector.screenLimits[0].y + config.input.holeWidth) { 
				break;
			}
			if (newLine.y2 > detector.screenLimits[1].y - config.input.holeWidth) { 
				break;
			}
		}

		// Agora calculamos a linha da proxima trajetora
		// Ela sera a normal da linha atual no ponto de reflexao atual

		//LineEquation newLineEq;
		//newLineEq.m = lineEq.m
		//GetLineEquation (line, lineEq);

		/*int ndx = newLine.x2 - newLine.x1;
		int ndy = newLine.y2 - newLine.y1;
		line.x1 = -ndy;
		line.y1 = ndx;
		line.x2 = ndy;
		line.y2 = -ndx;		*/

		LineEquation newLineEq;
		GetLineEquation (newLine, newLineEq);
		float angle = atan(newLineEq.m);
		float deltaAngle = 3.1415f - 2.0 * angle;
		angle = angle + (deltaAngle * 1.00);
		newLineEq.m = tan(angle);

		newLineEq.h = newLine.y2 - newLineEq.m * newLine.x2;

		line.x1 = newLine.x2;
		line.y1 = newLine.y2;		

		if (hitSide == 0) {			
			line.y2 = detector.screenLimits[1].y;
			line.x2 = int(float(line.y2 - newLineEq.h)/float(newLineEq.m));
		} else if (hitSide == 1) {
			line.x2 = detector.screenLimits[0].y;
			line.y2 = newLineEq.m * line.x2 + newLineEq.h;
		} else if (hitSide == 2) {
			line.y2 = detector.screenLimits[0].y;
			line.x2 = int(float(line.y2 - newLineEq.h)/float(newLineEq.m));
		} else if (hitSide == 3) {
			line.x2 = detector.screenLimits[1].x;
			line.y2 = newLineEq.m * line.x2 + newLineEq.h;
		}

		newLine.x1 = line.x1;
		newLine.y1 = line.y1;
	}	
}

//-----------------------------------------------------------------------------

void Pool::CalculateCueVector(void)
{
	LineEquation lineEq;
	GetLineEquation (detector.cue, lineEq);

	if (abs(detector.cue.x1 - detector.cue.x2) < 1) {
		// If cue is fully vertical, we have to avoid calculating the line equation
		// because we'll get gradient == 0
		cueVector.x1 = detector.cue.x1;
		cueVector.x2 = detector.cue.x2;
		cueVector.y2 = detector.cue.y1;	
		if (detector.cue.y1 < detector.screenLimits[0].y + 20) {			
			cueVector.y2 = detector.screenLimits[1].y;	
		} else {
			cueVector.y2 = detector.screenLimits[0].y;	
		}
		return;
	}
	
	// Encontra o ponto de contado da direcao do taco com a borda da mesa
	float yc, xc;
	if (detector.cue.x1 < detector.cue.x2) { // vai pra direita
		xc = float(detector.screenLimits[1].x);
	} else {
		xc = float(detector.screenLimits[0].x);
	}
	yc = lineEq.m * xc + lineEq.h;

	if (yc < detector.screenLimits[0].y) { // calculou-se a borda lateral, mas acertou a borda superior antes
		yc = float(detector.screenLimits[0].y);
		xc = (yc - lineEq.h)/lineEq.m;
	} else if (yc > detector.screenLimits[1].y) { // ... acertou a borda inferior antes
		yc = float(detector.screenLimits[1].y);
		xc = (yc - lineEq.h)/lineEq.m;
	}

	cueVector.x1 = detector.cue.x1;
	cueVector.y1 = detector.cue.y1;
	cueVector.x2 = int(xc);
	cueVector.y2 = int(yc);	
}

//-----------------------------------------------------------------------------

void Pool::FindBallHitByThisBall(int ball, vector <ScreenLine> &trajectory, float &hitX, float &hitY)
{
	if (ball == whiteBall) {
		hitBall = -1;
	}
	int hit = -1;
	int smallestDistance = 100000;
	for (unsigned int l=0; l < trajectory.size(); l++) {
		ScreenLine line = trajectory.at(l);
		LineEquation eq;
		GetLineEquation (line, eq);
		for (unsigned int b=0; b < detector.balls.size(); b++) {
			// Ignora se atingir a bola branca, pois ela nao estara mais no seu ponto original
			// Logo, esse hit nao poderia ser calculado usando a posicao original
			// da bola branca
			if ((b == ball) || (b == whiteBall)) {
				continue;				
			}
			int xx = detector.balls.at(b).x;
			int yy = detector.balls.at(b).y;

			float tmpHitX, tmpHitY;
			int dist = CheckIfLineCrossesBall(b, config.input.ballSize * 2, line, tmpHitX, tmpHitY, false);
			if ( (dist >= 0) && (dist < smallestDistance)) {
				hitX = tmpHitX;
				hitY = tmpHitY;
				smallestDistance = dist;				
				hit = b;
			}
		}
		if (hit >= 0) {
			trajectory.at(l).x2 = int(hitX);
			trajectory.at(l).y2 = int(hitY);

			// Apaga as demais linhas de trajetoria da bola
			// pois ela ja atingiu outra bola no caminho
			unsigned int delCount = trajectory.size() - l - 1;
			for (unsigned int i=0; i < delCount; i++) {
				trajectory.pop_back();
			}
			break;
		}
	}	
	if (ball == whiteBall) {
		hitBall = hit;
	}
}

//-----------------------------------------------------------------------------

int Pool::CheckIfLineCrossesBall(int ball, int ray, ScreenLine line, float &hitX, float &hitY, bool fullLine)
{		
	int distance = -1;
	
	// fullLine: true= linha infinita, false = segmento de reta limitado em x1,y1,x2,y2
	// A bola so pode cruzar o segmento de linha se estiver dentro do retangulo (x1, y1, x2, y2), pelo menos
	// O codigo abaixo teste isso, ainda considerando o raio da bola
	// ou seja, verifica as condicoes minimas para a bola poder cruzar o segmento de reta	
	// como x1 pode ser maior que x2 aqui (o mesmo vale para y1 e y2)
	// entao encontramos o rectangulo formado pela reta, e ele sera armazenado
	// em xa,ya,xb,yb, sendo que aqui xa e ya sao sempre menores que xb e yb, respectivamente
	if (fullLine == false) {
		int xa,xb,ya,yb;
		if (line.x1<line.x2) {xa = line.x1; xb = line.x2;} else {xa = line.x2; xb = line.x1;}
		if (line.y1<line.y2) {ya = line.y1; yb = line.y2;} else {ya = line.y2; yb = line.y1;}
		if ((detector.balls.at(ball).x + ray < xa) || (detector.balls.at(ball).x - ray > xb)) {
			return -2;
		}
		if ((detector.balls.at(ball).y + ray < ya) || (detector.balls.at(ball).y - ray > yb)) {
			return -3;
		}
	}
	

	if (abs(line.x1 - line.x2) < 3) {
		// If lineis fully vertical, we have to avoid calculating the line equation
		// because we'll get gradient == 0
		hitX = float(line.x1);
		float dx = abs(float(line.x1 - detector.balls.at(ball).x));
		if (dx > ray) {
			// didn't touched
			return -1;
		}
		float ang = acos(dx/float(ray));
		float dy = fabs(sin(ang)) * float(ray);	
		if (line.y1 < detector.balls.at(ball).y) {
			hitY = detector.balls.at(ball).y - dy;
		} else {
			hitY = detector.balls.at(ball).y + dy;
		}

		return int(abs(line.y1 - hitY));
	}

	LineEquation wEq;
	GetLineEquation (line, wEq);

	float p = float(detector.balls.at(ball).x);

	float q = float(detector.balls.at(ball).y);
	float r = float(ray); //10.0; // raio de cada bola
			
	float A = pow(wEq.m, 2.0f) + 1.0f;
	float B = 2.0f * (wEq.m*float(wEq.h) - wEq.m*q - p);
	float C = pow(q,2.0f) - pow(r,2.0f) + pow(p,2.0f) - 2.0f*float(wEq.h)*q + pow(float(wEq.h),2.0f);
	float D = pow(B,2.0f) - 4.0f*A*C;

	if (D > -0.00001) { // Line only intercepts circle if D>0
		if (D < 0.0001){
			D = 0.0;
		}
		float xi1 = (-B - sqrt(D)) / (2*A);
		float xi2 = (-B + sqrt(D)) / (2*A);
		float yi1 = wEq.m * xi1 + wEq.h;
		float yi2 = wEq.m * xi2 + wEq.h;
		if ((xi1 < detector.screenLimits[0].x) || (xi1 > detector.screenLimits[1].x)) {
			return -1;
		}
		if ((xi2 < detector.screenLimits[0].x) || (xi2 > detector.screenLimits[1].x)) {
			return -1;
		}
		if ((yi1 < detector.screenLimits[0].y) || (yi1 > detector.screenLimits[1].y)) {
			return -1;
		}
		if ((yi2 < detector.screenLimits[0].y) || (yi2 > detector.screenLimits[1].y)) {
			return -1;
		}
		int l1 = PointsDistance(line.x1, line.y1, int(xi1), int(yi1));
		int l2 = PointsDistance(line.x1, line.y1, int(xi2), int(yi2));
		if (l1 < l2) {
			distance = l1;
			hitX = xi1; hitY = yi1;
		} else {
			distance = l2;
			hitX = xi2; hitY = yi2;
		}
	}

	return distance;
}

cv::Mat * Pool::GetImgTable(void)
{
	return &(detector.imgTableRemoved);
}

cv::Mat * Pool::GetImgCue(void)
{
	return &(detector.imgCue);
}

int Pool::GetDistanceFromHitBallToNearestHole(void)
{
	if (hitTrajectory.size() == 0) {
		return -1;
	}

	int distance = 1000000;

	for (int i=0; i < hitTrajectory.size(); i++) {
		int ballX, ballY;
		ballX = hitTrajectory.at(i).x2;
		ballY = hitTrajectory.at(i).y2;
		ScreenPoint hole;

		int dist = PointsDistance (ballX, ballY, detector.screenLimits[0].x,detector.screenLimits[0].y);
		if (dist < distance) { distance = dist;}
		dist = PointsDistance (ballX, ballY, detector.screenLimits[1].x,detector.screenLimits[0].y);
		if (dist < distance) { distance = dist;}
		dist = PointsDistance (ballX, ballY, detector.screenLimits[1].x,detector.screenLimits[1].y);
		if (dist < distance) { distance = dist;}
		dist = PointsDistance (ballX, ballY, detector.screenLimits[0].x,detector.screenLimits[1].y);
		if (dist < distance) { distance = dist;}
		
		int xc = detector.screenLimits[0].x + (detector.screenLimits[1].x - detector.screenLimits[0].x) / 2;
		dist = PointsDistance (ballX, ballY, xc,detector.screenLimits[0].y);
		if (dist < distance) { distance = dist;}
		dist = PointsDistance (ballX, ballY, xc,detector.screenLimits[1].y);
		if (dist < distance) { distance = dist;}
	}

	// Se a bola acerta a cacapa, considera que a distancia eh 0
	if (distance < config.input.holeWidth) {
		distance = 0;
	}

	return distance;
}

//-----------------------------------------------------------------------------

/*void Pool::FetchInput(void)
{
	memcpy(&(detector.configInput), &(config.input), sizeof(PoolInput));
	memcpy(&(detector.configOutput), &(config.output), sizeof(PoolOutput));
}

//-----------------------------------------------------------------------------

void FetchOutput(void);
*/