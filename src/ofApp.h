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
	ofxKinect 			kinect1;

//	ofxCvColorImage 		colorImg;
//	ofxCvGrayscaleImage 	k1GrayImage; // grayscale depth image
//	ofxCvGrayscaleImage 	k1GrayImageThreshNear; // the near thresholded image
//	ofxCvGrayscaleImage 	k1GrayImageThreshFar; // the far thresholded image

	ofImage 	k1GrayImage; // grayscale depth image
	ofImage 	k1GrayImageThreshNear; // the near thresholded image
	ofImage 	k1GrayImageThreshFar; // the far thresholded image

	
	
	// Time
	float				lastTime;
	float				curTime;
	float				deltaTime;
	
	// FlowTools
	int					flowWidth;
	int					flowHeight;
	int					drawWidth;
	int					drawHeight;
	int					halfDrawWidth;
	
	
	//sound
	void 				audioIn(ofSoundBuffer &inBuffer);
	void				showAudioDebug();
	//	void				audioIn(float *input, int bufferSize, int nChannels);
	ofSoundStream 		soundStream;
	ofSoundBuffer 		audioInSoundBuffer;
	ofSoundBuffer		chennalBuffer;
	
	
//	ofxFft* 			fft;
//	float * 			fftData;
	
	float 				rms;
	float 				power;
	
	vector <float> 		left;
	vector <float> 		right;
	
	
	
	
	
	
	
	
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
	ofParameter<float> 	fluidGravityY;

	ofParameter<int>	k1GrayThreshNear;
	ofParameter<int>	k1GrayThreshFar;
	
	ofParameter<bool> 		bThreshWithOpenCV;
	ofParameter<int>  		k1Angle;


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
