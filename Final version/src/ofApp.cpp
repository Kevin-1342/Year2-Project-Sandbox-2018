#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	
	currentTheme = SANDBOX_THEME;
	// OF basics
	ofSetFrameRate(60);
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_WARNING);
	ofSetLogLevel("ofFbo", OF_LOG_ERROR);
	ofSetLogLevel("ofShader", OF_LOG_ERROR);
	ofSetLogLevel("ofxKinect", OF_LOG_WARNING);

	// Setup kinectProjector
	kinectProjector = std::make_shared<KinectProjector>(projWindow);
	kinectProjector->setup(true);
	//kinectProjector2 = std::make_shared<KinectProjector>(pcjWindow);
	//kinectProjector2->setup(true);
	// Setup sandSurfaceRenderer
	sandSurfaceRenderer = new SandSurfaceRenderer(kinectProjector, projWindow);
	sandSurfaceRenderer->setup(true);

	// Retrieve variables
	ofVec2f kinectRes = kinectProjector->getKinectRes();
	ofVec2f projRes = ofVec2f(projWindow->getWidth(), projWindow->getHeight());
	ofRectangle kinectROI = kinectProjector->getKinectROI();
	mainWindowROI = ofRectangle((ofGetWindowWidth() - kinectRes.x) / 2, (ofGetWindowHeight() - kinectRes.y) / 2, kinectRes.x, kinectRes.y);
	cout << "Kinect roi" << endl;
	cout<< kinectROI .x<<endl;
	cout << kinectROI.y << endl;
	cout << kinectROI.width << endl;
	cout << kinectROI.height << endl;
	cout << "projRes rec" << endl;
	cout << projRes.x << endl;
	cout << projRes.y << endl;
	shapeGameManager.setProjectorRes(projRes);
	shapeGameManager.setKinectRes(kinectRes);
	shapeGameManager.setKinectROI(kinectROI);
	shapeGameManager.loadLeaderBoard();
	shapeGameManager.saveLeaderBoard();


}

