#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//test
	///////////////
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
	Lunar_Starter = new ofxDatGui(115, 700-z); //Button for lunar
	Lunar_Starter->addButton(" ");//1

	Sandbox_Starter = new ofxDatGui(670, 670 - z);
	Sandbox_Starter->addButton("  ");//2

	Sandbox_flood = new ofxDatGui(670, 730 - z);
	Sandbox_flood->addButton("   ");//3

    Triangle_Starter = new ofxDatGui(130, 1480 - z);
	Triangle_Starter->addButton("     ");//4

    Circle_Starter = new ofxDatGui(130, 1530 - z);
	Circle_Starter->addButton("       ");//5

	Square_Starter = new ofxDatGui(130, 1580 - z);
	Square_Starter->addButton("      ");//6

	ShapeHeart_Starter = new ofxDatGui(130, 1630 - z);
	ShapeHeart_Starter->addButton("    ");//7 

	ShapeUK_Starter = new ofxDatGui(130, 1430 - z);//map
	ShapeUK_Starter->addButton("    ");//7 

	Leardboard_Starter = new ofxDatGui(670, 1480 - z);
	Leardboard_Starter->addButton("   "); //9 the learderboard


	Lunar_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	Sandbox_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	ShapeHeart_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	Sandbox_flood->onButtonEvent(this, &ofApp::onButtonEvent);
	Triangle_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	Square_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	Circle_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	Leardboard_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	ShapeUK_Starter->onButtonEvent(this, &ofApp::onButtonEvent);
	//ofImage temp;
	//cv::Mat img;
	//std::string fname = "mapGame/ReferenceData/testMap.png";
	//if (!temp.loadImage(fname))
	//{
		//ofLogVerbose("CMapGameController") << "SafeReadBinaryImage(): could not read " + fname;
	//}
	//cv::Mat t1 = ofxCv::toCv(temp.getPixels());

	//cv::Mat t2;
	//if (t1.channels() > 1)
		//cv::cvtColor(t1, t2, cv::COLOR_BGR2GRAY);
	//else
		//t1.copyTo(t2);

	//cv::normalize(t2, img, 0, 255, cv::NORM_MINMAX);

	//IplImage im = img;
	//IplImage im = IplImage(img);

	//IplImage* im = cvCreateImage(cvGetSize(mat), 8, 1);
	//cvGetImage(img, im);

	//cvSaveImage("rice1.bmp", im);
	//cvSave("my_matrix.xml", &img);
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




	

}

