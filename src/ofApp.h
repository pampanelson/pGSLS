#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
//#include "ofxFlowTools.h"
#include "MyFlowTools.h"
#include "ofxFft.h"
#include "ofxSyphon.h"
#include "ofxKinect.h"


using namespace cv;
using namespace ofxCv;

class Scheduler : public ofThread {
public:
	unsigned long counterSec = 0;
	unsigned long counterMil = 0;
	Scheduler() {
//		timerSec.setPeriodicEvent(1000000000); // 1 second
		timerMil.setPeriodicEvent(1000000); // 1 mil second
		startThread();
	}
private:
//	ofTimer timerSec;
	ofTimer timerMil;
	void threadedFunction() {
		while (isThreadRunning()) {
			timerMil.waitNext();
			counterMil++;
			
			if(counterMil % 1000 == 0){
				counterSec++;
			}
			
		}
		
	}
};





class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();

	
	Scheduler               myTimer;

	
	// Camera
	ofVideoGrabber		simpleCam;
	bool				didCamUpdate;
	ftFbo				cameraFbo;
	ofParameter<bool>	doFlipCamera;
	
	
	// kinect --------
	ofxKinect 			kinect;

	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
	
	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	
	int nearThreshold;
	int farThreshold;
	
	int angle;
	
	
	
	
	
	// for debug contour =======================
	
	
	// Time
	float				lastTime;
	float				curTime;
	float				deltaTime;
	
	// FlowTools
	int					flowWidth;
	int					flowHeight;
	int					drawWidth;
	int					drawHeight;
	
	
	// syphon
	ofFbo				syphonFbo;
	ofPixels			fboPixels;
	ofImage				fboImg;
	
	ofxSyphonServer		syphonServer;
	
	
	// flow
	ofFbo               flowFbo;
	ofFbo               obsticleFbo;
	

	// shufa
	ofImage				shufaDiff;
	ofImage				overlap;
	ofImage				shufaImg1;
	
	
	ofPixels 			previous;
	ofImage				diff;
	cv::Scalar 			diffMean;
	float				diffValue;
	

	
	ofxPanel			gui;
	ofParameter<bool> 	bFlipCamera;
	ofParameter<float> 	threshold;
	ofParameter<float> 	diffThreshold;
	ofParameter<float> 	diffValueFactorForDissipation;
	ofParameter<float> 	dissipationTopValue;



	ofParameter<bool> 	trackHs;
	ofParameter<int>	blackWhiteThreshold;
	ofParameter<float>	rmsThreshold;
	

	bool				bDrawGui = true;
	
	
	vector<MyFlowTools *>		vecMyFlowTools;
	
	

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
	
};
