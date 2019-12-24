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

#include "detector.h"

//-----------------------------------------------------------------------------

//Detector detector;

Detector::Detector(QObject *parent) :
	QObject(parent),
	imageTransmitter(parent)
{
	config.output.imageRequestCount = 0;
	config.output.imageTransmitCount = 0;
}

bool Detector::Initialize(void)
{

//	pos = 67;//162;//99;
	pos = configInput.initialVideoPos;
	if (pos < 0) {
		pos = 0;
	}

	lastCueAngle = 720;
	currentCueAngle = 0;
	//prevAngleDeviation = 0;

	cueHistoryCount = 0;

	screenLimits[0].x = configInput.limitX1;//25;
	screenLimits[0].y = configInput.limitY1;//83;
	screenLimits[1].x = configInput.limitX2;//605;
	screenLimits[1].y = configInput.limitY2;//410;

	tableLimits = cv::Rect (cv::Point(screenLimits[0].x, screenLimits[0].y), 
									cv::Point(screenLimits[1].x, screenLimits[1].y));

	imageTransmitter.Initialize(&(originalImage));

	error = false;
	foundWhiteBall = false;
	configOutput.cueFound = false;

	QString videoFile = QDir(config.videoFilePath).filePath(config.input.videoFileName);
	
	if (config.loadVideoFile == false) {
		videoFile = "";
	}

	if (videoFile == "") {
		fromFile = false;
		cap.open(configInput.cameraId);
	} else {
		fromFile = true;
		cap.open(videoFile.toStdString());
	}
	
	if ( !cap.isOpened() )  // if not success, exit program
	{
		 cout << "Cannot open the video file" << endl;
		 error = true;
		 configOutput.errorCode = CASSAPA_ERROR_COULD_NOT_GET_CAMERA_IMAGE;
		 return false;
	}

	
	if (configInput.useKeyboard) {
		cap.set(CV_CAP_PROP_POS_FRAMES, pos);		
	} 
	
	if (fromFile) {
		if (cap.read(frame) == false) {
			error = true;
			configOutput.errorCode = CASSAPA_ERROR_COULD_NOT_GET_CAMERA_IMAGE;
			cout << "Cannot read from the video file" << endl;
			return false;
		}
	} else {
		if (cap.read(frame) == false) {
			configOutput.errorCode = CASSAPA_ERROR_COULD_NOT_GET_CAMERA_IMAGE;
			return false;
		}		
	}
	
	cv::resize(frame, frame, cv::Size(640, 480));
	
	/*if (config.output.licenceStatus < 0) {
		frame = cv::Scalar(0,0,255);
	}*/
	
	imgHSV = cvCreateImage(cvSize(frame.size().width, frame.size().height),IPL_DEPTH_8U, 3);
	imgThresh = cvCreateImage(cvSize(imgHSV.size().width, imgHSV.size().height),IPL_DEPTH_8U, 1);

	imgCue = cvCreateImage(cvSize(imgHSV.size().width, imgHSV.size().height),IPL_DEPTH_8U, 1);

	//cv::namedWindow("Taco2", cv::WINDOW_NORMAL);// | cv::WND_PROP_ASPECT_RATIO);

	int erosionSize = 3;
	erosionBall = cv::getStructuringElement(cv::MORPH_RECT, 
											cv::Size(2*erosionSize + 1, 2*erosionSize+1),
											cv::Point( erosionSize, erosionSize ) );
	erosionSize = 1;
	erosionTip = cv::getStructuringElement(cv::MORPH_RECT, 
										   cv::Size(2*erosionSize + 1, 2*erosionSize+1),
										   cv::Point( erosionSize, erosionSize ) );
	erosionSize = 2;
	erosionCue = cv::getStructuringElement(cv::MORPH_RECT, 
										   cv::Size(2*erosionSize + 1, 2*erosionSize+1),
										   cv::Point( erosionSize, erosionSize ) );
	
	return true;
}

//-----------------------------------------------------------------------------


// Forca uma linha a ir em direcao ao centro, e nao saindo do centro
// x1,y1 devem sempre ser o ponto mais longe do centro
void Detector::AdjustLineDirectionToPoolCenter(ScreenLine &line)
{
	int xc = frame.size().width / 2;
	int yc = frame.size().height / 2;
	int l1 = PointsDistance(line.x1, line.y1, xc, yc);
	int l2 = PointsDistance(line.x2, line.y2, xc, yc);

	if (l1 < l2) {
		int aux = line.x1;
		line.x1 = line.x2;
	line.x2 = aux;
		aux = line.y1;
		line.y1 = line.y2;
		line.y2 = aux;
	}
}

//-----------------------------------------------------------------------------

void Detector::GoNext(void)
{
	if (configInput.useKeyboard == false) {
		return;
	}
	pos++;
	cap.set(CV_CAP_PROP_POS_FRAMES, pos);
	if (cap.read(frame) == false) {
		pos--;
		cap.set(CV_CAP_PROP_POS_FRAMES, pos);		
		cap.read(frame);
	}
}

//-----------------------------------------------------------------------------

void Detector::GoPrevious(void)
{
	if (configInput.useKeyboard == false) {
		return;
	}
	pos--;
	if (pos < 0) {
		pos = 0;
	}
	cap.set(CV_CAP_PROP_POS_FRAMES, pos);
	cap.read(frame);
}

//-----------------------------------------------------------------------------

