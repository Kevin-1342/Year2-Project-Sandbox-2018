#include "ShapeGameManager.h"

ShapeGameManager::ShapeGameManager() {
	referenceDepthMap[0] = "ShapeGameData/circle_DF.png";
	referenceDepthMap[1] = "ShapeGameData/square_DF.png";
	referenceDepthMap[2] = "ShapeGameData/triangle_DF.png";
	referenceDepthMap[3] = "ShapeGameData/heart_DF.png";
	referenceDepthMap[4] = "ShapeGameData/GB_DF.png";
	referenceColorMap[0] = "ShapeGameData/circle_CF.png";
	referenceColorMap[1] = "ShapeGameData/square_CF.png";
	referenceColorMap[2] = "ShapeGameData/triangle_CF.png";
	referenceColorMap[3] = "ShapeGameData/heart_CF.png";
	referenceColorMap[4] = "ShapeGameData/GB_CF.png";
	referenceName[0] = "Circle: ";
	referenceName[1] = "Square: ";
	referenceName[2] = "Regular Triangle: ";
	referenceName[3] = "Heart: ";
	referenceName[4] = "Great Britain: ";
}

void ShapeGameManager::loadLeaderBoard() {
	//ifstream readFile("ShapeGameData/LeaderBoard.txt");
	fstream fout;
	//fout.open("bin/data/ShapeGameData/LeaderBoard.txt");
	fout.open("c:/data/LeaderBoard.txt");
	if (!fout) {
		cout << "loading leaderboard error" << endl;
	}
	//ifstream fin("bin/data/ShapeGameData/LeaderBoard.txt");
	ifstream fin("c:/data/LeaderBoard.txt");
	for (int i = 0; i < shapeNum;i++) {
		for (int j = 0; j < leaderNum;j++) {
			fin >> shapeLeaderBoard[i][j];
			cout<< shapeLeaderBoard[i][j] <<endl;
		}
	}
	sortLeaderBoard();
}

void ShapeGameManager::saveLeaderBoard() {
	sortLeaderBoard();
	//ofstream outfile("bin/data/ShapeGameData/LeaderBoard.txt");
	ofstream outfile("c:/data/LeaderBoard.txt");
	for (int i = 0; i < shapeNum; i++) {
		for (int j = 0; j < leaderNum; j++) {
			stringstream ttext;
			string BoardText;
			ttext << shapeLeaderBoard[i][j];
			BoardText = ttext.str();
			outfile << BoardText <<endl;
		}
	}
}

void ShapeGameManager::sortLeaderBoard() {
	for (int i = 0; i < shapeNum; i++) {
		float sortList[3] = { shapeLeaderBoard[i][0],shapeLeaderBoard[i][1],shapeLeaderBoard[i][2] };
		sort(sortList, sortList + 3);
		shapeLeaderBoard[i][0] = sortList[2];
		shapeLeaderBoard[i][1] = sortList[1];
		shapeLeaderBoard[i][2] = sortList[0];
	}
}

float ShapeGameManager::getMaxScore(int shape) {
	sortLeaderBoard();
	return shapeLeaderBoard[shape][0];
}

float ShapeGameManager::getMinScore(int shape) {
	sortLeaderBoard();
	return shapeLeaderBoard[shape][2];
}

void ShapeGameManager::addScore(int shape, float score) {
	float sortList[4] = { shapeLeaderBoard[shape][0],shapeLeaderBoard[shape][1],shapeLeaderBoard[shape][2],score };
	sort(sortList, sortList + 4);
	shapeLeaderBoard[shape][0] = sortList[3];
	shapeLeaderBoard[shape][1] = sortList[2];
	shapeLeaderBoard[shape][2] = sortList[1];
	saveLeaderBoard();
}

void ShapeGameManager::setProjectorRes(ofVec2f& PR)
{
	projRes = PR;

	fboProjWindow.allocate(projRes.x, projRes.y, GL_RGBA);
	fboProjWindow.begin();
	ofClear(255, 255, 255, 0);
	ofBackground(255);
	fboProjWindow.end();
	elapsedTime = ofGetElapsedTimef();
}

void ShapeGameManager::setKinectRes(ofVec2f& KR)
{
	kinectRes = KR;
}