//--------------------------------------------------------------
void ofApp::update(){
	//Theme
	
	if (changeTheme) {
		changeTheme = false;
		if (currentTheme == SANDBOX_THEME) {
			sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMap.xml");
			sandSurfaceRenderer->drawColorLine(false);
		}else if (currentTheme == LUNAR_THEME) {
			sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMapL.xml");
			PlaySound(TEXT("c:\\smallStep.wav"), NULL,  SND_ASYNC | SND_NODEFAULT);
			sandSurfaceRenderer->drawColorLine(true);
		}
	}

	//Flood
	if (sandboxFlood) {
		for (int i = 14; i >=0; i--) {
			sandSurfaceRenderer->setHeightKey(i, sandSurfaceRenderer->heightMap.getHeight(i)+floodSpeed);
		}
		if (ofGetElapsedTimef()-floodTimer>7) {
			sandboxFlood = false;
		}
		if (ofGetElapsedTimef() - floodTimer<0.1) {
			PlaySound(TEXT("c:\\4424.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
		}
		if (ofGetElapsedTimef() - floodTimer>2&& ofGetElapsedTimef() - floodTimer<2.1) {
			PlaySound(TEXT("c:\\4424.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
		}
		if (ofGetElapsedTimef() - floodTimer>4&& ofGetElapsedTimef() - floodTimer<4.1) {
			PlaySound(TEXT("c:\\4424.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
		}

	}
	kinectProjector->update();
	sandSurfaceRenderer->update();

	//Shape game
	if (shapeGameRunning) {
		if (ofGetElapsedTimef()-elapsedTime<5) {

		}else{
			if (ofGetElapsedTimef()-similarityCheckTimer>3) {
				similarityCheckTimer = ofGetElapsedTimef();
				getBinaryImage();
				//shapeGameManager.drawScore(shapeGame_Similarity);
				if (shapeGameManager.maxOccupationCutout()) {
					if (shapeGameManager.calSimilarity()) {
						shapeGame_Similarity = shapeGameManager.getShapeGame_Similarity();
					}
					else {
						shapeGame_Similarity = 0;
					}
				}
				else {
					shapeGame_Similarity = 0;
				}
			}

		}

	}



}

//--------------------------------------------------------------
void ofApp::draw(){
	float x = mainWindowROI.x;
	float y = mainWindowROI.y;
	float w = mainWindowROI.width;
	float h = mainWindowROI.height;

	if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
	{
		sandSurfaceRenderer->drawMainWindow(x, y, w, h);//400, 20, 400, 300);
	}

	kinectProjector->drawMainWindow(x, y, w, h);
}

void ofApp::drawProjWindow(ofEventArgs &args)
{
	if (kinectProjector->GetApplicationState() == KinectProjector::APPLICATION_STATE_RUNNING)
	{
		sandSurfaceRenderer->drawProjectorWindow();

	}
	kinectProjector->drawProjectorWindow();
	if (shapeGameRunning) {
		shapeGameShowFinal = false;
		if (ofGetElapsedTimef() - elapsedTime<5) {
			ofImage Img_Bac(shapeGameManager.getColorMapPath());
			Img_Bac.draw(0,0);
		}
		else {
			ofTrueTypeFont text;
			float maxTime;
			//ofSetColor(255, 0, 0);
			text.load("ofxbraitsch/fonts/Verdana.ttf", 40);
			stringstream ttext;
			string ScoreText;
			//int tscore = (int)shapeGame_Similarity;
			ttext << shapeGameManager.getMapName();
				//<<shapeGame_Similarity * 100 << "%";
			ScoreText = ttext.str();
			ofSetColor(0,0,0);
			text.drawString(ScoreText, 50, 100);
			ofSetColor(0,0,0,95);
			ofFill();
			ofDrawRectangle(50,148, 300, 34);
			if (shapeGame_Similarity<1) {
				shapeGame_Similarity += 0.001;
			}
			
			if (shapeGame_Similarity<0.7) {
				ofSetColor(255, 0, 0);
				ofDrawRectangle(50, 150, shapeGame_Similarity * 300, 30);
			}
			else  {
				ofSetColor(0, 255, 0);
				ofDrawRectangle(50, 150, shapeGame_Similarity * 300, 30);
			}
			//text.drawString(ScoreText, 100, 230);
			//if (shapeGame_Similarity>0.85) {
				//text.drawString(shapeGameManager.getMapName()+"shape found", 50, 230);
			//}
			//stringstream timeText;
			int time;
			if (shapeGameManager.getShapeNum()<3) {
				time = (int)65 - (ofGetElapsedTimef() - elapsedTime);
				maxTime = 60;
			}
			else if (shapeGameManager.getShapeNum()<4&& shapeGameManager.getShapeNum()>3) {
				time = (int)85 - (ofGetElapsedTimef() - elapsedTime);
				maxTime = 80;
			}
			else {
				time = (int)185 - (ofGetElapsedTimef() - elapsedTime);
				maxTime = 180;
			}
			
			//timeText << time <<"s";
			//ScoreText = timeText.str();
			//text.drawString(ScoreText, 1000, 100);
			ofSetColor(0, 0, 0,70);
			ofFill();
			ofDrawRectangle(900,30,300,30);
			if (time <= 10) {
				ofSetColor(255, 0, 0);
				ofDrawRectangle(900, 30, (maxTime - time) / maxTime * 300, 30);
			}
			else if (time < 20&&time>10) {
				ofSetColor(255, 180, 0);
				ofDrawRectangle(900, 30, (maxTime - time) / maxTime * 300, 30);
			}
			if (time<=10 && time > 9.9) {
				PlaySound(TEXT("c:\\11004.wav"), NULL,  SND_ASYNC | SND_NODEFAULT);
			}
			else if (time <= 20&& time > 19.9) {
				PlaySound(TEXT("c:\\11004.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
			}
			else if(time >= 20) {
				PlaySound(TEXT("c:\\10377.wav"), NULL, SND_NOSTOP | SND_ASYNC | SND_NODEFAULT);
				ofSetColor(0, 255, 0);
				ofDrawRectangle(900, 30, (maxTime - time) / maxTime * 300, 30);
			}
			
			//end game
			if (time<=0) {
				shapeGameRunning = false;
				shapeGameShowFinal = true;
				shapeGame_FinalScroe = shapeGame_Similarity*100;
				elapsedTime = ofGetElapsedTimef();
				if (shapeGame_FinalScroe > shapeGameManager.getMaxScore(shapeGameManager.getShapeNum())) {
					shapeGameNewRecord = true;
				}
				shapeGameManager.addScore(shapeGameManager.getShapeNum(),shapeGame_FinalScroe);
			}
			ofSetColor(255, 255, 255);
		}

	}
	if (shapeGameShowFinal) {
		ofTrueTypeFont text;
		text.load("ofxbraitsch/fonts/Verdana.ttf", 50);
		stringstream ttext;
		string ScoreText;
		ttext << "Final score:" << shapeGame_FinalScroe  << "%";
		ScoreText = ttext.str();
		ofSetColor(0, 0, 0);
		text.drawString(ScoreText, 350, 500);
		if (shapeGameNewRecord) {
			stringstream htext;
			string HighScoreText;
			ofSetColor(255, 0, 0);
			htext << "New Record!";
			HighScoreText = htext.str();
			text.drawString(HighScoreText, 450, 600);
		}
		if (ofGetElapsedTimef()- elapsedTime>5) {
			shapeGameShowFinal = false;
			shapeGameNewRecord = false;
		}
		if (ofGetElapsedTimef() - elapsedTime<0.1) {
			PlaySound(TEXT("c:\\9347.wav"), NULL,  SND_ASYNC | SND_NODEFAULT);
		}
		ofSetColor(255, 255, 255);
	}
	
}

//abcde
void ofApp::drawSUIWindow(ofEventArgs &args) {
	/*
	//Lunar image
	//ofImage Img_Bac("bac.png");
	//Img_Bac.draw(0, 0);
	//ofImage Img_Lunar("lunar4.png");
	ofSetBackgroundColor(255, 255, 255);
	ofImage Img_Sandbox("SandboxBack.png");
	ofImage Img_Lunar("LunarBack.png");
	//Img_Lunar.draw(40, 345);     //x,y
	Img_Lunar.draw(40, 10);
								 //Sandbox image
	
    //Img_Sandbox.draw(580, 345);
	Img_Sandbox.draw(580, 10);
	//Shape Island Game
	//ofImage Img_ShapeIsland("shapeGame.png");
	ofImage Img_ShapeIsland("ShapeBack.png");
	//Img_ShapeIsland.draw(40, 930);
	Img_ShapeIsland.draw(40, 850);

	ofImage Img_Score("ScoreBack.png");
	//Img_ShapeIsland.draw(40, 930);
	Img_Score.draw(580, 850);*/

	ofTrueTypeFont text;
	ofSetBackgroundColor(255, 255, 255);
	//Lunar image
	ofImage Img_Lunar("lunar5.jpg");
	Img_Lunar.draw(40, 345);     //x,y
								 //Sandbox image
	ofImage Img_Sandbox("sandbox.png");
	Img_Sandbox.draw(580, 345);
	//Shape Island Game
	ofImage Img_ShapeIsland("shapeGame.png");
	Img_ShapeIsland.draw(40, 930);

	ofSetColor(11, 253, 247);
	ofSetLineWidth(5);
	ofNoFill();
	ofSetLineWidth(20);
	ofSetColor(150, 150, 150);
	ofDrawRectangle(40, 345, 460, 500);  //x,y
	ofSetColor(0, 255, 0);
	ofDrawRectangle(580, 345, 460, 500);
	ofSetColor(0, 191, 255);
	ofDrawRectangle(40, 930, 460, 880);
	ofSetColor(0, 191, 255);
	ofDrawRectangle(580, 930, 460, 880);

	//introduction
	ofSetColor(100, 191, 255);
	ofDrawRectangle(40, 30, 1000, 260);


	//lunar start
	ofSetColor(255, 215, 0);
	ofSetLineWidth(6);
	ofDrawRectangle(128, 668, 275, 30);
	//sandbox start
	ofSetColor(0, 191, 255);
	ofSetLineWidth(6);
	ofDrawRectangle(668, 668, 275, 30);
	ofDrawRectangle(668, 728, 275, 30);

	//shape game start
	ofSetColor(248, 248, 255);
	ofSetLineWidth(6);
	ofDrawRectangle(128, 1428, 275, 30);
	ofDrawRectangle(128, 1478, 275, 30);
	ofDrawRectangle(128, 1528, 275, 30);
	ofDrawRectangle(128, 1578, 275, 30);
	ofDrawRectangle(128, 1628, 275, 30);
	//ofDrawCircle(100,400,300);

	ofSetColor(255, 0, 0);
	text.loadFont("verdana.ttf", 10);
	text.drawString("Click here to start", 700, 670);
	text.drawString("Click here to start", 160, 670);
	text.drawString("Click here to flood the water", 700, 730);

	text.drawString("Click here to create Great Britain Map", 160, 1430);
	text.drawString("Click here to create a Triangle", 160, 1480);
	text.drawString("Click here to create a Circle", 160, 1530);
	text.drawString("Click here to create a Square", 160, 1580);
	text.drawString("Click here to create a Heart", 160, 1630);
	//Lunar Theme

	ofSetColor(169, 169, 169);
	text.loadFont("verdana.ttf", 40);
	text.drawString("Lunar Theme", 70, 405);


	//Sandbox Theme
	ofSetColor(0, 255, 127);
	text.loadFont("verdana.ttf", 34);
	text.drawString("Sandbox Theme", 630, 405);


	//Shape an Island Game
	ofSetColor(255, 69, 0);
	text.loadFont("verdana.ttf", 25);
	text.drawString("Make a Shape Game", 70, 970);

	ofSetColor(255, 69, 0);
	text.loadFont("verdana.ttf", 25);
	text.drawString("Leaderboard", 610, 970);

	//ofSetColor(255, 30, 0);
	//text.loadFont("cooperBlack.ttf", 18);
	//text.drawString("1st. t", 620, 1015);
	//text.drawString("2nd. t", 620, 1065);
	//text.drawString("3rd. t", 620, 1115);
	//text.drawString("4th. t", 620, 1165);

	ofFill();
	int e = 400;
	int r = 1000;
	ofSetColor(255, 250, 205);
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + e, 135 + r);
	ofVertex(215 + e, 135 + r);
	ofVertex(365 + e, 25 + r);
	ofVertex(305 + e, 200 + r);
	ofVertex(250 + e, 25 + r);
	ofVertex(305 + e, 700 + r);
	ofEndShape();

	ofEnableAlphaBlending();    // turn on alpha blending
	ofSetColor(238, 233, 191, 165); // red, 50% transparent
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + e + 100, 135 + r + 100);
	ofVertex(215 + e + 100, 135 + r + 100);
	ofVertex(365 + e + 100, 25 + r + 100);
	ofVertex(305 + e + 100, 200 + r + 100);
	ofVertex(250 + e + 100, 25 + r + 100);
	ofVertex(305 + e + 100, 700 + r + 100);
	ofEndShape();

	ofDisableAlphaBlending();   // turn off alpha
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + e + 50, 135 + r + 50);
	ofVertex(215 + e + 50, 135 + r + 50);
	ofVertex(365 + e + 50, 25 + r + 50);
	ofVertex(305 + e + 50, 200 + r + 50);
	ofVertex(250 + e + 50, 25 + r + 50);
	ofVertex(305 + e + 50, 700 + r + 50);
	ofEndShape();
	/*text.drawString("1st. testName1", 630, 480);
	text.drawString("2nd. testName2", 630, 530);
	text.drawString("1st. testName1", 70, 1240);*/

	//introduction
	ofSetColor(50, 105, 127);
	text.loadFont("verdana.ttf", 35);
	text.drawString("Introductions for four areas: ", 50, 80);

	ofSetColor(50, 115, 127);
	text.loadFont("verdana.ttf", 20);
	text.drawString("1.Lunar Theme: The sandbox will show the surface of the moon.", 50, 120);
	text.drawString("2.Sandbox Theme: The sandbox will show the surface of the earth.", 50, 155);
	text.drawString("3.Make a Shape: Choose the shape to create.", 50, 190);
	text.drawString("4.leaderboard: The similarity check of the shape game.", 50, 225);
	ofSetColor(255,255,255,0);
	ofImage Img_GB("GBShape.png");
	Img_GB.draw(410, 1430);     //x,y
	ofImage Img_Tri("triangleShape.png");
	Img_Tri.draw(410, 1470);     //x,y

	ofImage Img_Circle("circleShape.png");
	Img_Circle.draw(410, 1520);     //x,y

	ofImage Img_Square("squareShape.png");
	Img_Square.draw(410, 1570);     //x,y

	ofImage Img_Heart("heartShape.png");
	Img_Heart.draw(410, 1620);     //x,y
	ofSetColor(255, 255, 255);

}
void ofApp::drawPcjWindow(ofEventArgs &args) {

	
	int cS = currentTheme;

	ofTrueTypeFont text;
	ofSetBackgroundColor(255, 255, 255);
	////Lunar image
	//ofImage Img_Bac("bac.png");
	//Img_Bac.draw(0, 0);
	//ofImage Img_Lunar("lunar4.png");
	//Img_Lunar.draw(40, 345);     //x,y
	//							 //Sandbox image
	//ofImage Img_Sandbox("sandbox.png");
	//Img_Sandbox.draw(580, 345);
	////Shape Island Game
	//ofImage Img_ShapeIsland("shapeGame.png");
	//Img_ShapeIsland.draw(40, 930);

	//ofSetColor(11, 253, 247);
	//ofSetLineWidth(5);
	int x = 310;
	ofNoFill();
	ofSetLineWidth(20);
	ofSetColor(150, 150, 150);
	ofDrawRectangle(40, 345 - x, 460, 500);  //x,y
	ofSetColor(0, 255, 0);
	ofDrawRectangle(580, 345 - x, 460, 500);
	ofSetColor(0, 191, 255);
	ofDrawRectangle(40, 930 - x, 460, 880);
	ofDrawRectangle(580, 930 - x, 460, 880);

	//a test of circle
	ofSetColor(100, 100, 100);
	ofDrawCircle(230, 520 - x, 150);
	ofSetCircleResolution(1000);
	//ofDrawBitmapString("Moon",220,700);
	//The part of the lunar
	ofSetLineWidth(4);
	ofDrawLine(120, 500 - x, 140, 450 - x);
	ofDrawLine(140, 450 - x, 155, 500 - x);
	ofDrawLine(155, 500 - x, 175, 450 - x);
	ofDrawLine(175, 450 - x, 190, 500 - x);
	ofDrawCircle(215, 475 - x, 25);
	ofDrawCircle(265, 475 - x, 25);
	ofDrawLine(295, 450 - x, 295, 500 - x);
	ofDrawLine(295, 455 - x, 320, 455 - x);
	ofDrawLine(320, 500 - x, 320, 455 - x);

	ofSetLineWidth(4);
	ofSetCircleResolution(20);
	ofDrawCircle(150, 515 - x, 10);
	ofDrawCircle(270, 620 - x, 20);
	ofDrawCircle(200, 550 - x, 30);
	ofDrawCircle(280, 560 - x, 25);
	ofDrawCircle(450, 790 - x, 45);
	ofDrawCircle(460, 810 - x, 10);
	ofDrawCircle(440, 780 - x, 20);
	//The part of the sandbox
	ofFill();

	ofSetColor(0, 0, 80);
	ofDrawEllipse(680, 650 - x, 130, 35);

	ofSetColor(0, 191, 255);
	ofDrawEllipse(720, 635 - x, 150, 40);

	ofSetColor(176, 266, 255);        //LightSkyBlue1
	ofDrawEllipse(750, 610 - x, 160, 50);

	ofSetColor(67, 205, 128);
	ofDrawEllipse(780, 570 - x, 170, 60);
	ofSetColor(46, 139, 87);            //SeaGreen4
	ofDrawEllipse(810, 540 - x, 190, 70);

	ofSetColor(238, 232, 170);          //PaleGoldenrod
	ofDrawEllipse(830, 510 - x, 200, 75);

	ofSetColor(244, 164, 96);           //SandyBrown
	ofDrawEllipse(850, 470 - x, 230, 80);

	ofSetColor(205, 38, 38);           //Firebrick3
	ofDrawEllipse(870, 430 - x, 250, 90);


	//Draw the picture of the CO2
	int v = -15;
	ofSetColor(0, 0, 0);
	ofSetLineWidth(1);
	ofDrawLine(1002 + v, 418, 1027 + v, 443);
	ofDrawLine(998 + v, 422, 1023 + v, 447);
	ofDrawLine(998 + v, 418, 973 + v, 443);
	ofDrawLine(1002 + v, 422, 977 + v, 447);
	ofSetColor(135, 135, 135);
	ofDrawCircle(1000 + v, 420, 15);
	ofSetColor(159, 182, 205);
	ofDrawCircle(975 + v, 445, 10);
	ofDrawCircle(1025 + v, 445, 10);

	//ofDrawSphere(12.5, 0, 12.5, 25);
	//ofDrawPlane(600,100,100,1000);//no angle
	//ofDrawBox(100,100,50,50,100,50);
	//ofDrawLine();
	//ofDrawTriangle(940,625,1000,625,910,660);
	//ofDrawTriangle(1000,625,910,660,970,660);

	//shape game
	ofSetColor(99, 184, 255); //Triangle
	ofDrawTriangle(430, 1470 - x, 450, 1510 - x, 410, 1510 - x);
	//ofSetColor();
	///ofDrawTriangle(440, 1000, 400, 900, 440, 700);

	ofSetColor(193, 255, 193);  //Circle
	ofDrawCircle(430, 1545 - x, 20);

	ofSetColor(79, 79, 79);   //Rectangle
	ofDrawRectangle(410, 1580 - x, 30, 30);

	ofSetColor(255, 174, 184);  //heart shape
	ofDrawCircle(430, 1650 - x, 17.5);
	ofDrawCircle(455, 1650 - x, 17.5);
	ofDrawTriangle(415, 1660 - x, 470, 1660 - x, 442.5, 1690 - x);



	ofSetColor(0, 255, 255);
	ofSetLineWidth(3);
	ofDrawLine(410, 1430 - x, 410, 1455 - x);   //u
	ofDrawLine(410, 1455 - x, 425, 1455 - x);
	ofDrawLine(425, 1457 - x, 425, 1430 - x);

	ofDrawLine(440, 1430 - x, 440, 1455 - x);//k
	ofDrawLine(440, 1440 - x, 455, 1430 - x);
	ofDrawLine(440, 1440 - x, 455, 1455 - x);

	//ofDrawLine(140,1410)
	ofNoFill();
	ofSetColor(191, 239, 255);
	ofSetLineWidth(5);
	ofDrawRectangle(80, 970 - x, 380, 420); // the frame of the shape game


	ofFill();
	ofSetColor(0, 255, 127);    //Map of America  //
	ofDrawTriangle(100, 680, 95, 800, 200, 740);//upper triangle//
	ofSetColor(0, 255, 154, 0);
	ofDrawTriangle(100, 670, 80, 700, 205, 685);
	ofDrawTriangle(205, 700, 225, 740, 105, 810);

	ofSetColor(155, 205, 155);
	ofDrawTriangle(220, 685, 230, 710, 250, 695); //island

	ofSetColor(0, 205, 102);
	ofDrawTriangle(100, 740, 90, 775, 170, 830);  //the triangle connecting with south America

	ofSetColor(84, 255, 159);
	ofDrawTriangle(170, 830, 230, 860, 200, 980); //Main part of south america
	ofSetColor(67, 205, 128, 100);
	ofDrawTriangle(180, 830, 165, 860, 210, 900);

	//Europe
	ofSetColor(255, 193, 193);
	ofDrawTriangle(250, 715, 300, 725, 275, 765);

	//Asian
	ofSetColor(238, 154, 73);   //main part
	ofDrawTriangle(300, 725, 450, 720, 375, 855);
	ofSetColor(244, 164, 96);
	ofDrawTriangle(300, 750, 430, 750, 400, 800);
	ofSetColor(205, 102, 0);
	ofDrawTriangle(290, 775, 360, 780, 370, 800);
	ofDrawTriangle(290, 775, 360, 830, 350, 870);

	//Africa
	ofSetColor(139, 69, 19);   //main part
	ofDrawTriangle(250, 795, 320, 805, 280, 970);
	ofSetColor(205, 133, 63);
	ofDrawTriangle(250, 795, 240, 835, 300, 910);

	//Australian
	ofSetColor(0, 206, 209);   //main part
	ofDrawTriangle(405, 930, 365, 960, 455, 965);
	ofSetColor(176, 226, 255);
	ofDrawTriangle(405, 930, 365, 960, 365, 945);
	ofSetColor(135, 206, 255);
	ofDrawTriangle(400, 930, 445, 955, 415, 985);

	//Antarctica
	ofSetColor(135, 206, 255);
	ofDrawTriangle(85, 1080, 455, 1080, 115, 975);
	//ofSetColor(0, 191, 255);
	ofDrawTriangle(300, 1080, 455, 1080, 315, 975);
	ofDrawTriangle(300, 1080, 455, 1080, 445, 985);
	ofDrawTriangle(100, 1080, 455, 1080, 215, 985);
	ofDrawTriangle(85, 1080, 200, 1080, 85, 995);
	/*ofSetColor(0, 0, 139);
	ofSetCircleResolution(100);
	ofDrawCircle(270,1100 - x,45);*/

	//ofDrawBezier(130,1000,150,900,140,1100,145,1300);
	//ofSetColor(255,106,106);
	//ofDrawRectangle(200, 1300 - x, 100, 50);

	//ofSetColor(0,238,118);
	//ofDrawEllipse(370,1200 - x,140,70);

	//ofSetColor(238, 44, 44);  //heart shape
	//ofDrawCircle(400, 1010 - x, 17.5);
	//ofDrawCircle(425, 1010 - x, 17.5);
	//ofDrawTriangle(385, 1020 - x, 440, 1020 - x, 412.5, 1050 - x);

	//introduction
	float y = 28.5;
	ofNoFill();
	ofSetColor(147, 112, 219);
	ofDrawRectangle(40, 1560, 1000, 260);

	ofFill();
	ofSetColor(0, 0, 0);//1
	ofDrawTriangle(60, 1570, 110, 1590, 60, 1780);
	ofDrawTriangle(110, 1590, 60, 1780, 110, 1810);

	ofSetColor(0, 0, 80);//2
	ofDrawTriangle(60 + y, 1570, 110 + y, 1590, 60 + y, 1780);
	ofDrawTriangle(110 + y, 1590, 60 + y, 1780, 110 + y, 1810);

	ofSetColor(0, 30, 100);//3
	ofDrawTriangle(60 + 2 * y, 1570, 110 + 2 * y, 1590, 60 + 2 * y, 1780);
	ofDrawTriangle(110 + 2 * y, 1590, 60 + 2 * y, 1780, 110 + 2 * y, 1810);

	ofSetColor(0, 50, 102);//4
	ofDrawTriangle(60 + 3 * y, 1570, 110 + 3 * y, 1590, 60 + 3 * y, 1780);
	ofDrawTriangle(110 + 3 * y, 1590, 60 + 3 * y, 1780, 110 + 3 * y, 1810);

	ofSetColor(19, 108, 160);//5
	ofDrawTriangle(60 + 4 * y, 1570, 110 + 4 * y, 1590, 60 + 4 * y, 1780);
	ofDrawTriangle(110 + 4 * y, 1590, 60 + 4 * y, 1780, 110 + 4 * y, 1810);

	ofSetColor(24, 140, 205);//6
	ofDrawTriangle(60 + 5 * y, 1570, 110 + 5 * y, 1590, 60 + 5 * y, 1780);
	ofDrawTriangle(110 + 5 * y, 1590, 60 + 5 * y, 1780, 110 + 5 * y, 1810);

	ofSetColor(135, 206, 250);//7
	ofDrawTriangle(60 + 6 * y, 1570, 110 + 6 * y, 1590, 60 + 6 * y, 1780);
	ofDrawTriangle(110 + 6 * y, 1590, 60 + 6 * y, 1780, 110 + 6 * y, 1810);

	ofSetColor(176, 223, 255);//8
	ofDrawTriangle(60 + 7 * y, 1570, 110 + 7 * y, 1590, 60 + 7 * y, 1780);
	ofDrawTriangle(110 + 7 * y, 1590, 60 + 7 * y, 1780, 110 + 7 * y, 1810);

	ofSetColor(0, 97, 71);//9
	ofDrawTriangle(60 + 8 * y, 1570, 110 + 8 * y, 1590, 60 + 8 * y, 1780);
	ofDrawTriangle(110 + 8 * y, 1590, 60 + 8 * y, 1780, 110 + 8 * y, 1810);

	ofSetColor(16, 122, 47);//10
	ofDrawTriangle(60 + 9 * y, 1570, 110 + 9 * y, 1590, 60 + 9 * y, 1780);
	ofDrawTriangle(110 + 9 * y, 1590, 60 + 9 * y, 1780, 110 + 9 * y, 1810);

	ofSetColor(0, 255, 0);//11
	ofDrawTriangle(60 + 10 * y, 1570, 110 + 10 * y, 1590, 60 + 10 * y, 1780);
	ofDrawTriangle(110 + 10 * y, 1590, 60 + 10 * y, 1780, 110 + 10 * y, 1810);

	ofSetColor(232, 215, 125);//12
	ofDrawTriangle(60 + 11 * y, 1570, 110 + 11 * y, 1590, 60 + 11 * y, 1780);
	ofDrawTriangle(110 + 11 * y, 1590, 60 + 11 * y, 1780, 110 + 11 * y, 1810);

	ofSetColor(161, 67, 0);//13
	ofDrawTriangle(60 + 12 * y, 1570, 110 + 12 * y, 1590, 60 + 12 * y, 1780);
	ofDrawTriangle(110 + 12 * y, 1590, 60 + 12 * y, 1780, 110 + 12 * y, 1810);

	ofSetColor(130, 30, 30);//14
	ofDrawTriangle(60 + 13 * y, 1570, 110 + 13 * y, 1590, 60 + 13 * y, 1780);
	ofDrawTriangle(110 + 13 * y, 1590, 60 + 13 * y, 1780, 110 + 13 * y, 1810);

	ofSetColor(161, 161, 161);//15
	ofDrawTriangle(60 + 14 * y, 1570, 110 + 14 * y, 1590, 60 + 14 * y, 1780);
	ofDrawTriangle(110 + 14 * y, 1590, 60 + 14 * y, 1780, 110 + 14 * y, 1810);

	ofSetColor(206, 206, 206);//16
	ofDrawTriangle(60 + 15 * y, 1570, 110 + 15 * y, 1590, 60 + 15 * y, 1780);
	ofDrawTriangle(110 + 15 * y, 1590, 60 + 15 * y, 1780, 110 + 15 * y, 1810);

	ofSetColor(250, 250, 250);//17
	ofDrawTriangle(60 + 16 * y, 1570, 110 + 16 * y, 1590, 60 + 16 * y, 1780);
	ofDrawTriangle(110 + 16 * y, 1590, 60 + 16 * y, 1780, 110 + 16 * y, 1810);

	ofSetColor(230, 230, 230);//18
	ofDrawTriangle(60 + 17 * y, 1570, 110 + 17 * y, 1590, 60 + 17 * y, 1780);
	ofDrawTriangle(110 + 17 * y, 1590, 60 + 17 * y, 1780, 110 + 17 * y, 1810);

	ofSetColor(225, 225, 225);//19
	ofDrawTriangle(60 + 18 * y, 1570, 110 + 18 * y, 1590, 60 + 18 * y, 1780);
	ofDrawTriangle(110 + 18 * y, 1590, 60 + 18 * y, 1780, 110 + 18 * y, 1810);

	ofSetColor(220, 220, 220);//20
	ofDrawTriangle(60 + 19 * y, 1570, 110 + 19 * y, 1590, 60 + 19 * y, 1780);
	ofDrawTriangle(110 + 19 * y, 1590, 60 + 19 * y, 1780, 110 + 19 * y, 1810);

	ofSetColor(200, 200, 200);//21
	ofDrawTriangle(60 + 20 * y, 1570, 110 + 20 * y, 1590, 60 + 20 * y, 1780);
	ofDrawTriangle(110 + 20 * y, 1590, 60 + 20 * y, 1780, 110 + 20 * y, 1810);

	ofSetColor(190, 190, 190);//22
	ofDrawTriangle(60 + 21 * y, 1570, 110 + 21 * y, 1590, 60 + 21 * y, 1780);
	ofDrawTriangle(110 + 21 * y, 1590, 60 + 21 * y, 1780, 110 + 21 * y, 1810);

	ofSetColor(180, 180, 180);//23
	ofDrawTriangle(60 + 22 * y, 1570, 110 + 22 * y, 1590, 60 + 22 * y, 1780);
	ofDrawTriangle(110 + 22 * y, 1590, 60 + 22 * y, 1780, 110 + 22 * y, 1810);

	ofSetColor(165, 165, 165);//24
	ofDrawTriangle(60 + 23 * y, 1570, 110 + 23 * y, 1590, 60 + 23 * y, 1780);
	ofDrawTriangle(110 + 23 * y, 1590, 60 + 23 * y, 1780, 110 + 23 * y, 1810);

	ofSetColor(140, 140, 140);//25
	ofDrawTriangle(60 + 24 * y, 1570, 110 + 24 * y, 1590, 60 + 24 * y, 1780);
	ofDrawTriangle(110 + 24 * y, 1590, 60 + 24 * y, 1780, 110 + 24 * y, 1810);

	ofSetColor(125, 125, 125);//26
	ofDrawTriangle(60 + 25 * y, 1570, 110 + 25 * y, 1590, 60 + 25 * y, 1780);
	ofDrawTriangle(110 + 25 * y, 1590, 60 + 25 * y, 1780, 110 + 25 * y, 1810);

	ofSetColor(105, 105, 105);//27
	ofDrawTriangle(60 + 26 * y, 1570, 110 + 26 * y, 1590, 60 + 26 * y, 1780);
	ofDrawTriangle(110 + 26 * y, 1590, 60 + 26 * y, 1780, 110 + 26 * y, 1810);

	ofSetColor(95, 95, 95);//28
	ofDrawTriangle(60 + 27 * y, 1570, 110 + 27 * y, 1590, 60 + 27 * y, 1780);
	ofDrawTriangle(110 + 27 * y, 1590, 60 + 27 * y, 1780, 110 + 27 * y, 1810);

	ofSetColor(70, 70, 70);//29
	ofDrawTriangle(60 + 28 * y, 1570, 110 + 28 * y, 1590, 60 + 28 * y, 1780);
	ofDrawTriangle(110 + 28 * y, 1590, 60 + 28 * y, 1780, 110 + 28 * y, 1810);

	ofSetColor(45, 45, 45);//30
	ofDrawTriangle(60 + 29 * y, 1570, 110 + 29 * y, 1590, 60 + 29 * y, 1780);
	ofDrawTriangle(110 + 29 * y, 1590, 60 + 29 * y, 1780, 110 + 29 * y, 1810);

	ofSetColor(30, 30, 30);//31
	ofDrawTriangle(60 + 30 * y, 1570, 110 + 30 * y, 1590, 60 + 30 * y, 1780);
	ofDrawTriangle(110 + 30 * y, 1590, 60 + 30 * y, 1780, 110 + 30 * y, 1810);

	ofSetColor(15, 15, 15);//32
	ofDrawTriangle(60 + 31 * y, 1570, 110 + 31 * y, 1590, 60 + 31 * y, 1780);
	ofDrawTriangle(110 + 31 * y, 1590, 60 + 31 * y, 1780, 110 + 31 * y, 1810);

	ofSetColor(0, 0, 0);//33
	ofDrawTriangle(60 + 32 * y, 1570, 110 + 32 * y, 1590, 60 + 32 * y, 1780);
	ofDrawTriangle(110 + 32 * y, 1590, 60 + 32 * y, 1780, 110 + 32 * y, 1810);

	//leaderboard
	//int c = 660;
	//ofsetcolor(135,206,235);
	//ofdrawtriangle(950, 650+c, 880.72, 770 + c, 1019.28, 770 + c);
	//ofdrawtriangle(880.72, 690 + c, 1019.28, 690 + c, 950, 810 + c);

	//int q = 620;
	//int w = 620;
	//ofsetcolor(106,90,205);
	//ofsetpolymode(of_poly_winding_nonzero);
	//ofbeginshape();
	//ofvertex(400 + q,135 + w);
	//ofvertex(215 + q, 135 + w);
	//ofvertex(365 + q, 25 + w);
	//ofvertex(305 + q, 200 + w);
	//ofvertex(250 + q, 25 + w);
	//   //ofvertex(305,700);
	//ofendshape();

	//the part is for the shape game "c"
	//ofPushMatrix();
	//ofTranslate(360, 1020, 0);
	//ofEnableDepthTest();
	//ofSetColor(255);
	//ofDrawSphere(0, 0, 50, 30);
	//ofSetColor(255, 215, 0);
	//ofDrawSphere(25, 0, 25, 50);
	//ofDisableDepthTest();
	//ofPopMatrix();

	int e = 400;
	int r = 680;
	ofSetColor(255, 250, 205);
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + e, 135 + r);
	ofVertex(215 + e, 135 + r);
	ofVertex(365 + e, 25 + r);
	ofVertex(305 + e, 200 + r);
	ofVertex(250 + e, 25 + r);
	ofVertex(305 + e, 700 + r);
	ofEndShape();

	ofEnableAlphaBlending();    // turn on alpha blending
	ofSetColor(238, 233, 191, 165);    // red, 50% transparent
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + e + 100, 135 + r + 100);
	ofVertex(215 + e + 100, 135 + r + 100);
	ofVertex(365 + e + 100, 25 + r + 100);
	ofVertex(305 + e + 100, 200 + r + 100);
	ofVertex(250 + e + 100, 25 + r + 100);
	ofVertex(305 + e + 100, 700 + r + 100);
	ofEndShape();

	ofDisableAlphaBlending();   // turn off alpha
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + e + 50, 135 + r + 50);
	ofVertex(215 + e + 50, 135 + r + 50);
	ofVertex(365 + e + 50, 25 + r + 50);
	ofVertex(305 + e + 50, 200 + r + 50);
	ofVertex(250 + e + 50, 25 + r + 50);
	ofVertex(305 + e + 50, 700 + r + 50);
	ofEndShape();



	ofSetColor(255, 255, 255);



}

void ofApp::drawMapWindow(ofEventArgs& args) {
	ofSetBackgroundColor(255, 255, 255);
	ofImage ImgMap("ShapeGameData/GBMap.png");
	ImgMap.draw(0, 0);
}

void ofApp::drawLeaderWindow(ofEventArgs &args){

	ifstream fin("c:/data/LeaderBoard.txt");
	string shapeLeaderBoard[5][3];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			fin >> shapeLeaderBoard[i][j];
		}
	}
	ofTrueTypeFont text;
	text.load("ofxbraitsch/fonts/Verdana.ttf", 20);


	ofSetBackgroundColor(255, 255, 255);
	ofSetColor(200,0,0);
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			stringstream ttext;
			string BoardText;
			ttext << (j+1)<<". "<<shapeLeaderBoard[i][j]<<" points";
			BoardText = ttext.str();
			text.drawString(BoardText, 50, 100+25*((i*5)+j));
		}
		stringstream ttext;
		string BoardText;
		
	}
	ofSetColor(0, 0, 180);
	text.drawString("Circle: ", 50, 100 + 25 * ((0 * 5-1)));
	text.drawString("Square: ", 50, 100 + 25 * ((1 * 5-1)));
	text.drawString("Triangle: ", 50, 100 + 25 * ((2 * 5-1)));
	text.drawString("Heart shape:", 50, 100 + 25 * ((3 * 5-1)));
	text.drawString("GB map: ", 50, 100 + 25 * ((4 * 5-1)));

}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
	if (e.target->is(" ")) {//1 Lunar Theme
		cout << "Change to Lunar Theme" << endl;
		changeTheme = true;
		currentTheme = LUNAR_THEME;
		ofxDatGuiThemeCharcoal guiTheme;

		

	}
	else if (e.target->is("  ")) {//2 Sandbox Theme
		cout << "Change to Sandbox Theme" << endl;
		changeTheme = true;
		currentTheme = SANDBOX_THEME;
	}
	else if (e.target->is("   ")) {//3 Sandbox_flood
		if (currentTheme!=SANDBOX_THEME) {
			changeTheme = true;
			currentTheme = SANDBOX_THEME;
		}
		cout << "Flood" << endl;
		floodTimer = ofGetElapsedTimef();
		sandboxFlood = true;
		
	}
	else if (e.target->is("    ")) {//4 CircleShape_Starter
		cout << "Shape a heart" << endl;
		shapeGameManager.chooseShape(CIRCLE);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();

	}
	else if (e.target->is("     ")) {//5 Square_Starter
		shapeGameManager.chooseShape(SQUARE);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();

	}
	else if (e.target->is("      ")) {//6 Triangle_Starter
		shapeGameManager.chooseShape(TRIANGLE);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();

	}
	else if (e.target->is("       ")) {//7 Heart_Starter
		shapeGameManager.chooseShape(HEART);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();

	}
	else if (e.target->is("        ")) {//8 GB_Starter
		shapeGameManager.chooseShape(GB);
		shapeGameRunning = true ;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();
		ofGLFWWindowSettings settings;
		settings.setPosition(ofVec2f(0, 0));
		settings.resizable = true;
		settings.decorated = true;
		settings.title = "Great Britain Map";
		shared_ptr<ofAppBaseWindow> GBMapWindow = ofCreateWindow(settings);
		GBMapWindow->setWindowPosition(50, 50);
		GBMapWindow->setWindowShape(500,787);
		shared_ptr<ofApp> mainApp(new ofApp);
		ofAddListener(GBMapWindow->events().draw, mainApp.get(), &ofApp::drawMapWindow);
		mainApp->GBMapWindow = GBMapWindow;

	}
	else if (e.target->is("         ")) {//9 leader board
		ofGLFWWindowSettings settings;
		settings.setPosition(ofVec2f(0, 0));
		settings.resizable = true;
		settings.decorated = true;
		settings.title = "Leader Board";
		shared_ptr<ofAppBaseWindow> leaderWindow = ofCreateWindow(settings);
		leaderWindow->setWindowPosition(50, 50);
		leaderWindow->setWindowShape(400, 700);
		shared_ptr<ofApp> mainApp(new ofApp);
		ofAddListener(leaderWindow->events().draw, mainApp.get(), &ofApp::drawLeaderWindow);
		mainApp->leadWindow = leaderWindow;

	}

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ')
	{
		kinectProjector->startApplication();
		sandSurfaceRenderer->drawColorLine(false);
		//shapeGameManager.GetActualBinaryLandImage();
		//ConnectedComponentAnalysis();
		//matchMap();
		//ofGLFWWindowSettings settings;
		//settings.width = 1600; // Default settings
		//settings.height = 800;
		//settings.setPosition(ofVec2f(0, 0));
		//settings.resizable = true;
		//settings.decorated = true;
		//settings.title = "Magic-Sand ";
		//shared_ptr<ofApp> mainApp(new ofApp);
		//shared_ptr<ofAppBaseWindow> thirdWindow = ofCreateWindow(settings);
		//thirdWindow->setWindowShape(1080, 1920);

		//ofAddListener(thirdWindow->events().draw, mainApp.get(), &ofApp::drawPcjWindow);
		//mainApp->pcjWindow = thirdWindow;


		//Lunar_Starter = new ofxDatGui(130, 550);
		//Lunar_Starter->addButton(" ");

		//Sandbox_Starter = new ofxDatGui(670, 550);
		//Sandbox_Starter->addButton("  ");

		//ShapeGame_Starter = new ofxDatGui(130, 1300);
		//ShapeGame_Starter->addButton("   ");
		//Lunar_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		//Sandbox_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		//ShapeGame_Starter->onButtonEvent(this, &ofApp::onButtonEvent);

	}
	else if (key == 'a') {
		
		ofGLFWWindowSettings settings;
		settings.width = 1600; // Default settings
		settings.height = 800;
		settings.setPosition(ofVec2f(0, 0));
		settings.resizable = true;
		settings.decorated = true;
		settings.title = "Sandbox-UI";
		shared_ptr<ofAppBaseWindow> forthWindow = ofCreateWindow(settings);
		forthWindow->setWindowShape(1080, 1920);
		shared_ptr<ofApp> mainApp(new ofApp);
		ofAddListener(forthWindow->events().draw, mainApp.get(), &ofApp::drawSUIWindow);
		mainApp->SUIWindow = forthWindow;
		/*
		int z = 310;
		Lunar_Starter = new ofxDatGui(130, 670 - z);
		Lunar_Starter->addButton(" ");//1

		Sandbox_Starter = new ofxDatGui(670, 670 - z);
		Sandbox_Starter->addButton("  ");//2

		Sandbox_flood = new ofxDatGui(670, 730 - z);
		Sandbox_flood->addButton("   ");//3

		Circle_Starter = new ofxDatGui(130, 1530 - z);
		Circle_Starter->addButton("    ");//4

		Square_Starter = new ofxDatGui(130, 1580 - z);
		Square_Starter->addButton("     ");//5

		Triangle_Starter = new ofxDatGui(130, 1480 - z);
		Triangle_Starter->addButton("      ");//6

		heartShape_Starter = new ofxDatGui(130, 1630 - z);
		heartShape_Starter->addButton("       ");//7

		ShapeUK_Starter = new ofxDatGui(130, 1430 - z);//map
		ShapeUK_Starter->addButton("        ");//8

		Leardboard_Starter = new ofxDatGui(670, 1480 - z);
		Leardboard_Starter->addButton("         "); //9 the learderboard


		Lunar_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Sandbox_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		heartShape_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Sandbox_flood->onButtonEvent(this, &ofApp::onButtonEvent);
		Triangle_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Square_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Circle_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Leardboard_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		ShapeUK_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		*/
	}

	else if (key == '1') {
		ofGLFWWindowSettings settings;
		settings.width = 1600; // Default settings
		settings.height = 800;
		settings.setPosition(ofVec2f(0, 0));
		settings.resizable = true;
		settings.decorated = true;
		settings.title = "Sandbox-UI";
		shared_ptr<ofAppBaseWindow> thirdWindow = ofCreateWindow(settings);
		thirdWindow->setWindowShape(1080, 1920);
		shared_ptr<ofApp> mainApp(new ofApp);
		ofAddListener(thirdWindow->events().draw, mainApp.get(), &ofApp::drawPcjWindow);
		mainApp->pcjWindow = thirdWindow;

		int z = 310;
		Lunar_Starter = new ofxDatGui(115, 700-z);
		Lunar_Starter->addButton(" ");//1

		Sandbox_Starter = new ofxDatGui(670, 670 - z);
		Sandbox_Starter->addButton("  ");//2

		Sandbox_flood = new ofxDatGui(670, 730 - z);
		Sandbox_flood->addButton("   ");//3

		Circle_Starter = new ofxDatGui(130, 1530 - z);
		Circle_Starter->addButton("    ");//4

		Square_Starter = new ofxDatGui(130, 1580 - z);
		Square_Starter->addButton("     ");//5

		Triangle_Starter = new ofxDatGui(130, 1480 - z);
		Triangle_Starter->addButton("      ");//6

		heartShape_Starter = new ofxDatGui(130, 1630 - z);
		heartShape_Starter->addButton("       ");//7
	
		ShapeUK_Starter = new ofxDatGui(130, 1430 - z);//map
		ShapeUK_Starter->addButton("        ");//8

		Leardboard_Starter = new ofxDatGui(670, 1480 - z);
		Leardboard_Starter->addButton("         "); //9 the learderboard


		Lunar_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Sandbox_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		heartShape_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Sandbox_flood->onButtonEvent(this, &ofApp::onButtonEvent);
		Triangle_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Square_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Circle_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		Leardboard_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
		ShapeUK_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	}
	else if (key == '2') {//Lunar
		cout << "Change to Lunar Theme" << endl;
		changeTheme = true;
		currentTheme = LUNAR_THEME;
		ofxDatGuiThemeCharcoal guiTheme;
	}
	else if (key == '3') {//Sandbox
		cout << "Change to Sandbox Theme" << endl;
		changeTheme = true;
		currentTheme = SANDBOX_THEME;
	}
	else if (key == 's') {//Stop similarity
		//cout << "R" << endl;
		shapeGameRunning = false;
		//elapsedTime = ofGetElapsedTimef();
		//similarityCheckTimer = ofGetElapsedTimef();
		//cout<<"Shape game: "<<shapeGameRunning<<endl;
		//shapeGameManager.GetActualBinaryLandImage();
		//ofxCvGrayscaleImage BinaryLandImage;
		//std::string BinOutName = "BinaryLandImage.png";
		//kinectProjector->getBinaryLandImage(BinaryLandImage);
		//if (!kinectProjector->getBinaryLandImage(BinaryLandImage)) {
			//cout<<"*!*!Wrong!"<<endl;
		//}
		//ofSaveImage(BinaryLandImage.getPixels(), BinOutName);
	}
	else if (key == '4') {//circle
		shapeGameManager.chooseShape(CIRCLE);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();
	}
	else if (key == '5') {//square
		shapeGameManager.chooseShape(SQUARE);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();
	}
	else if (key == '6') {//Triangle
		shapeGameManager.chooseShape(TRIANGLE);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();
	}
	else if (key == '7') {//heart
		shapeGameManager.chooseShape(HEART);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();
	}
	else if (key == '8') {//GB
		shapeGameManager.chooseShape(GB);
		shapeGameRunning = true;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();
		ofGLFWWindowSettings settings;
		settings.setPosition(ofVec2f(0, 0));
		settings.resizable = true;
		settings.decorated = true;
		settings.title = "Great Britain Map";
		shared_ptr<ofAppBaseWindow> GBMapWindow = ofCreateWindow(settings);
		GBMapWindow->setWindowPosition(50, 50);
		GBMapWindow->setWindowShape(500,787);
		shared_ptr<ofApp> mainApp(new ofApp);
		ofAddListener(GBMapWindow->events().draw, mainApp.get(), &ofApp::drawMapWindow);
		mainApp->GBMapWindow = GBMapWindow;
	}
	else if (key == 'f') {//flood
	//	shapeGameManager.GetActualBinaryLandImage();
	//	ofxCvGrayscaleImage BinaryLandImage;
	//	std::string BinOutName = "BinaryLandImage.png";
	//	//kinectProjector->getBinaryLandImage(BinaryLandImage);
	//	if (!kinectProjector->getBinaryLandImage(BinaryLandImage)) {
	//		ofSaveImage(BinaryLandImage.getPixels(), BinOutName);
		//sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMapL.xml");
		if (currentTheme != SANDBOX_THEME) {
			changeTheme = true;
			currentTheme = SANDBOX_THEME;
		}
		cout << "Flood" << endl;
		floodTimer = ofGetElapsedTimef();
		sandboxFlood = true;


		}
	else if (key == 'g') {
		sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMap.xml");

	}
	else if (key == 'p') {
		floodSpeed += 0.005;
		cout << floodSpeed << endl;
	}
	else if (key == 'm') {
		floodSpeed -= 0.004;
		cout << floodSpeed << endl;
	}
	else if (key == 'l') {//Leader board
		ofGLFWWindowSettings settings;
		settings.setPosition(ofVec2f(0, 0));
		settings.resizable = true;
		settings.decorated = true;
		settings.title = "Leader Board";
		shared_ptr<ofAppBaseWindow> leaderWindow = ofCreateWindow(settings);
		leaderWindow->setWindowPosition(50, 50);
		leaderWindow->setWindowShape(400, 700);
		shared_ptr<ofApp> mainApp(new ofApp);
		ofAddListener(leaderWindow->events().draw, mainApp.get(), &ofApp::drawLeaderWindow);
		mainApp->leadWindow = leaderWindow;
		
	}
	else if (key == 'o') {//line
		sandSurfaceRenderer->drawColorLine(true);
	}
	else if (key == 'c') {
		sandSurfaceRenderer->drawColorLine(false);
	}

}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	kinectProjector->mouseDragged(x - mainWindowROI.x, y - mainWindowROI.y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (mainWindowROI.inside((float)x, (float)y))
	{
		kinectProjector->mousePressed(x - mainWindowROI.x, y - mainWindowROI.y, button);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	kinectProjector->mouseReleased(x - mainWindowROI.x, y - mainWindowROI.y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::getBinaryImage() {
	std::string BinOutName = "BinaryLandImage.png";
	kinectProjector->getBinaryLandImage(BinaryLandImage);
	ofSaveImage(BinaryLandImage.getPixels(), BinOutName);
}