//--------------------------------------------------------------
void ofApp::update(){
	// Call kinectProjector->update() first during the update function()
	//Theme
	if (changeTheme) {
		changeTheme = false;
		if (currentTheme == SANDBOX_THEME) {
			sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMap.xml");
		}else if (currentTheme == LUNAR_THEME) {
			sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMapL.xml");
		}
	}

	//Flood
	if (sandboxFlood) {
		cout << height << endl;
		
		if (height<0) {
			height += 0.005;
			sandSurfaceRenderer->setHeightKey(4, -125 + height);
			sandSurfaceRenderer->setHeightKey(5, -7.5 + height);
			sandSurfaceRenderer->setHeightKey(6, -2.5+ height);
			sandSurfaceRenderer->setHeightKey(7, height);
			sandSurfaceRenderer->setHeightKey(8, 0+height/2);
		}
		else if (height>0&& height<2.5) {
			height += 0.01;
			sandSurfaceRenderer->heightMap.setColorKey(8, ofColor(135, 206, 250));
			sandSurfaceRenderer->heightMap.setColorKey(7, ofColor(135,206,250));
			sandSurfaceRenderer->setHeightKey(4, -125 + height);
			sandSurfaceRenderer->setHeightKey(5, -7.5 + height);
			sandSurfaceRenderer->setHeightKey(6, -2.5 + height);
			sandSurfaceRenderer->setHeightKey(8, height);
			sandSurfaceRenderer->setHeightKey(9, 2.5 + height / 2);
		}
		else if (height>2.5 && height<25) {
			height += 0.05;
			sandSurfaceRenderer->heightMap.setColorKey(9, ofColor(135, 206, 250));
			sandSurfaceRenderer->heightMap.setColorKey(8, ofColor(135, 206, 250));
			sandSurfaceRenderer->setHeightKey(4, -125 + height);
			sandSurfaceRenderer->setHeightKey(5, -7.5 + height);
			sandSurfaceRenderer->setHeightKey(6, -2.5 + height);
			sandSurfaceRenderer->setHeightKey(9, height);
		}
		else {

		}
		
		
	}
	kinectProjector->update();
	sandSurfaceRenderer->update();


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

		//if (ConnectedComponentAnalysis()) {
		//	if (matchMap()) {
		//		
		//	}
		//	else {
		//		shapeGame_Similarity = 0;
		//	}
		//}
		//else {
		//	shapeGame_Similarity = 0;
		//}

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
		if (ofGetElapsedTimef() - elapsedTime<5) {
			ofImage Img_Bac("sss.png");

			Img_Bac.draw(100, 100);
		}
		else {
			ofTrueTypeFont text;
			//ofSetColor(255, 0, 0);
			text.load("ofxbraitsch/fonts/Verdana.ttf", 70);
			stringstream ttext;
			string ScoreText;
			//int tscore = (int)shapeGame_Similarity;
			ttext << " Current Similarity: " << shapeGame_Similarity * 100 << "%";
			ScoreText = ttext.str();
			ofSetColor(255, 0, 0);
			text.drawString("Shape: Circle Island", 100, 100);
			text.drawString(ScoreText, 100, 230);
			if (shapeGame_Similarity>0.85) {
				ofSetColor(0, 100, 0);
				text.drawString("Circle Island found", 350, 400);
			}
			else {
				text.drawString("No match shape", 350, 400);
			}
			ofSetColor(255, 255, 255);
		}

	}
	
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
	ofDrawRectangle(40, 345-x, 460, 500);  //x,y
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
	ofSetColor(100, 149, 237);
	ofDrawEllipse(680, 650 - x, 130, 35);

	ofSetColor(0,191,255);
	ofDrawEllipse(720,635 - x,150,40);

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
	ofDrawLine(1002+v, 418, 1027 + v, 443);
	ofDrawLine(998 + v, 422, 1023 + v, 447);
	ofDrawLine(998 + v, 418, 973 + v, 443);
	ofDrawLine(1002 + v, 422, 977 + v, 447);
	ofSetColor(135,135,135);
	ofDrawCircle(1000 + v,420,15);
	ofSetColor(159,182,205);
	ofDrawCircle(975 + v,445,10);
	ofDrawCircle(1025 + v, 445, 10);
	
	//ofDrawSphere(12.5, 0, 12.5, 25);
	//ofDrawPlane(600,100,100,1000);//no angle
	//ofDrawBox(100,100,50,50,100,50);
	//ofDrawLine();
	//ofDrawTriangle(940,625,1000,625,910,660);
	//ofDrawTriangle(1000,625,910,660,970,660);

	//shape game
	ofSetColor(99,184,255); //Triangle
	ofDrawTriangle(430,1470 - x,450,1510 - x,410,1510 - x);
	//ofSetColor();
	ofDrawTriangle(440, 1000, 400, 900, 440, 700);

	ofSetColor(193,255,193);  //Circle
	ofDrawCircle(430,1545 - x,20);

	ofSetColor(79,79,79);   //Rectangle
	ofDrawRectangle(410,1580 - x,30,30);

	ofSetColor(255,174,184);  //heart shape
	ofDrawCircle(430, 1650 - x, 17.5);
	ofDrawCircle(455,1650 - x,17.5);
	ofDrawTriangle(415,1660 - x,470,1660 - x,442.5,1690 - x);
	


	ofSetColor(0,255,255);
	ofSetLineWidth(3);
	ofDrawLine(410, 1430 - x, 410, 1455 - x);   //u
	ofDrawLine(410, 1455 - x, 425, 1455 - x);
	ofDrawLine(425, 1457 - x, 425, 1430 - x);

	ofDrawLine(440, 1430 - x, 440, 1455 - x);//k
	ofDrawLine(440, 1440 - x, 455, 1430 - x);
	ofDrawLine(440, 1440 - x, 455, 1455 - x);

	//ofDrawLine(140,1410)
	ofNoFill();
	ofSetColor(191,239,255);
	ofSetLineWidth(5);
	ofDrawRectangle(90, 980 - x, 370, 420);
	ofFill();
	ofSetColor(0, 191, 255);
	ofDrawTriangle(150, 1000 - x, 120, 1300 - x, 230, 1200 - x);
	ofSetColor(0, 0, 139);
	ofSetCircleResolution(100);
	ofDrawCircle(270,1100 - x,45);
	//ofDrawBezier(130,1000,150,900,140,1100,145,1300);
	ofSetColor(255,106,106);
	ofDrawRectangle(200, 1300 - x, 100, 50);

	ofSetColor(0,238,118);
	ofDrawEllipse(370,1200 - x,140,70);

	ofSetColor(238, 44, 44);  //heart shape
	ofDrawCircle(400, 1010 - x, 17.5);
	ofDrawCircle(425, 1010 - x, 17.5);
	ofDrawTriangle(385, 1020 - x, 440, 1020 - x, 412.5, 1050 - x);

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
	ofDrawTriangle(60+y, 1570, 110 + y, 1590, 60 + y, 1780);
	ofDrawTriangle(110+y, 1590, 60 + y, 1780, 110 + y, 1810);

	ofSetColor(0, 30, 100);//3
	ofDrawTriangle(60 + 2*y, 1570, 110 + 2 * y, 1590, 60 + 2 * y, 1780);
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
	int c = 660;
	ofSetColor(135,206,235);
	ofDrawTriangle(950, 650+c, 880.72, 770 + c, 1019.28, 770 + c);
	ofDrawTriangle(880.72, 690 + c, 1019.28, 690 + c, 950, 810 + c);

	int q = 620;
	int w = 620;
	ofSetColor(106,90,205);
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + q,135 + w);
	ofVertex(215 + q, 135 + w);
	ofVertex(365 + q, 25 + w);
	ofVertex(305 + q, 200 + w);
	ofVertex(250 + q, 25 + w);
    //ofVertex(305,700);
	ofEndShape();

	//the part is for the shape game "c"
	ofPushMatrix();
	ofTranslate(360, 1020, 0);
	ofEnableDepthTest();
	ofSetColor(255);
	ofDrawSphere(0, 0, 50, 30);
	ofSetColor(255, 215, 0);
	ofDrawSphere(25, 0, 25, 50);
	ofDisableDepthTest();
	ofPopMatrix();

	int e=400;
	int r=680;
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
	ofSetColor(238, 233, 191,165);    // red, 50% transparent
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + e+100, 135 + r + 100);
	ofVertex(215 + e + 100, 135 + r + 100);
	ofVertex(365 + e + 100, 25 + r + 100);
	ofVertex(305 + e + 100, 200 + r + 100);
	ofVertex(250 + e + 100, 25 + r + 100);
	ofVertex(305 + e + 100, 700 + r + 100);
	ofEndShape();
	
	ofDisableAlphaBlending();   // turn off alpha
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400 + e+50, 135 + r + 50);
	ofVertex(215 + e + 50, 135 + r + 50);
	ofVertex(365 + e + 50, 25 + r + 50);
	ofVertex(305 + e + 50, 200 + r + 50);
	ofVertex(250 + e + 50, 25 + r + 50);
	ofVertex(305 + e + 50, 700 + r + 50);
	ofEndShape();


}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
	if (e.target->is(" ")) {//1 Lunar Theme
		//myAlert.setTitle("Please input your name");
		//myAlert.setMessage("It's time to go outside.");
		//myAlert.show();
		cout << "Change to Lunar Theme" << endl;
		//sandSurfaceRenderer->colorMapFile = "HeightColorMapL.xml";
		//sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMapL.xml");
		
		changeTheme = true;
		currentTheme = LUNAR_THEME;
		//Sandbox_flood->setVisible(false);
		//ShapeGame_Starter->setVisible(false);
		//Triangle_Starter->setVisible(false);
		//Square_Starter->setVisible(false);
		//Circle_Starter->setVisible(false);
		ofxDatGuiThemeCharcoal guiTheme;

		

	}
	else if (e.target->is("  ")) {//2 Sandbox Theme
		cout << "Change to Sandbox Theme" << endl;
		//sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMap.xml");
		changeTheme = true;
		currentTheme = SANDBOX_THEME;
	}
	else if (e.target->is("   ")) {//3 Sandbox_flood
		changeTheme = true;
		currentTheme = SANDBOX_THEME;
		cout << "Flood" << endl;
		sandboxFlood = !sandboxFlood;
		
	}
	else if (e.target->is("    ")) {//4 ShapeGame_Starter
		cout << "Shape an Island Game" << endl;
		shapeGameRunning = !shapeGameRunning;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();

	}
	else if (e.target->is("     ")) {//5 Triangle_Starter


	}
	else if (e.target->is("     ")) {//6 Square_Starter


	}
	else if (e.target->is("     ")) {//7 Circle_Starter


	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ')
	{
		kinectProjector->startApplication();
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
	else if (key == '1') {
		
	}
	else if (key == 'r') {
		//cout << "R" << endl;
		shapeGameRunning = !shapeGameRunning;
		elapsedTime = ofGetElapsedTimef();
		similarityCheckTimer = ofGetElapsedTimef();
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
	else if (key == 'm') {
	//	shapeGameManager.GetActualBinaryLandImage();
	//	ofxCvGrayscaleImage BinaryLandImage;
	//	std::string BinOutName = "BinaryLandImage.png";
	//	//kinectProjector->getBinaryLandImage(BinaryLandImage);
	//	if (!kinectProjector->getBinaryLandImage(BinaryLandImage)) {
	//		ofSaveImage(BinaryLandImage.getPixels(), BinOutName);
		//sandSurfaceRenderer->heightMap.loadFile("colorMaps/HeightColorMapL.xml");
		}
	//	
	//}
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

//bool ofApp::ConnectedComponentAnalysis(){
//	std::string iThres = "BinaryLandImage.png";
//	std::string onameRoiPng = "bin/data/HeightMap_ROI.png";
//	//std::string oNoBord = "bin/data/HM_cutout_noBorderBlobs.png";
//	std::string oMaxBlob =  "HM_cutout_maxBlob.png";
//	cv::Mat M;
//	if (BinaryLandImage.width == 0)
//	{
//		if (!SafeReadBinaryImage(M, iThres))
//		{
//			ofLogVerbose("CMapGameController") << "ConnectedComponentAnalysis(): could not read BinaryLandImage.png";
//			return false;
//		}
//	}
//	else
//	{
//		M = ofxCv::toCv(BinaryLandImage);
//	}
//	//SafeReadBinaryImage(M, iThres);
//	//
//	//int xmin = 0;
//	//int ymin = 0;
//	//int xmax = 640;
//	//int ymax = 480;
//	//ROI = cv::Rect(xmin, ymin, xmax - xmin, ymax - ymin);
//	//
//	//cv::Mat bw = M(ROI);
//
//	RemoveBorderBlobs(M);
//	//cv::imwrite("output1.bmp", M);
//	std::vector<std::vector<cv::Point> > contours;
//	cv::findContours(M, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	if (contours.size() == 0)
//	{
//		//ofLogVerbose("CMapGameController") << "ConnectedComponentAnalysis(): could not find any contours";
//		cout<<"Please put hands off!"<<endl;
//		return false;
//	}
//	int MaxCont = 0;
//	int maxID = -1;
//
//	for (int i = 0; i < contours.size(); i++)
//	{
//		int sz = contours[i].size();
//		if (sz > MaxCont)
//		{
//			MaxCont = sz;
//			maxID = i;
//		}
//	}
//
//	maxBLOBImage = cv::Mat::zeros(M.size(), CV_8UC1);
//	drawContours(maxBLOBImage, contours, maxID, 255, CV_FILLED, 8);
//	//cv::imwrite(, maxBLOBImage);
//	cv::Mat t;
//	maxBLOBImage.copyTo(t);  // Do a temporary copy to avoid trouble with ROI definitions and data boundaries
//
//	unsigned char *input = (unsigned char*)(t.data);
//	ofxCvGrayscaleImage img2;
//	img2.setFromPixels(input, t.cols, t.rows);
//	ofSaveImage(img2.getPixels(), oMaxBlob);
//
//	return true;
//
//}
//
//
//bool ofApp::matchMap() {
//	std::string FoundMap = "HM_cutout_maxBlob.png";
//	std::string referenceMap = "sss_GT.png";
//	std::string referenceImage = "sss.png";
//	cv::Mat fMap;
//	cv::Mat refMap;
//	if (BinaryLandImage.width == 0)
//	{
//		if (!SafeReadBinaryImage(fMap, FoundMap))
//			return false;
//	}
//	else
//	{
//		fMap = maxBLOBImage;
//	}
//	if (!SafeReadBinaryImage(refMap, referenceMap))
//		return false;
//	
//	std::vector<cv::Point2f> AutoLMs;
//	ComputeLandmarksFromTemplate(refMap, AutoLMs);//ÇÐ¸î
//	//cout << refMap << endl;
//	//cv::imwrite("output1.bmp", fMap);
//	//cv::imwrite("output2.bmp", refMap);
//	double AfMap = cv::sum(fMap)[0] / 255.0;
//	double ArefMap = cv::sum(refMap)[0] / 255.0;
//	double factor = sqrt(AfMap / ArefMap);
//	cv::Mat ScaledI;
//
//	//·Å´ó
//	cv::resize(refMap, ScaledI, cv::Size(), factor, factor, cv::INTER_AREA);
//	//cv::imwrite("output1.bmp", ScaledI);
//
//	cv::Mat TMat = cv::Mat::zeros(3, 3, CV_32FC1);
//	TMat.at<float>(0, 0) = factor;
//	TMat.at<float>(1, 1) = factor;
//	TMat.at<float>(2, 2) = 1;
//	std::vector<cv::Point2f> LMsScaled(AutoLMs.size());
//	cv::perspectiveTransform(AutoLMs, LMsScaled, TMat);
//
//	cv::Mat Itrans = cv::Mat::zeros(ScaledI.rows, ScaledI.cols, ScaledI.type());
//	double rCMx = 0;
//	double rCMy = 0;
//	if (!ReferenceToCM(ScaledI, Itrans, rCMx, rCMy))
//		return false;
//	//cv::imwrite("output2.bmp", Itrans);
//
//	double tX = ScaledI.cols / 2 - rCMx;
//	double tY = ScaledI.rows / 2 - rCMy;
//	TMat.at<float>(0, 0) = 1;
//	TMat.at<float>(1, 1) = 1;
//	TMat.at<float>(0, 2) = tX;
//	TMat.at<float>(1, 2) = tY;
//	std::vector<cv::Point2f> LMsScaledTrans(AutoLMs.size());
//	cv::perspectiveTransform(LMsScaled, LMsScaledTrans, TMat);
//
//	double CMx = 0;
//	double CMy = 0;
//	ComputeCM(fMap, CMx, CMy);
//	double midX = Itrans.cols / 2;
//	double midY = Itrans.rows / 2;
//
//	cv::Mat MatchImage = cv::Mat::zeros(fMap.size(), CV_8UC3);
//	double DSC = 0;
//
//	double maxDSC = 0;
//	double maxAngle = -1000;
//	double MinSearchAngle = 180 - 45;
//	double MaxSearchAngle = 180 + 45;
//	int num = 0;
//	for (double angle = MinSearchAngle; angle <= MaxSearchAngle; angle++)
//	{
//		num++;
//		cv::Mat Irot = cv::Mat::zeros(Itrans.rows, Itrans.cols, Itrans.type());
//
//		cv::Point center = cv::Point(Itrans.cols / 2, Itrans.rows / 2);
//		double scale = 1.0;
//		cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, scale);
//		warpAffine(Itrans, Irot, rotMat, Irot.size());
//		CompareImages(fMap, Irot, CMx, CMy, MatchImage, DSC);
//
//		//char path[20];
//		//sprintf_s(path, "%s%d%s", "test/image", (int)angle, ".bmp");
//		//std::string str(path);
//		//cv::imwrite(path, Irot);
//		if (DSC > maxDSC)
//		{
//			maxDSC = DSC;
//			maxAngle = angle;
//		}
//		//std::cout << "Angle: " << angle << " Similarity: " << DSC << "%"<<std::endl;
//	}
//	std::cout << "Max Similarity " << maxDSC << "% At angle " << maxAngle << std::endl;
//	shapeGame_Similarity = maxDSC;
//	double MinScore = 0.70;
//	double MaxScore = 1;
//	double finalScore = (maxDSC - MinScore) / (MaxScore - MinScore);
//	finalScore = std::max(0.0, finalScore);
//	//cout<<"fin: "<<finalScore <<endl;
//	Score = finalScore * 1000;
//	
//
//	cv::Mat Irot = cv::Mat::zeros(Itrans.rows, Itrans.cols, Itrans.type());
//
//	cv::Point center = cv::Point(Itrans.cols / 2, Itrans.rows / 2);
//	double scale = 1.0;
//	cv::Mat rotMat = cv::getRotationMatrix2D(center, maxAngle, scale);
//	warpAffine(Itrans, Irot, rotMat, Irot.size());
//
//	TMat = cv::Mat::zeros(3, 3, CV_32FC1);
//	TMat.at<float>(2, 2) = 1;
//	for (int c = 0; c < 3; c++)
//	{
//		for (int r = 0; r < 2; r++)
//		{
//			TMat.at<float>(r, c) = rotMat.at<double>(r, c);
//		}
//	}
//	std::vector<cv::Point2f> LMsScaledTransRot(AutoLMs);
//	cv::perspectiveTransform(LMsScaledTrans, LMsScaledTransRot, TMat);
//
//	tX = CMx - ScaledI.cols / 2;
//	tY = CMy - ScaledI.rows / 2;
//	TMat = cv::Mat::zeros(3, 3, CV_32FC1);
//	TMat.at<float>(2, 2) = 1;
//	TMat.at<float>(0, 0) = 1;
//	TMat.at<float>(1, 1) = 1;
//	TMat.at<float>(0, 2) = tX;
//	TMat.at<float>(1, 2) = tY;
//	std::vector<cv::Point2f> LMsScaledTransRotT2(AutoLMs);
//	cv::perspectiveTransform(LMsScaledTransRot, LMsScaledTransRotT2, TMat);
//
//
//	//int xmin = ROI.x;
//	//int ymin = ROI.y;
//	int xmin = 0;
//	int ymin = 0;
//
//	TMat.at<float>(0, 0) = 1;
//	TMat.at<float>(1, 1) = 1;
//	TMat.at<float>(0, 2) = xmin;
//	TMat.at<float>(1, 2) = ymin;
//	std::vector<cv::Point2f> LMsinOrgDepthImage(AutoLMs);
//	cv::perspectiveTransform(LMsScaledTransRotT2, LMsinOrgDepthImage, TMat);
//	return true;
//
//}
//
//bool ofApp::SafeReadBinaryImage(cv::Mat &img, const std::string &fname)
//{
//	ofImage temp;
//	if (!temp.loadImage(fname))
//	{
//		ofLogVerbose("CMapGameController") << "SafeReadBinaryImage(): could not read " + fname;
//		return false;
//	}
//	cv::Mat t1 = ofxCv::toCv(temp.getPixels());
//
//	cv::Mat t2;
//	if (t1.channels() > 1)
//		cv::cvtColor(t1, t2, cv::COLOR_BGR2GRAY);
//	else
//		t1.copyTo(t2);
//
//	cv::normalize(t2, img, 0, 255, cv::NORM_MINMAX);
//	return true;
//}
//
//void ofApp::ComputeLandmarksFromTemplate(cv::Mat& I, std::vector<cv::Point2f> &LMs) {
//	LMs.clear();
//
//	int maxx = 0;
//	int minx = I.cols;
//	int maxy = 0;
//	int miny = I.rows;
//
//	// Find template size
//	for (int i = 0; i < I.rows; ++i)
//	{
//		const uchar *p = I.ptr<uchar>(i);
//		for (int j = 0; j < I.cols; ++j)
//		{
//			if (p[j] > 0)
//			{
//				if (i > maxy)
//					maxy = i;
//				if (i < miny)
//					miny = i;
//				if (j > maxx)
//					maxx = j;
//				if (j < minx)
//					minx = j;
//			}
//		}
//	}
//
//	// Squeeze it 20% not to get extreme points
//	double W = maxx - minx;
//	double H = maxy - miny;
//	maxx = minx + W * 0.80;
//	minx = minx + W * 0.20;
//	maxy = miny + H * 0.80;
//	miny = miny + H * 0.20;
//
//	int nx = 5;
//	int ny = 5;
//
//	for (int x = 0; x < nx; x++)
//	{
//		double LMx = (double)minx + x / (double)(nx - 1) * (double)(maxx - minx);
//		for (int y = 0; y < ny; y++)
//		{
//			double LMy = (double)miny + y / (double)(ny - 1) * (double)(maxy - miny);
//			//cout<< LMx <<endl;
//			//cout << LMy << endl;
//			//cout << endl;
//			LMs.push_back(cv::Point2f(LMx, LMy));
//		}
//	}
//}
//
//bool ofApp::ReferenceToCM(cv::Mat& I, cv::Mat& Iout, double &rCMx, double &rCMy)
//{
//	rCMx = 0;
//	rCMy = 0;
//
//	if (!ComputeCM(I, rCMx, rCMy))
//	{
//		std::cout << "Could not compute CM" << std::endl;
//		return false;
//	}
//
//	double midX = I.cols / 2;
//	double midY = I.rows / 2;
//
//	//	std::cout << "CM: " << rCMx << ", " << rCMy << " mid: " << midX << ", " << midY << std::endl;
//
//	double tX = midX - rCMx;
//	double tY = midY - rCMy;
//
//	cv::Mat trans_mat = cv::Mat::zeros(2, 3, CV_32FC1);
//	trans_mat.at<float>(0, 0) = 1;
//	trans_mat.at<float>(1, 1) = 1;
//	trans_mat.at<float>(0, 2) = tX;
//	trans_mat.at<float>(1, 2) = tY;
//
//	warpAffine(I, Iout, trans_mat, Iout.size());
//	return true;
//}
//
//bool ofApp::ComputeCM(const cv::Mat& I, double& CMx, double& CMy)
//{
//	CMx = 0;
//	CMy = 0;
//	int N = 0;
//
//	// Compute CM
//	for (int i = 0; i < I.rows; ++i)
//	{
//		const uchar *p = I.ptr<uchar>(i);
//		for (int j = 0; j < I.cols; ++j)
//		{
//			if (p[j] > 0)
//			{
//				CMx += j;
//				CMy += i;
//				N++;
//			}
//		}
//	}
//	if (N == 0)
//		return false;
//
//	CMx /= N;
//	CMy /= N;
//
//	return true;
//}
//
//void ofApp::CompareImages(cv::Mat& I, cv::Mat& T, int Xi, int Yi, cv::Mat& MatchImage, double &DSC) {
//	int dX = (Xi - T.cols / 2);
//	int dY = (Yi - T.rows / 2);
//
//	// Run through template
//
//	int match = 0;
//	int Inm = 0;
//
//	int Tnm = 0;
//	int NI = 0;
//	int NT = 0;
//	for (int i = 0; i < T.rows; ++i)
//	{
//		int Ii = i + dY;
//		if (Ii >= 0 && Ii < I.rows)
//		{
//			uchar *pI = I.ptr<uchar>(Ii);
//			uchar *pT = T.ptr<uchar>(i);
//			for (int j = 0; j < T.cols; ++j)
//			{
//				int Ij = j + dX;
//				if (Ij >= 0 && Ij < I.cols)
//				{
//					cv::Vec3b &MarkPixel = MatchImage.at<cv::Vec3b>(Ii, Ij);
//
//					bool Ion = pI[Ij] > 0;
//					bool Ton = pT[j] > 0;
//
//					if (Ion)
//						NI++;
//					if (Ton)
//						NT++;
//
//					if (Ion && Ton)
//					{
//						match++;
//						MarkPixel = cv::Vec3b(0, 255, 0);
//					}
//
//					if (Ion && !Ton)
//					{
//						Inm++;
//						MarkPixel = cv::Vec3b(128, 0, 0);
//					}
//
//					if (!Ion && Ton)
//					{
//						Tnm++;
//						MarkPixel = cv::Vec3b(0, 0, 128);
//					}
//
//				}
//			}
//		}
//	}
//	DSC = 2 * (double)match / (NI + NT);
//}
//
//void ofApp::RemoveBorderBlobs(cv::Mat& image)
//{
//	const uchar white(255);
//
//	// do top and bottom row
//	for (int y = 0; y < image.rows; y += image.rows - 1)
//	{
//		uchar* row = image.ptr<uchar>(y);
//		for (int x = 0; x < image.cols; ++x)
//		{
//			if (row[x] == white)
//			{
//				cv::floodFill(image, cv::Point(x, y), cv::Scalar(0), (cv::Rect*)0, cv::Scalar(), cv::Scalar(200));
//			}
//		}
//	}
//	// fix left and right sides
//	for (int y = 0; y < image.rows; ++y)
//	{
//		uchar* row = image.ptr<uchar>(y);
//		for (int x = 0; x < image.cols; x += image.cols - 1)
//		{
//			if (row[x] == white)
//			{
//				cv::floodFill(image, cv::Point(x, y), cv::Scalar(0), (cv::Rect*)0, cv::Scalar(), cv::Scalar(200));
//			}
//		}
//	}
//}

//bool ofApp::getBinaryLandImage(ofxCvGrayscaleImage& BinImg) {
//	float *imgData = FilteredDepthImage.getFloatPixelsRef().getData();
//	ofVec2f kinectRes;
//	kinectRes.x = 800;
//	kinectRes.y = 800;
//	BinImg.allocate(kinectRes.x,kinectRes.y);
//	unsigned char *binData = BinImg.getPixels().getData();
//	for (int y = 0; y < kinectRes.y; y++)
//	{
//		for (int x = 0; x < kinectRes.x; x++)
//		{
//			int IDX = y * kinectRes.x + x;
//			double val = imgData[IDX];
//
//			//float H = elevationAtKinectCoord(x, y);
//
//			//unsigned char BinOut = 255 * (H > 0);
//
//			binData[IDX] = BinOut;
//		}
//	}
//
//	return true;
//
//}