bool Detector::Sample(void)
{
	if (error) {
		return false;
	}

	screenLimits[0].x = configInput.limitX1;//25;
	screenLimits[0].y = configInput.limitY1;//83;
	screenLimits[1].x = configInput.limitX2;//605;
	screenLimits[1].y = configInput.limitY2;//410;

	tableLimits = cv::Rect (cv::Point(screenLimits[0].x, screenLimits[0].y), 
							cv::Point(screenLimits[1].x, screenLimits[1].y));

	//configInput.useKeyboard = true;
	//pos = 2;
	//GoNext();

	
	

	if (configInput.useKeyboard == false) {
		if (fromFile) {
			pos++;
			if (configInput.initialVideoPos >= 0) {
				if (pos > configInput.finalVideoPos) {
					pos = configInput.initialVideoPos;					
				}
				cap.set(CV_CAP_PROP_POS_FRAMES, pos);
			}
			

			//////
			//pos = 87;
			//cap.set(CV_CAP_PROP_POS_FRAMES, pos);
			////

			if (cap.read(frame) == false) {
				cap.set(CV_CAP_PROP_POS_FRAMES, 0);
				if (cap.read(frame) == false) {
					configOutput.errorCode = CASSAPA_ERROR_COULD_NOT_GET_CAMERA_IMAGE;
					return false;
				}
			}
		} else {
			if (cap.read(frame) == false) {
				configOutput.errorCode = CASSAPA_ERROR_COULD_NOT_GET_CAMERA_IMAGE;
				return false;
			}
		}
		pos = cap.get(CV_CAP_PROP_POS_FRAMES)-1;
	}

	if (frame.empty() == false) {	

		/*if (config.output.licenceStatus < 0) {
			frame = cv::Scalar(0,0,255);		
		}*/
		//cv::Mat tmp;
		//frame.copyTo(originalImage);
		imageTransmitter.mutex.lock();
		cv::resize(frame, frame, cv::Size(640, 480));
		cv::cvtColor(frame, originalImage, CV_BGR2RGB); 
		//frame.copyTo(originalImage);
		imageTransmitter.mutex.unlock();
		//preview = QImage((uchar*) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
		//preview = QGLWidget::convertToGLFormat(preview);

		cv::cvtColor(frame(tableLimits), imgHSV, CV_BGR2HSV); 
		cv::inRange(imgHSV, cv::Scalar(configInput.tableColors.lh,configInput.tableColors.ls,configInput.tableColors.lv), 
					cv::Scalar(configInput.tableColors.uh,configInput.tableColors.us,configInput.tableColors.uv), imgTableRemoved);

		if ((configOutput.cueFound == false) || (configInput.cue.imgId == 0)) {
			cueROI = cv::Rect(0, 0, imgHSV.cols-1, imgHSV.rows-1);//cv::Rect(314, 48, 55, 124);					 
		}

		cv::bitwise_not(imgTableRemoved, imgTableRemoved);	
		//if (configInput.configuring == false) {
			cv::erode(imgTableRemoved, imgTableRemoved, erosionTip);
			cv::dilate(imgTableRemoved, imgTableRemoved, erosionTip);		
		//}

		profiler.StartTimer(3);	
		FindCue();
		profiler.EndTimer(3);	

		profiler.StartTimer(4);			
		FindAllBalls();
		profiler.EndTimer(4);			
	}
	
	return true;
}

//-----------------------------------------------------------------------------

void Detector::FindAllBalls(void)
{
	if (configOutput.cueFound == false) {
		return;
	} 

	if (abs(currentCueAngle - lastCueAngle) < 0.1) {
		//return; //// comentado para debug
	}
	
	lastCueAngle = currentCueAngle;
	cv::Mat temp;
	imgTableRemoved.copyTo(temp);

	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	cv::threshold(temp, temp, 1, 255, cv::THRESH_BINARY);
	findContours(temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	cv::threshold(temp, temp, 1, 255, cv::THRESH_BINARY);	

	vector <ScreenPoint> ballsBak;
	ballsBak = balls;
	balls.clear();

	/*ScreenPoint sp;
	sp.x = 54;
	sp.y = 90;
	balls.push_back(sp);
	sp.x = 54; //410;//310;
	sp.y = 375; //250;//355;
	balls.push_back(sp);
	sp.x = 374;//310;
	sp.y = 174;//355;
	balls.push_back(sp);
	sp.x = 374;//310;
	sp.y = 304;//355;
	balls.push_back(sp);
	sp.x = 262;//310;
	sp.y = 210;//355;
	balls.push_back(sp);
	sp.x = 306;//310;
	sp.y = 393;//355;
	balls.push_back(sp);
	return;*/

	int count=0;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		if (numObjects < 100) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				cv::Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				ScreenPoint sp;
				sp.x = int( moment.m10 / area);
				sp.y = int (moment.m01 / area);
				
				//char msg[100];
				cv::Point bp = cv::Point(sp.x, sp.y);
				
				if ( (area > 130) && (area < 400) ) {					
					//sprintf(msg,"%d:%d", count, int(area));				
					//putText(temp, msg, bp, cv::FONT_HERSHEY_COMPLEX_SMALL, 0.7, cv::Scalar(255,255,255));
				
					sp.x += screenLimits[0].x;
					sp.y += screenLimits[0].y;

					count++;
				
					balls.push_back(sp);
				}
			}
		}
	}

	// Filtra pequenas variacoes na leitura das posicoes das bolas
	for (unsigned int i=0; i < balls.size(); i++) {
		for (unsigned int j=0; j < ballsBak.size(); j++) {
			if ( (abs(balls.at(i).x-ballsBak.at(j).x) < 3) && (abs(balls.at(i).y-ballsBak.at(j).y) < 3) ) {
				balls.at(i).x = ballsBak.at(j).x;
				balls.at(i).y = ballsBak.at(j).y;
				break;
			}
		}
	}

	// se licenca venceu, inverte x e y das bolas pra impedir a brincadeira
	/*if (config.output.licenceStatus < 0) {
		//frame = cv::Scalar(0,0,255);
		for (unsigned int i=0; i < balls.size(); i++) {
			int aux = balls.at(i).x;
			balls.at(i).x = ballsBak.at(i).y;
			balls.at(i).y = aux;
		}
	}*/

	//cv::imshow("temp", temp);
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