void ShapeGameManager::setKinectROI(ofRectangle &KROI)
{
	kinectROI = KROI;
	ROI.x = kinectROI.x;
	ROI.y = kinectROI.y;
	ROI.width = kinectROI.width;
	ROI.height = kinectROI.height;
}

bool ShapeGameManager::GetActualBinaryLandImage()

{
	ofLogVerbose("GetActualDepthImage") << "getting raw land data ";

	std::string BinOutName = "BinaryLandImage.png";

	if (!kinectProjector->getBinaryLandImage(BinaryLandImage))
		return false;

	ofSaveImage(BinaryLandImage.getPixels(), BinOutName);

	ofLogVerbose("GetActualDepthImage") << "done getting raw land ";
	return true;
}

void ShapeGameManager::drawScore(double shapeGame_Similarity) {
	fboProjWindow.allocate(projRes.x, projRes.y, GL_RGBA);
	fboProjWindow.draw(0, 0);
	if (!scoreFont.loadFont("verdana.ttf", 64))
		std::cout << "Could not read font verdana.ttf" << std::endl;
	fboProjWindow.begin();
	stringstream ttext;
	string ScoreText;
	int tscore = (int)shapeGame_Similarity;
	ttext << " Current Similarity: " << tscore << "%";
	ofSetColor(10, 30, 200);
	ScoreText = ttext.str();
	scoreFont.drawString(ScoreText, 100, 100);
	cout<< shapeGame_Similarity <<endl;
	fboProjWindow.end();

}

