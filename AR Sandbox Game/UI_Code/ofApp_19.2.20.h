#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "KinectGrabber.h"
#include "ofxModal.h"
#include "KinectProjector/KinectProjector.h"
#include "SandSurfaceRenderer/SandSurfaceRenderer.h"

#include "KinectProjectorCalibration.h"
#include "Utils.h"
#include "TemporalFrameFilter.h"

#include "ShapeGameManager.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawProjWindow(ofEventArgs& args);
		void drawPcjWindow(ofEventArgs& args);

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
		bool matchMap();
		bool SafeReadBinaryImage(cv::Mat &img, const std::string &fname);
		void ComputeLandmarksFromTemplate(cv::Mat& I, std::vector<cv::Point2f> &LMs);
		bool ReferenceToCM(cv::Mat& I, cv::Mat& Iout, double &rCMx, double &rCMy);
		bool ComputeCM(const cv::Mat& I, double& CMx, double& CMy);
		void CompareImages(cv::Mat& I, cv::Mat& T, int Xi, int Yi, cv::Mat& MatchImage, double &DSC);
		double Score;
		bool ConnectedComponentAnalysis();
		void RemoveBorderBlobs(cv::Mat& image);
		cv::Rect ROI;
		cv::Mat maxBLOBImage;
		std::vector<ofVec2f> MatchResultContours;
		//
		//bool getBinaryLandImage(ofxCvGrayscaleImage& BinImg);
		ofxCvFloatImage             FilteredDepthImage;
		std::shared_ptr<ofAppBaseWindow> projWindow;
		std::shared_ptr<ofAppBaseWindow> pcjWindow;
		
		//UI button
		ofxDatGui* Lunar_Starter;
		ofxDatGui* Sandbox_Starter;
		ofxDatGui* ShapeHeart_Starter;
		ofxDatGui* Sandbox_flood;
		ofxDatGui* Triangle_Starter;
		ofxDatGui* Square_Starter;
		ofxDatGui* Circle_Starter;
		ofxDatGui* Leardboard_Starter;
		ofxDatGui* ShapeUK_Starter;
		void onButtonEvent(ofxDatGuiButtonEvent e);
		ofxModalAlert myAlert;

		float height = -0.5;

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
		float shapeGame_Similarity;

		bool changeTheme = false;

		bool sandboxFlood = false;
		float elapsedTime;
		float similarityCheckTimer;

		


		
	
		
};