void Detector::FindCue(void)
{
	// Esta funcao encontra uma linha (cue) cujos 2 pontos indicam a posicao do
	// taco dentro da mesa visivel (Dentro dos limites da mesa)

	// Iniciaalmente ela usa a imagem imgTaco, que contem a imagem da mesa ja com todas
	// as cores diferentes do taco eliminadas. Assim, imgTaco contem apenas um bitmap com fundo 
	// preto e areas brancas equivalentes a uma mascara do taco. Porem, pode haver ruidos,
	// principalmente devido a cor da mao, que eh muito parecida com a cor do taco
	
	// Ela entao faz duas buscas:
	// 1) A primeira usa HughLinesP para fazer uma busca probabilistica de todas as linhas
	// encontradas e identificar um retangulo onde o taco deve estar
	// 2) A segunda usa HughLines standard para identificar, com mais precisao, as linhas
	// Porem, a funcao standard nao retorna uma linha, mas apenas a equacao. Por isso
	// precisamos da hughlinesP, para fazer o cliping da linha encontrada nessa 2a. chamada
	
	//cv::Rect imgRect = cv::Rect(0, 0, imgHSV.cols-1, imgHSV.rows-1);

	cv::Mat imgTaco, imgTacoBak, imgTaco2;	
	configOutput.cueFound = false;

	if (configInput.cue.imgId == 0) {		
		cv::inRange(imgHSV, cv::Scalar(configInput.cueColors.lh,configInput.cueColors.ls,configInput.cueColors.lv), 
		cv::Scalar(configInput.cueColors.uh,configInput.cueColors.us,configInput.cueColors.uv), imgCue);
		cv::cvtColor(imgCue, imgCue, CV_GRAY2RGB);
		return;
	}

	cv::inRange(imgHSV/*(cueROI)*/, cv::Scalar(configInput.cueColors.lh,configInput.cueColors.ls,configInput.cueColors.lv), 
			cv::Scalar(configInput.cueColors.uh,configInput.cueColors.us,configInput.cueColors.uv), imgTaco);

	if (configInput.cue.imgId >= 0) {		
		cv::cvtColor(imgHSV, imgCue, CV_HSV2RGB);
		if (configInput.cue.imgId == 0) {		
			cv::cvtColor(imgTaco, imgTaco, CV_GRAY2RGB);
			imgTaco.copyTo( imgCue( cueROI ) );
			cv::cvtColor(imgTaco, imgTaco, CV_RGB2GRAY);
			cv::rectangle(imgCue, cueROI, cv::Scalar(100, 100, 100), 1);
		}
	}

	if (configInput.cue.erodeSize > 0) {
		int erosionSize = configInput.cue.erodeSize;
		cv::Mat erosionCue = cv::getStructuringElement(cv::MORPH_RECT, 
										   cv::Size(2*erosionSize + 1, 2*erosionSize+1),
										   cv::Point( erosionSize, erosionSize ) );
		cv::erode(imgTaco, imgTaco, erosionCue);
	}

	if (configInput.cue.dilateSize > 0) {
		int erosionSize = configInput.cue.dilateSize;
		cv::Mat erosionCue = cv::getStructuringElement(cv::MORPH_RECT, 
										   cv::Size(2*erosionSize + 1, 2*erosionSize+1),
										   cv::Point( erosionSize, erosionSize ) );
		cv::dilate(imgTaco, imgTaco, erosionCue);		
	}

	if (configInput.cue.imgId == 1) {		
		cv::cvtColor(imgTaco, imgTaco, CV_GRAY2RGB);
		imgTaco.copyTo( imgCue);//( cueROI ) );
		cv::cvtColor(imgTaco, imgTaco, CV_RGB2GRAY);
		cv::rectangle(imgCue, cueROI, cv::Scalar(100, 100, 100), 1);
	}
	
	vector<vector<cv::Point>> contours;	
//	cv::Mat imgTacoBak;
	imgTaco.copyTo(imgTacoBak);	
	imgTaco.copyTo(imgTaco2);	
	
	// detecta todas as linhas dentro do taco
	vector<cv::Vec4i> lines4i;	
	
	//cv::Mat imgTaco2;	

	//HoughLinesP(imgTaco, lines4i, 1, CV_PI/180, 50, 50, 3);	
	/*if (configInput.debugMode) {
		cv::imshow("debug 1", imgTaco);
	}*/
	
	cv::HoughLinesP(imgTaco, lines4i, configInput.cue.hlp_rho, configInput.cue.hlp_theta, 
		configInput.cue.hlp_threshold, configInput.cue.hlp_minLength, configInput.cue.hlp_maxGap);	

	if (lines4i.size() > 200) {
		// Alguma coisa deu errada: Achou muitas linhas
		// Provavelmente tableLimits ainda nao esta corretamente definido
		// e ele esta encontrando as linhas da borda da mesa
		return;
	}

/*	if (configInput.debugMode) {
		cv::imshow("debug 2", imgTaco);
	}*/
	
	int sz = lines4i.size();
	configOutput.cueFound = sz > 0;
	imgTacoBak.copyTo(imgTaco);		
	
	if (configOutput.cueFound == false) {
		if (configInput.cue.imgId == 2) {		
			cv::cvtColor(imgTaco, imgTaco, CV_GRAY2RGB);
			imgTaco.copyTo( imgCue);//( cueROI ) );
			cv::cvtColor(imgTaco, imgTaco, CV_RGB2GRAY);
			cv::rectangle(imgCue, cueROI, cv::Scalar(100, 100, 100), 1);
		}
		return; 
	}

	// encontramos os valores min e max de x e y em todas as linhas encontradas
	// para depois usar estes limites para encontrar os limites da linha media calculada
	float minx = 10000;
	float maxx = 0;
	float miny = 10000;
	float maxy = 0;
	float maxLength = 0;
	vector<cv::Vec2f> lines2fa;
	for( size_t i = 0; i < lines4i.size(); i++ )  {		
		cv::Vec4i l = lines4i[i];
		float length = (float)PointsDistance(l[0], l[1], l[2], l[3]);		
		if (maxLength < length) {
			maxLength = length;
		}
		int x1 = l[0];// + cueROI.x;
		int x2 = l[2];// + cueROI.x;
		int y1 = l[1];// + cueROI.y;
		int y2 = l[3];// + cueROI.y;
		if (x1 > maxx) { maxx = (float)x1;}
		if (x2 > maxx) { maxx = (float)x2;}
		if (y1 > maxy) { maxy = (float)y1;}
		if (y2 > maxy) { maxy = (float)y2;}

		if (x1 < minx) { minx = (float)x1;}
		if (x2 < minx) { minx = (float)x2;}
		if (y1 < miny) { miny = (float)y1;}
		if (y2 < miny) { miny = (float)y2;}
		
		// amarela: Logo depois the houghlinesP
		if (configInput.cue.imgId == 2) {
			cv::Point p1 = cv::Point(x1 -10 - 10* i, y1 -10 - 10* i );
			cv::Point p2 = cv::Point(x2 - 10 - 10*i, y2 -10 - 10* i);
			cv::line(imgCue,p1, p2, cv::Scalar(255,255, 0));
		}

		cv::Vec2f a;
		a[0] = x1;
		a[1] = y1;
		lines2fa.push_back(a);
		a[0] = x2;
		a[1] = y2;
		lines2fa.push_back(a);
	}

	cv::Vec4f linea;
	cv::fitLine(lines2fa, linea, CV_DIST_L2, 0, 0.01, 0.01);
	float vx = linea[0];
	float vy = linea[1];
	float x0 = linea[2];
	float y0 = linea[3];	

	cue.x1 = int(x0 - 1000 * vx);
	cue.x2 = int(x0 + 1000 * vx);
	cue.y1 = int(y0 - 1000 * vy);
	cue.y2 = int(y0 + 1000 * vy);

	if (configInput.cue.imgId == 2) {		
		//imgTaco2.copyTo(imgCue);	
		cv::cvtColor(imgTaco, imgTaco, CV_GRAY2RGB);
		//imgTaco.copyTo( imgCue( cueROI ) );
		imgTaco.copyTo( imgCue);//( cueROI ) );
		cv::cvtColor(imgTaco, imgTaco, CV_RGB2GRAY);
		cv::rectangle(imgCue, cueROI, cv::Scalar(100, 100, 100), 1);
	}

	cv::Point p1a = cv::Point(cue.x1, cue.y1);
	cv::Point p2a = cv::Point(cue.x2, cue.y2);	
	if (configInput.cue.imgId == 2) {
		cv::line(imgCue,p1a, p2a, cv::Scalar(0, 0, 255));
	} 

	cv::Rect newCueROI = cueROI;

	//if (configInput.cue.precisionLevel == 0) {
	// Aqui fazemos o clipping da linha
	float a;
	AdjustLineDirectionToPoolCenter(cue);	
	if (cue.y1 < miny) {
		cue.y1 = (int)miny;
		a = (float(cue.y1) - y0)/vy;
		cue.x1 = (int)(x0 + a * vx);
	}	
	if (cue.y2 > maxy) {
		cue.y2 = (int)maxy;
		a = (float(cue.y2) - y0)/vy;
		cue.x2 = (int)(x0 + a * vx);
	}
	if (cue.x1 < minx) {
		cue.x1 = (int)minx;
		a = ((float(cue.x1) - x0)/vx);
		cue.y1 = (int)(y0 + a * vy);
	}	
	if (cue.x2 > maxx) {
		cue.x2 = (int)maxx;
		a = ((float(cue.x2) - x0)/vx);
		cue.y2 = (int)(y0 + a * vy);
	}

	// testa tambem para vetor invertido (x2 < x1, etc)
	if (cue.y1 > maxy) {
		cue.y1 = (int)maxy;
		a = (float(cue.y1) - y0)/vy;
		cue.x1 = (int)(x0 + a * vx);
	}	
	if (cue.y2 < miny) {
		cue.y2 = (int)miny;
		a = (float(cue.y2) - y0)/vy;
		cue.x2 = (int)(x0 + a * vx);
	}
	if (cue.x1 >maxx) {
		cue.x1 = (int)maxx;
		a = ((float(cue.x1) - x0)/vx);
		cue.y1 = (int)(y0 + a * vy);
	}	
	if (cue.x2 < minx) {
		cue.x2 = (int)minx;
		a = ((float(cue.x2) - x0)/vx);
		cue.y2 = (int)(y0 + a * vy);
	}

	cv::Point cueP1 = cv::Point(cue.x1, cue.y1);
	cv::Point cueP2 = cv::Point(cue.x2, cue.y2);
	bool ok = clipLine(imgHSV.size(), cueP1, cueP2);
	configOutput.cueFound = configOutput.cueFound && ok;
	cue.x1 = cueP1.x;
	cue.y1 = cueP1.y;
	cue.x2 = cueP2.x;
	cue.y2 = cueP2.y;


	cv::Rect cueRect;// = cv::Rect(cue.x1, cue.y1, abs(cue.x2-cue.x1), abs(cue.y2);

	if ((configOutput.cueFound) && (configInput.cue.imgId != 0)) {
		//cueROI usa dimensoes da imgHSV, portanto tableLimits nao se aplica aqui			
		int w = abs(cue.x2-cue.x1);
		int h = abs(cue.y2-cue.y1);
		int x = cue.x1;
		if (x > cue.x2) {x = cue.x2;}		
		int y = cue.y1;		
		if (y > cue.y2) {y = cue.y2;}

		cueRect = cv::Rect(x, y, w, h);

		if (x < 10) {
			x = 10;
		} else {
			x-=10;
		}
		if (y < 10) {
			y = 10;
		} else {
			y-=10;
		}		
		w += 20;
		h += 20;

		if (x+w > imgHSV.cols-1) { w = imgHSV.cols-1-x;}
		if (y+h > imgHSV.rows-1) { h = imgHSV.rows-1-y;}
		newCueROI = cv::Rect(x,y,w,h);
		y++;
	}

	//SleepEx(2000, true);
		
	//} else { // precisionLevel != 0
	if (configInput.cue.precisionLevel == 1) {
		//cv::Mat imgTacoROI = imgTaco(imgRect);	
		// Faz nova busca, agora usando houghlines padrao e limitando-se a buscar dentro da area
		// onde foram encontradas as linhas anteriores
		vector<cv::Vec2f> lines2f;
		if (configInput.cue.canny_apply) {
			//cv::Canny(imgTaco, imgTaco, 10, 10, 5, false);
			cv::Canny(imgTaco, imgTaco, configInput.cue.canny_threshold1, 
				configInput.cue.canny_threshold2, configInput.cue.canny_apertureSize, configInput.cue.canny_L2gradient);
		}

		if (configInput.cue.imgId == 3) {		
			//cv::cvtColor(imgTaco, imgCue, CV_GRAY2RGB);
			cv::cvtColor(imgTaco, imgTaco, CV_GRAY2RGB);
			//imgTaco.copyTo( imgCue( cueROI ) );
			imgTaco.copyTo( imgCue);//( cueROI ) );
			cv::cvtColor(imgTaco, imgTaco, CV_RGB2GRAY);
			cv::rectangle(imgCue, cueROI, cv::Scalar(100, 100, 100), 1);
		}

		//cv::HoughLines(imgTaco, lines2f, 1, CV_PI/180.0, 100,0, 0);	
		cv::HoughLines(imgTaco, lines2f, configInput.cue.hl_rho, 
			configInput.cue.hl_theta, configInput.cue.hl_threshold,0, 0);	

		if (lines2f.size() == 0) {
			// Se nao encontrou linhas com tamanho minimo 100, busca linhas com tamanho
			// minimo 80 ou 50. Quanto maior o tamanho minimo, mas precisa sera a 
			// linha encontrada, por isso iniciamos com um numero maior
			cv::HoughLines(imgTaco, lines2f, 1, CV_PI/180.0, 80,0, 0);
			if (lines2f.size() == 0) {
				cv::HoughLines(imgTaco, lines2f, 1, CV_PI/180.0, 50,0, 0);			
			}
		}
		
		// Apos hughLine, temos apenas a equacao da linha
		// Precisamos calcular varios pontos e usar a funcao fitLine para
		// encontrar uma linha que contenha esses pontos (Reducao linear)
		cv::Vec4f line;
		vector<cv::Point> points;

		for( size_t i = 0; i < lines2f.size(); i++ )
		{
			float rho = lines2f[i][0], theta = lines2f[i][1];
			float diff = abs(theta - 0.001f);
			if (diff < 0.002f) {
				theta = 0.01f;
			}
			cv::Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;

			pt1.x = cvRound(x0 + 1000*(-b));
			pt1.y = cvRound(y0 + 1000*(a));
			pt2.x = cvRound(x0 - 1000*(-b));
			pt2.y = cvRound(y0 - 1000*(a));
			points.push_back(pt1);
			points.push_back(pt2);
			
			// verde: Logo depois de houghLines
			if (configInput.cue.imgId == 4) {
				pt1.x = pt1.x + 10 + 10* i;
				pt1.y += 10 + 10* i;
				pt2.x = pt2.x + 10 + 10* i;
				pt2.y +=  10 + 10* i;			
				cv::line(imgCue,pt1, pt2, cv::Scalar(100, 150, 0));
			}
		}

		if (lines2f.size() == 0) {
			configOutput.cueFound = false;
			return;
		}

		cv::fitLine(points, line, CV_DIST_L2, 0, 0.01, 0.01);
		vx = line[0];
		vy = line[1];
		x0 = line[2];
		y0 = line[3];	

		// x0,y0 eh um ponto pelo qual a linha media passa
		// vx e vy sao vetores para calcular outros pontos da linha.
		// ex.: x = x0 + a * vx, onde a pode ser qualquer valor

		// Calculamos valores extremos para os limites do taco
		// Depois fazemos um clipping no retangulo encontrado no 
		// primero huglinesp
		cue.x1 = int(x0 - 1000 * vx);
		cue.x2 = int(x0 + 1000 * vx);
		cue.y1 = int(y0 - 1000 * vy);
		cue.y2 = int(y0 + 1000 * vy);

		// Linha branca: Final unclipped
		cv::Point pt1 = cv::Point(cue.x1 + 20, cue.y1 + 20);
		cv::Point pt2 = cv::Point(cue.x2 + 20, cue.y2 + 20);
		if (configInput.cue.imgId == 4) {
			cv::line(imgCue,pt1, pt2, cv::Scalar(255, 255, 255));
		}
		
	
		// Aqui fazemos o clipping da linha
		float a;
		AdjustLineDirectionToPoolCenter(cue);	
		if (cue.y1 < miny) {
			cue.y1 = (int)miny;
			a = (float(cue.y1) - y0)/vy;
			cue.x1 = (int)(x0 + a * vx);
		}
	
		if (cue.y2 > maxy) {
			cue.y2 = (int)maxy;
			a = (float(cue.y2) - y0)/vy;
			cue.x2 = (int)(x0 + a * vx);
		}
		if (cue.x1 < minx) {
			cue.x1 = (int)minx;
			a = ((float(cue.x1) - x0)/vx);
			cue.y1 = (int)(y0 + a * vy);
		}
	
		if (cue.x2 > maxx) {
			cue.x2 = (int)maxx;
			a = ((float(cue.x2) - x0)/vx);
			cue.y2 = (int)(y0 + a * vy);
		}
		
		if (configInput.cue.imgId > 3) {
			//Desenha a linha calculada em houghlinesp apenas para comparacao
			cv::line(imgCue,p1a, p2a, cv::Scalar(0,0, 255));
			
			// Desenha a nova linha
			pt1 = cv::Point(cue.x1, cue.y1);
			pt2 = cv::Point(cue.x2, cue.y2);
			cv::line(imgCue,pt1, pt2, cv::Scalar(255, 0, 0));	
		}

		/*cue.x1 += cueROI.x;
		cue.y1 += cueROI.y;
		cue.x2 += cueROI.x;
		cue.y2 += cueROI.y;*/


	}

	
	if (cue.x1 < 0) { cue.x1 = 0;}
	if (cue.y1 < 0) { cue.y1 = 0;}
	if (cue.x2 < 0) { cue.x2 = 0;}
	if (cue.y2 < 0) { cue.y2 = 0;}

	AdjustLineDirectionToPoolCenter(cue);	

	// Precisao extra: Encontra os limites da imagem do taco ao redor da linha encontrada,
	// e depois calcula uma nova linha, que eh a mediana dessas duas novas linhas encontradas
	// Isso permite calcular a linha que passa mais perto do centro do taco, ao contrario das
	// funcoes padro do OpenCV, que encontram alguma linha em uma posicao qualquer dentro do taco

	if (configOutput.cueFound == false) {
		return;
	}

	LineEquation eq;
	GetLineEquation (cue, eq);

	bool goHorizontal = true;
	int delta = cue.x2 - cue.x1;
	if (abs(delta) < abs(cue.y1 - cue.y2)) {
		goHorizontal = false;
		delta = cue.y2 - cue.y1;
	}
	int step = 1;
	if (delta < 0) {
		step = -1;
		delta = -delta;
	}
	cv::Vec4f limLine1, limLine2;
	vector<cv::Vec2f> limPoints1, limPoints2;
	unsigned char * pixelPtr = (unsigned char *)(imgTaco2.data);
	int cols = imgTaco2.cols;
	int lins = imgTaco2.rows;

	if (configInput.cue.imgId == 5) {		
		cv::cvtColor(imgTaco2, imgTaco2, CV_GRAY2RGB);
		imgTaco2.copyTo(imgCue);
		cv::cvtColor(imgTaco2, imgTaco2, CV_RGB2GRAY);
		cv::rectangle(imgCue, cueROI, cv::Scalar(100, 100, 100), 1);
	}

	for (int i=0; i < delta; i+=3) {		
		float x,y;
		if (goHorizontal) {
			x = cue.x1 + i * step;
			y = x * eq.m + eq.h;
		} else {
			y = cue.y1 + i * step;
			x = (y - eq.h) / eq.m;
		}

		// calcula a normal
		LineEquation normalEq;
		ScreenLine newLine;
		newLine.x1 = cue.x1;
		newLine.y1 = cue.y1;
		newLine.x2 = x;
		newLine.y2 = y;
		GetLineNormal(newLine, normalEq);

		cv::Vec2f point1, point2;
		point1[0] = x;
		point1[1] = y;
		point2[0] = x;
		point2[1] = y;

		for (int j=0; j < 10; j++) {	
			float xn1,yn1;
			float xn2,yn2;
			if (goHorizontal == false) { // na normal essa logica eh invertida, pois ela esta rotacionada 90 graus
				xn1 = x + j;
				yn1 = xn1 * normalEq.m + normalEq.h;
				xn2 = x - j;
				yn2 = xn2 * normalEq.m + normalEq.h;
			} else {
				yn1 = y + j;
				xn1 = (y - normalEq.h) / normalEq.m;
				yn2 = y - j;
				xn2 = (y - normalEq.h) / normalEq.m;
			}

			if ((xn1 < 0) || (xn1 >= cols)) { continue;}
			if ((yn1 < 0) || (yn1 >= lins)) { continue;}
			if ((xn2 < 0) || (xn2 >= cols)) { continue;}
			if ((yn2 < 0) || (yn2 >= lins)) { continue;}
			//unsigned char pixel1 = pixelPtr[int( (yn1) * cols + (xn1) )]; 
			//unsigned char pixel2 = pixelPtr[int( (yn2) * cols + (xn2) )]; 
			unsigned char pixel1 = imgTaco2.at <uchar> (yn1, xn1);
			unsigned char pixel2 = imgTaco2.at <uchar> (yn2, xn2);
			//pixelPtr[int( (yn1 - cueROI.y) * cols + (xn1 - cueROI.x) )] = 0;
			//pixelPtr[int( (yn2 - cueROI.y) * cols + (xn2 - cueROI.x) )] = 0; 
			
			if (pixel1 > 100) {
				point1[0] = xn1;
				point1[1] = yn1;
			}
			if (pixel2 > 100) {
				point2[0] = xn2;
				point2[1] = yn2;
			}
		}	
		/*if (configInput.cue.imgId == 5) {		
			cv::Point pt1 = cv::Point(point1[0],point1[1]);
			cv::Point pt2 = cv::Point(point2[0],point2[1]);
			cv::line(imgCue,pt1, pt2, cv::Scalar(0, 255, 255));	
		}*/
		limPoints1.push_back(point1);
		limPoints2.push_back(point2);
	}

	if ( (limPoints1.size() > 0) && (limPoints2.size() > 0)) {
		cv::fitLine(limPoints1, limLine1, CV_DIST_L2, 0, 0.01, 0.01);
		cv::fitLine(limPoints2, limLine2, CV_DIST_L2, 0, 0.01, 0.01);
		float vx1 = limLine1[0];
		float vy1 = limLine1[1];
		float x01 = limLine1[2];
		float y01 = limLine1[3];	
		float vx2 = limLine2[0];
		float vy2 = limLine2[1];
		float x02 = limLine2[2];
		float y02 = limLine2[3];	

		float xa1,xb1,ya1,yb1;
		float xa2,xb2,ya2,yb2;

		// Desenha as duas linhas encontradas no limite do taco, em azul-piscina
		xa1 = (x01 - 1000 * vx1);
		xb1 = (x01 + 1000 * vx1);
		ya1 = (y01 - 1000 * vy1);
		yb1 = (y01 + 1000 * vy1);

		xa2 = (x02 - 1000 * vx2);
		xb2 = (x02 + 1000 * vx2);
		ya2 = (y02 - 1000 * vy2);
		yb2 = (y02 + 1000 * vy2);

		// Novo taco calculado, em azul, usando a media das duas linhas encontradas
		ScreenLine newCue = cue;
		newCue.x1 = (xa1 + xa2) / 2;
		newCue.y1 = (ya1 + ya2) / 2;
		newCue.x2 = (xb1 + xb2) / 2;
		newCue.y2 = (yb1 + yb2) / 2;

		if (configInput.cue.imgId == 5) {
			cv::Point pt1 = cv::Point(xa1,ya1);
			cv::Point pt2 = cv::Point(xb1,yb1);
			cv::line(imgCue,pt1, pt2, cv::Scalar(0, 255, 255));	

			pt1 = cv::Point(xa2,ya2);
			pt2 = cv::Point(xb2,yb2);
			cv::line(imgCue,pt1, pt2, cv::Scalar(0, 255, 255));	
	
			pt1 = cv::Point(newCue.x1,newCue.y1);
			pt2 = cv::Point(newCue.x2,newCue.y2);
			cv::line(imgCue,pt1, pt2, cv::Scalar(0, 0, 255));	

			// Taco calculado anteriormente, em vermelho
			pt1 = cv::Point(cue.x1,cue.y1);
			pt2 = cv::Point(cue.x2,cue.y2);
			cv::line(imgCue,pt1, pt2, cv::Scalar(255, 0, 0));			

			//cv::Rect cueRect2 = cv::Rect(cueRect.x-tableLimits.x, cueRect.y-tableLimits.y, cueRect.width, cueRect.height);
			cv::rectangle (imgCue, cueRect, cv::Scalar(255, 255, 0));	
		}

		cue = newCue;
	}
	// Ajustamos a linha encontrada ao tamanho da imagem, descontados
	// os limites da mesa

	cv::Point p1 = cv::Point(cue.x1, cue.y1);
	cv::Point p2 = cv::Point(cue.x2, cue.y2);

	cv::clipLine(cueRect, p1, p2);
	cue.x1 = p1.x; cue.y1 = p1.y;
	cue.x2 = p2.x; cue.y2 = p2.y;

	cue.x1 += tableLimits.x; 
	cue.y1 += tableLimits.y;
	cue.x2 += tableLimits.x;	
	cue.y2 += tableLimits.y;	
	
	AdjustLineDirectionToPoolCenter(cue);	

	// Usa as ultimas medias do angulo do taco pra evitar que ele
	// fique pulando na tela.	
	//float dev = prevAngleDeviation; 

	/*cueHistory[cueHistoryCount] = cue;
	++cueHistoryCount;
	if (cueHistoryCount == configInput.cueHistorySize) {		
		float mx1=0; float mx2=0; float my1=0; float my2=0;
		for (int i=0; i < configInput.cueHistorySize; i++) {
			mx1 += cueHistory[i].x1;
			my1 += cueHistory[i].y1;
			mx2 += cueHistory[i].x2;
			my2 += cueHistory[i].y2;
		}
		cue.x1 = mx1 / configInput.cueHistorySize;
		cue.x2 = mx2 / configInput.cueHistorySize;
		cue.y1 = my1 / configInput.cueHistorySize;
		cue.y2 = my2 / configInput.cueHistorySize;

		cueHistoryCount = (configInput.cueHistorySize)-1;
		memcpy(cueHistory, &(cueHistory[1]), sizeof(ScreenLine) * (configInput.cueHistorySize - 1));
	}*/

	//prevAngleDeviation = dev;	
	cueROI = newCueROI;
	GetLineEquation (cue, eq);
	currentCueAngle = eq.m;	
	configOutput.cue = cue;
}