bool ShapeGameManager::maxOccupationCutout() {
	//std::string iThres = "BinaryLandImage.png";
	std::string iThres = "TestImage.png";
	std::string onameRoiPng = "bin/data/HeightMap_ROI.png";;
	std::string oMaxBlob = "HM_cutout_maxBlob.png";
	cv::Mat M;
	if (BinaryLandImage.width == 0)
	{
		if (!readBinaryImage(M, iThres))
		{
			ofLogVerbose("CMapGameController") << "ConnectedComponentAnalysis(): could not read BinaryLandImage.png";
			return false;
		}
	}
	else
	{
		M = ofxCv::toCv(BinaryLandImage);
	}
	//SafeReadBinaryImage(M, iThres);
	//
	//int xmin = 0;
	//int ymin = 0;
	//int xmax = 640;
	//int ymax = 480;
	//ROI = cv::Rect(xmin, ymin, xmax - xmin, ymax - ymin);
	//
	//cv::Mat bw = M(ROI);

	removeOffView(M);
	//cv::imwrite("output1.bmp", M);
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(M, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	if (contours.size() == 0)
	{
		//ofLogVerbose("CMapGameController") << "ConnectedComponentAnalysis(): could not find any contours";
		//cout << "Please put hands off!" << endl;
		return false;
	}
	int MaxCont = 0;
	int maxID = -1;

	for (int i = 0; i < contours.size(); i++)
	{
		int sz = contours[i].size();
		if (sz > MaxCont)
		{
			MaxCont = sz;
			maxID = i;
		}
	}

	maxBLOBImage = cv::Mat::zeros(M.size(), CV_8UC1);
	drawContours(maxBLOBImage, contours, maxID, 255, CV_FILLED, 8);
	//cv::imwrite(, maxBLOBImage);
	cv::Mat t;
	maxBLOBImage.copyTo(t);

	unsigned char *input = (unsigned char*)(t.data);
	ofxCvGrayscaleImage img2;
	img2.setFromPixels(input, t.cols, t.rows);
	ofSaveImage(img2.getPixels(), oMaxBlob);

	return true;

}

void ShapeGameManager::chooseShape(int num) {
	chooseMap = num;
}

int ShapeGameManager::getShapeNum(){
	return chooseMap;
}
bool ShapeGameManager::calSimilarity() {

	referenceMap = referenceDepthMap[chooseMap];
	//referenceMap = "sss_GT.png";
	cv::Mat fMap;
	cv::Mat refMap;
	if (BinaryLandImage.width == 0)
	{
		if (!readBinaryImage(fMap, FoundMap))
			return false;
	}
	else
	{
		fMap = maxBLOBImage;
	}
	if (!readBinaryImage(refMap, referenceMap))
		return false;
	std::vector<cv::Point2f> AutoLMs;
	computeLandmarks(refMap, AutoLMs);
	//cout << refMap << endl;
	//cv::imwrite("output1.bmp", fMap);
	//cv::imwrite("output2.bmp", refMap);
	double AfMap = cv::sum(fMap)[0] / 255.0;
	double ArefMap = cv::sum(refMap)[0] / 255.0;
	double factor = sqrt(AfMap / ArefMap);
	cv::Mat ScaledI;

	//blow up
	cv::resize(refMap, ScaledI, cv::Size(), factor, factor, cv::INTER_AREA);
	//cv::imwrite("output1.bmp", ScaledI);
	cv::Mat TMat = cv::Mat::zeros(3, 3, CV_32FC1);
	TMat.at<float>(0, 0) = factor;
	TMat.at<float>(1, 1) = factor;
	TMat.at<float>(2, 2) = 1;
	std::vector<cv::Point2f> LMsScaled(AutoLMs.size());
	cv::perspectiveTransform(AutoLMs, LMsScaled, TMat);

	cv::Mat Itrans = cv::Mat::zeros(ScaledI.rows, ScaledI.cols, ScaledI.type());
	double rCMx = 0;
	double rCMy = 0;
	if (!ReferenceToCM(ScaledI, Itrans, rCMx, rCMy))
		return false;
	//cv::imwrite("output2.bmp", Itrans);

	double tX = ScaledI.cols / 2 - rCMx;
	double tY = ScaledI.rows / 2 - rCMy;
	TMat.at<float>(0, 0) = 1;
	TMat.at<float>(1, 1) = 1;
	TMat.at<float>(0, 2) = tX;
	TMat.at<float>(1, 2) = tY;
	std::vector<cv::Point2f> LMsScaledTrans(AutoLMs.size());
	cv::perspectiveTransform(LMsScaled, LMsScaledTrans, TMat);

	double CMx = 0;
	double CMy = 0;
	ComputeCM(fMap, CMx, CMy);
	double midX = Itrans.cols / 2;
	double midY = Itrans.rows / 2;

	cv::Mat MatchImage = cv::Mat::zeros(fMap.size(), CV_8UC3);
	double DSC = 0;

	double maxDSC = 0;
	double maxAngle = -1000;
	double MinSearchAngle = 180 - 45;
	double MaxSearchAngle = 180 + 45;
	int num = 0;
	for (double angle = MinSearchAngle; angle <= MaxSearchAngle; angle++)
	{
		num++;
		cv::Mat Irot = cv::Mat::zeros(Itrans.rows, Itrans.cols, Itrans.type());

		cv::Point center = cv::Point(Itrans.cols / 2, Itrans.rows / 2);
		double scale = 1.0;
		cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, scale);
		warpAffine(Itrans, Irot, rotMat, Irot.size());
		//cv::imwrite("a.bmp", fMap);
		//cv::imwrite("b.bmp", Irot);
		CompareImages(fMap, Irot, CMx, CMy, MatchImage, DSC);

		//char path[20];
		//sprintf_s(path, "%s%d%s", "test/image", (int)angle, ".bmp");
		//std::string str(path);
		//cv::imwrite(path, Irot);
		if (DSC > maxDSC)
		{
			maxDSC = DSC;
			maxAngle = angle;
		}
		//std::cout << "Angle: " << angle << " Similarity: " << DSC << "%"<<std::endl;
	}
	std::cout << "Max Similarity " << maxDSC << "% At angle " << maxAngle << std::endl;
	shapeGame_Similarity = maxDSC;
	double MinScore = 0.70;
	double MaxScore = 1;
	double finalScore = (maxDSC - MinScore) / (MaxScore - MinScore);
	finalScore = std::max(0.0, finalScore);
	//cout<<"fin: "<<finalScore <<endl;
	//Score = finalScore * 1000;


	cv::Mat Irot = cv::Mat::zeros(Itrans.rows, Itrans.cols, Itrans.type());

	cv::Point center = cv::Point(Itrans.cols / 2, Itrans.rows / 2);
	double scale = 1.0;
	cv::Mat rotMat = cv::getRotationMatrix2D(center, maxAngle, scale);
	warpAffine(Itrans, Irot, rotMat, Irot.size());

	TMat = cv::Mat::zeros(3, 3, CV_32FC1);
	TMat.at<float>(2, 2) = 1;
	for (int c = 0; c < 3; c++)
	{
		for (int r = 0; r < 2; r++)
		{
			TMat.at<float>(r, c) = rotMat.at<double>(r, c);
		}
	}
	std::vector<cv::Point2f> LMsScaledTransRot(AutoLMs);
	cv::perspectiveTransform(LMsScaledTrans, LMsScaledTransRot, TMat);

	tX = CMx - ScaledI.cols / 2;
	tY = CMy - ScaledI.rows / 2;
	TMat = cv::Mat::zeros(3, 3, CV_32FC1);
	TMat.at<float>(2, 2) = 1;
	TMat.at<float>(0, 0) = 1;
	TMat.at<float>(1, 1) = 1;
	TMat.at<float>(0, 2) = tX;
	TMat.at<float>(1, 2) = tY;
	std::vector<cv::Point2f> LMsScaledTransRotT2(AutoLMs);
	cv::perspectiveTransform(LMsScaledTransRot, LMsScaledTransRotT2, TMat);


	//int xmin = ROI.x;
	//int ymin = ROI.y;
	int xmin = 0;
	int ymin = 0;

	TMat.at<float>(0, 0) = 1;
	TMat.at<float>(1, 1) = 1;
	TMat.at<float>(0, 2) = xmin;
	TMat.at<float>(1, 2) = ymin;
	std::vector<cv::Point2f> LMsinOrgDepthImage(AutoLMs);
	cv::perspectiveTransform(LMsScaledTransRotT2, LMsinOrgDepthImage, TMat);
	return true;
	

}

