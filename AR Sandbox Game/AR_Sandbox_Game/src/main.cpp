#include "ofMain.h"
#include "ofApp.h"

//========================================================================
const std::string MagicSandVersion = "";
bool setWindowDimensions(ofGLFWWindowSettings& settings, int windowsNum) {
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	cout << "Number of screens found: " << count << endl;
	if (count > windowsNum) {
		int xM; int yM;
		glfwGetMonitorPos(monitors[windowsNum], &xM, &yM); // We take the first monitor
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[windowsNum]);

		cout << "Monitor " << windowsNum << " size: " << desktopMode->width << "x" << desktopMode->height << endl;

		if (windowsNum == 0)
		{
			// Make main window almost full screen - but just a bit of space around to be able to grab other windows
			settings.width = desktopMode->width * 4.0 / 5.0;
			settings.height = desktopMode->height * 4.0 / 5.0;
		}
		else
		{
			// Projector window full screen
			settings.width = desktopMode->width;
			settings.height = desktopMode->height;
		}

		settings.setPosition(ofVec2f(xM, yM));

		return true;
	}
	else {
		settings.width = 1600; // Default settings
		settings.height = 800;
		settings.setPosition(ofVec2f(0, 0));
		return false;
	}

}
int main( ){
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//ofRunApp(new ofApp());
	ofGLFWWindowSettings settings;
	//	setFirstWindowDimensions(settings);
	//settings.width = 1200;
	//	settings.height = 600;
	settings.width = 1600; // Default settings
	settings.height = 800;
	settings.setPosition(ofVec2f(0, 0));
	settings.resizable = true;
	settings.decorated = true;
	settings.title = "AR Sandbox Game" + MagicSandVersion;
	//shared_ptr<ofAppBaseWindow> thirdWindow = ofCreateWindow(settings);
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);



	setWindowDimensions(settings, 0);
	mainWindow->setWindowPosition(ofGetScreenWidth() / 2 - settings.width / 2 - 30, ofGetScreenHeight() / 2 - settings.height / 2);
	mainWindow->setWindowShape(settings.width, settings.height);
	//
	//thirdWindow->setWindowPosition(ofGetScreenWidth() / 2 - settings.width / 2, ofGetScreenHeight() / 2 - settings.height / 2);
	//shared_ptr<ofApp> mainApp2(new ofApp);
	//ofAddListener(thirdWindow->events().draw, mainApp2.get(), &ofApp::drawProjWindow);
	//mainApp2->projWindow = thirdWindow;
	//
	//thirdWindow->setWindowPosition(ofGetScreenWidth() / 2 - settings.width / 2, ofGetScreenHeight() / 2 - settings.height / 2);
	//thirdWindow->setWindowShape(1080, 1920);

	setWindowDimensions(settings, 1);
	settings.resizable = false;

	settings.decorated = false;
	settings.shareContextWith = mainWindow;
	//shared_ptr<ofAppBaseWindow> thirdWindow = ofCreateWindow(settings);
	shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(settings);
	


	secondWindow->setVerticalSync(false);

	shared_ptr<ofApp> mainApp(new ofApp);
	//ofAddListener(thirdWindow->events().draw, mainApp.get(), &ofApp::drawPcjWindow);
	ofAddListener(secondWindow->events().draw, mainApp.get(), &ofApp::drawProjWindow);

	mainApp->projWindow = secondWindow;
	//mainApp->pcjWindow = thirdWindow;


	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}


