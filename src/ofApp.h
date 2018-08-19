#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
//#include "ofxFlowTools.h"
#include "MyFlowTools.h"
#include "ofxFft.h"
#include "ofxSyphon.h"




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
	
	
	//opencv

	ofPixels 			previous;
	ofImage 			diff;
	
	// a scalar is like an ofVec4f but normally used for storing color information
	cv::Scalar diffMean;

	
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
	
	
	ofFbo				fbo;
	ofPixels			fboPixels;
	ofImage				fboImg;
	ofFbo               flowFbo;
	ofFbo               obsticleFbo;

	

	bool				bDrawGui = true;
	
	
	vector<MyFlowTools *>		vecMyFlowTools;

	
	
	// syphon out server
	ofxSyphonServer		syphonServer;
	string				syphonServerName;
	ofFbo				syphonFbo;
	
	
	ofxPanel			gui;
	ofParameter<float> 	threshold;
	ofParameter<bool> 	bFlipCam;
	ofParameter<int>	blackWhiteThreshold;
	ofParameter<float>	rmsThreshold;
	
	
	
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