bool ShapeGameManager::readBinaryImage(cv::Mat &image, const std::string &path)
{
	ofImage temp;
	if (!temp.loadImage(path))
	{
		ofLogVerbose("CMapGameController") << "SafeReadBinaryImage(): could not read " + path;
		return false;
	}
	cv::Mat t1 = ofxCv::toCv(temp.getPixels());

	cv::Mat t2;
	if (t1.channels() > 1)
		cv::cvtColor(t1, t2, cv::COLOR_BGR2GRAY);
	else
		t1.copyTo(t2);

	cv::normalize(t2, image, 0, 255, cv::NORM_MINMAX);
	return true;
}

void ShapeGameManager::removeOffView(cv::Mat& image){
	const uchar white(255);

	for (int y = 0; y < image.rows; y += image.rows - 1)
	{
		uchar* row = image.ptr<uchar>(y);
		for (int x = 0; x < image.cols; ++x)
		{
			if (row[x] == white)
			{
				cv::floodFill(image, cv::Point(x, y), cv::Scalar(0), (cv::Rect*)0, cv::Scalar(), cv::Scalar(200));
			}
		}
	}

	for (int y = 0; y < image.rows; ++y)
	{
		uchar* row = image.ptr<uchar>(y);
		for (int x = 0; x < image.cols; x += image.cols - 1)
		{
			if (row[x] == white)
			{
				cv::floodFill(image, cv::Point(x, y), cv::Scalar(0), (cv::Rect*)0, cv::Scalar(), cv::Scalar(200));
			}
		}
	}
}

