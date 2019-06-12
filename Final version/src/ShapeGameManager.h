#pragma once
#ifndef _ShapeGameController_h_
#define _ShapeGameController_h_

#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "iostream"
#include "fstream"
#include "../KinectProjector/KinectProjector.h"
class ShapeGameManager {

public:
	ShapeGameManager();
	void setProjectorRes(ofVec2f& PR);
	void setKinectRes(ofVec2f& KR);
	void setKinectROI(ofRectangle &KROI);
	bool GetActualBinaryLandImage();
	void drawScore(double shapeGame_Similarity);
	ofxCvGrayscaleImage BinaryLandImage;
	bool maxOccupationCutout();//ConnectedComponentAnalysis
	bool calSimilarity();//MatchMap
	float getShapeGame_Similarity();
	
	//Shape game
	string referenceMap;
	string referenceImage;
	float shapeGame_Similarity;
	void chooseShape(int num);//0.circle, 1.square, 2.triangle, 3.heart, 4.uk
	int getShapeNum();
	string getColorMapPath();
	string getMapName();

	//leaderboard
	void loadLeaderBoard();
	void saveLeaderBoard();
	void addScore(int shape,float score);
	float getMaxScore(int shape);
	float getMinScore(int shape);




private:
	bool readBinaryImage(cv::Mat &image, const std::string &path);
	void removeOffView(cv::Mat& image);
	void computeLandmarks(cv::Mat& I, std::vector<cv::Point2f> &LMs);
	bool ComputeCM(const cv::Mat& I, double& CMx, double& CMy);
	bool ReferenceToCM(cv::Mat& I, cv::Mat& Iout, double &rCMx, double &rCMy);
	void CompareImages(cv::Mat& I, cv::Mat& T, int Xi, int Yi, cv::Mat& MatchImage, double &DSC);

	std::shared_ptr<KinectProjector> kinectProjector;
	
	string FoundMap = "HM_cutout_maxBlob.png";
	int const static shapeNum = 5;
	int const static leaderNum = 3;
	string referenceDepthMap[shapeNum];
	string referenceColorMap[shapeNum];
	string referenceName[shapeNum];
	float shapeLeaderBoard[shapeNum][leaderNum];
	int chooseMap = 0;

	ofFbo fboProjWindow;
	ofTrueTypeFont scoreFont;
	ofVec2f projRes;
	ofVec2f kinectRes;
	ofRectangle kinectROI;
	cv::Rect ROI;
	cv::Mat maxBLOBImage;

	float elapsedTime;

	//leaderboard
	void sortLeaderBoard();



};
#endif