//-----------------------------------------------------------------------------

void Detector::FindCue2(void)
{
	cv::Mat imgTaco;	
	configOutput.cueFound = false;

	if (configInput.cue.imgId == 0) {		
		cv::inRange(imgHSV, cv::Scalar(configInput.cueColors.lh,configInput.cueColors.ls,configInput.cueColors.lv), 
		cv::Scalar(configInput.cueColors.uh,configInput.cueColors.us,configInput.cueColors.uv), imgCue);
		cv::cvtColor(imgCue, imgCue, CV_GRAY2RGB);
		return;
	}

	cv::inRange(imgHSV(cueROI), cv::Scalar(configInput.cueColors.lh,configInput.cueColors.ls,configInput.cueColors.lv), 
			cv::Scalar(configInput.cueColors.uh,configInput.cueColors.us,configInput.cueColors.uv), imgTaco);

	if (configInput.cue.imgId >= 0) {		
		cv::cvtColor(imgHSV, imgCue, CV_HSV2RGB);
		if (configInput.cue.imgId == 0) {		
			cv::cvtColor(imgTaco, imgTaco, CV_GRAY2RGB);
			imgTaco.copyTo( imgCue( cueROI ) );
			cv::cvtColor(imgTaco, imgTaco, CV_RGB2GRAY);
			cv::rectangle(imgCue, cueROI, cv::Scalar(100, 100, 100), 1);
		}
	}
	
	if (configInput.cue.erodeSize > 0) {
		int erosionSize = configInput.cue.erodeSize;
		cv::Mat erosionCue = cv::getStructuringElement(cv::MORPH_RECT, 
										   cv::Size(2*erosionSize + 1, 2*erosionSize+1),
										   cv::Point( erosionSize, erosionSize ) );
		cv::erode(imgTaco, imgTaco, erosionCue);
	}

	if (configInput.cue.dilateSize > 0) {
		int erosionSize = configInput.cue.dilateSize;
		cv::Mat erosionCue = cv::getStructuringElement(cv::MORPH_RECT, 
										   cv::Size(2*erosionSize + 1, 2*erosionSize+1),
										   cv::Point( erosionSize, erosionSize ) );
		cv::dilate(imgTaco, imgTaco, erosionCue);		
	}

	if (configInput.cue.imgId == 1) {		
		cv::cvtColor(imgTaco, imgTaco, CV_GRAY2RGB);
		imgTaco.copyTo( imgCue( cueROI ) );
		cv::cvtColor(imgTaco, imgTaco, CV_RGB2GRAY);
		cv::rectangle(imgCue, cueROI, cv::Scalar(100, 100, 100), 1);
	}

	//cv::Canny(imgTaco, imgTaco, configInput.cue.canny_threshold1, 
	//			configInput.cue.canny_threshold2, configInput.cue.canny_apertureSize, configInput.cue.canny_L2gradient);
	
	vector<vector<cv::Point>> contours;	
	cv::Vec4f lines;
	cv::findContours(imgTaco,contours, CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

	//vector<int> useContours;
	//cv::Mat points;

	vector<cv::Vec2f> points;

	int biggerSize = 0;
	int biggerId = 0;
	for (int j=0; j < contours.size(); j++) {
		if (biggerSize < contours.at(j).size()) {
			biggerSize = contours.at(j).size();
			biggerId = j;
		}

		cv::fitLine(cv::Mat(contours[j]),lines, CV_DIST_HUBER, 0, 0.01, 0.01);
		float vx = lines[0];
		float vy = lines[1];
		float x0 = lines[2];
		float y0 = lines[3];	
		bool add = true;
		for (int i=0; i < contours[j].size(); i++) {
			int xe = contours[biggerId].at(i).x;
			int ye = contours[biggerId].at(i).y;

			float a = (float(ye) - y0)/vy;
			int xa = (int)(x0 + a * vx);

			if (abs(xa-xe) > 2) {
				add = false;
			}
		}
		if (add) {
			for (int i=0; i < contours[j].size(); i++) {
				cv::Vec2f p;
				p[0] = contours[j].at(i).x;
				p[1] = contours[j].at(i).y;
				points.push_back(p);		
			}
		}
	}


	//cv::fitLine(cv::Mat(contours[biggerId]),lines, CV_DIST_HUBER, 0, 0.01, 0.01);
	cv::fitLine(points,lines, CV_DIST_HUBER, 0, 0.01, 0.01);

	int lefty = (-lines[2]*lines[1]/lines[0])+lines[3];
	int righty = ((imgCue.cols-lines[2])*lines[1]/lines[0])+lines[3];

	cv::line(imgCue,cv::Point(imgCue.cols-1,righty),cv::Point(0,lefty),cv::Scalar(255,0,0),1);

	if (configInput.cue.imgId == 2) {		
		cv::drawContours(imgCue,contours,-1, cv::Scalar(0, 255, 0));
	}

	cv::Point p1 = cv::Point(0,0);
	
	for (int i=0; i < contours[biggerId].size(); i++) {
		int x = contours[biggerId].at(i).x;
		int y = contours[biggerId].at(i).y;
		cv::Point p2 = cv::Point(x,y);
		cv::line(imgCue, p1, p2, cv::Scalar(0, 0, 255));
		p1 = p2;
	}

	configOutput.cueFound = false;

	//vector<cv::Vec4i> lines4i;	
	
	//cv::Mat imgTaco2;	

}