void ShapeGameManager::computeLandmarks(cv::Mat& I, std::vector<cv::Point2f> &LMs) {
	LMs.clear();

	int maxx = 0;
	int minx = I.cols;
	int maxy = 0;
	int miny = I.rows;

	// Find template size
	for (int i = 0; i < I.rows; ++i)
	{
		const uchar *p = I.ptr<uchar>(i);
		for (int j = 0; j < I.cols; ++j)
		{
			if (p[j] > 0)
			{
				if (i > maxy)
					maxy = i;
				if (i < miny)
					miny = i;
				if (j > maxx)
					maxx = j;
				if (j < minx)
					minx = j;
			}
		}
	}

	// Squeeze it 20% not to get extreme points
	double W = maxx - minx;
	double H = maxy - miny;
	maxx = minx + W * 0.80;
	minx = minx + W * 0.20;
	maxy = miny + H * 0.80;
	miny = miny + H * 0.20;

	int nx = 5;
	int ny = 5;

	for (int x = 0; x < nx; x++)
	{
		double LMx = (double)minx + x / (double)(nx - 1) * (double)(maxx - minx);
		for (int y = 0; y < ny; y++)
		{
			double LMy = (double)miny + y / (double)(ny - 1) * (double)(maxy - miny);
			//cout<< LMx <<endl;
			//cout << LMy << endl;
			//cout << endl;
			LMs.push_back(cv::Point2f(LMx, LMy));
		}
	}
}

bool ShapeGameManager::ReferenceToCM(cv::Mat& I, cv::Mat& Iout, double &rCMx, double &rCMy)
{
	rCMx = 0;
	rCMy = 0;

	if (!ComputeCM(I, rCMx, rCMy))
	{
		std::cout << "Could not compute CM" << std::endl;
		return false;
	}

	double midX = I.cols / 2;
	double midY = I.rows / 2;

	//	std::cout << "CM: " << rCMx << ", " << rCMy << " mid: " << midX << ", " << midY << std::endl;

	double tX = midX - rCMx;
	double tY = midY - rCMy;

	cv::Mat trans_mat = cv::Mat::zeros(2, 3, CV_32FC1);
	trans_mat.at<float>(0, 0) = 1;
	trans_mat.at<float>(1, 1) = 1;
	trans_mat.at<float>(0, 2) = tX;
	trans_mat.at<float>(1, 2) = tY;

	warpAffine(I, Iout, trans_mat, Iout.size());
	return true;
}

bool ShapeGameManager::ComputeCM(const cv::Mat& I, double& CMx, double& CMy)
{
	CMx = 0;
	CMy = 0;
	int N = 0;

	// Compute CM
	for (int i = 0; i < I.rows; ++i)
	{
		const uchar *p = I.ptr<uchar>(i);
		for (int j = 0; j < I.cols; ++j)
		{
			if (p[j] > 0)
			{
				CMx += j;
				CMy += i;
				N++;
			}
		}
	}
	if (N == 0)
		return false;

	CMx /= N;
	CMy /= N;

	return true;
}

void ShapeGameManager::CompareImages(cv::Mat& I, cv::Mat& T, int Xi, int Yi, cv::Mat& MatchImage, double &DSC) {
	int dX = (Xi - T.cols / 2);
	int dY = (Yi - T.rows / 2);

	// Run through template

	int match = 0;
	int Inm = 0;

	int Tnm = 0;
	int NI = 0;
	int NT = 0;
	for (int i = 0; i < T.rows; ++i)
	{
		int Ii = i + dY;
		if (Ii >= 0 && Ii < I.rows)
		{
			uchar *pI = I.ptr<uchar>(Ii);
			uchar *pT = T.ptr<uchar>(i);
			for (int j = 0; j < T.cols; ++j)
			{
				int Ij = j + dX;
				if (Ij >= 0 && Ij < I.cols)
				{
					cv::Vec3b &MarkPixel = MatchImage.at<cv::Vec3b>(Ii, Ij);

					bool Ion = pI[Ij] > 0;
					bool Ton = pT[j] > 0;

					if (Ion)
						NI++;
					if (Ton)
						NT++;

					if (Ion && Ton)
					{
						match++;
						MarkPixel = cv::Vec3b(0, 255, 0);
					}

					if (Ion && !Ton)
					{
						Inm++;
						MarkPixel = cv::Vec3b(128, 0, 0);
					}

					if (!Ion && Ton)
					{
						Tnm++;
						MarkPixel = cv::Vec3b(0, 0, 128);
					}

				}
			}
		}
	}
	DSC = 2 * (double)match / (NI + NT);
}

float ShapeGameManager::getShapeGame_Similarity() {

	return shapeGame_Similarity;
}

string ShapeGameManager::getColorMapPath() {
	return referenceColorMap[chooseMap];
}

string ShapeGameManager::getMapName() {
	return referenceName[chooseMap];
}