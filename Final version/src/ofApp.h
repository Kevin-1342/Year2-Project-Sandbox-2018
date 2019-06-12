#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "KinectGrabber.h"
#include "ofxModal.h"
#include "KinectProjector/KinectProjector.h"
#include "SandSurfaceRenderer/SandSurfaceRenderer.h"

#include "iostream"
#include "fstream"

#include "KinectProjectorCalibration.h"
#include "Utils.h"
#include "TemporalFrameFilter.h"

#include "ShapeGameManager.h"

#include "windows.h"
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawProjWindow(ofEventArgs& args);
		void drawPcjWindow(ofEventArgs& args);
		void drawSUIWindow(ofEventArgs& args);
		void drawLeaderWindow(ofEventArgs& args);
		void drawMapWindow(ofEventArgs& args);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		//a
		double Score;
		cv::Rect ROI;
		cv::Mat maxBLOBImage;
		std::vector<ofVec2f> MatchResultContours;
		//
		//bool getBinaryLandImage(ofxCvGrayscaleImage& BinImg);
		ofxCvFloatImage             FilteredDepthImage;
		std::shared_ptr<ofAppBaseWindow> projWindow;
		std::shared_ptr<ofAppBaseWindow> pcjWindow;
		std::shared_ptr<ofAppBaseWindow> SUIWindow;
		std::shared_ptr<ofAppBaseWindow> leadWindow;
		std::shared_ptr<ofAppBaseWindow> GBMapWindow;
		
		//UI button
		ofxDatGui* Lunar_Starter;
		ofxDatGui* Sandbox_Starter;
		ofxDatGui* heartShape_Starter;
		ofxDatGui* Sandbox_flood;
		ofxDatGui* Triangle_Starter;
		ofxDatGui* Square_Starter;
		ofxDatGui* Circle_Starter;
		ofxDatGui* Leardboard_Starter;
		ofxDatGui* ShapeUK_Starter;


		void onButtonEvent(ofxDatGuiButtonEvent e);
		ofxModalAlert myAlert;



private:

		std::shared_ptr<KinectProjector> kinectProjector;
		SandSurfaceRenderer* sandSurfaceRenderer;
		ofRectangle mainWindowROI;
		ShapeGameManager shapeGameManager;
		void getBinaryImage();
		ofxCvGrayscaleImage BinaryLandImage;
		int currentTheme;
		int SANDBOX_THEME = 0;
		int LUNAR_THEME = 1;

		bool shapeGameRunning = false;
		bool shapeGameShowFinal = false;
		bool shapeGameNewRecord = false;
		float shapeGame_Similarity;
		float shapeGame_FinalScroe;

		bool changeTheme = false;

		bool sandboxFlood = false;
		float elapsedTime;
		float similarityCheckTimer;// check sim every 3 seconds

		float floodSpeed = 0.2;
		//Shape
		int const CIRCLE = 0;
		int const SQUARE = 1;
		int const TRIANGLE = 2;
		int const HEART = 3;
		int const GB = 4;

		//Flood
		float floodTimer;
	
		
};
