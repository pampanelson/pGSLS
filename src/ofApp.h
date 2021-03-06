#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
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

	ofxSyphonServer 		individualTextureSyphonServer;
	ofFbo					syphonFbo;
	
	
	Scheduler               myTimer;

	
	// Camera
	ofVideoGrabber		simpleCam;
	bool				didCamUpdate;
	ftFbo				cameraFbo;
	ofParameter<bool>	doFlipCamera;
	
	
	// opencv
	ofxCv::ContourFinder 	contourFinder;
	ofColor 				targetColor;
	ofImage					grayImg;
	
	// for debug contour =======================
	
	int					contourIndex = -1;
	
	// Time
	float				lastTime;
	float				deltaTime;
	
	// FlowTools
	int					flowWidth;
	int					flowHeight;
	int					drawWidth;
	int					drawHeight;
	
	
	MyFlowTools			myFlowTools;
	float				ratio;
	
	ofFbo				fbo;
	ofPixels			fboPixels;
	ofImage				fboImg;
	
	ofFbo               flowFbo;
	ofFbo               obsticleFbo;
	
	// sound
	
	void 				audioIn(ofSoundBuffer &inBuffer);
	//	void				audioIn(float *input, int bufferSize, int nChannels);
	ofSoundStream 		soundStream;
	ofSoundBuffer 		audioInSoundBuffer;
	ofSoundBuffer		chennalBuffer;
	ofxFft* 			fft;
	float * 			fftData;
	
	float 				rms;
	float 				power;
	
	vector <float> 		left;
	vector <float> 		right;
	
	
	
	// test
	
	ofImage				imgTest1;
	ofImage				imgTest3;
	ofImage				obsticleImg; // invert of back image
	ofImage				backImg; // show rest writtings
	bool				drawTestImage = false;
	bool				drawBackImg = true;
	
	
	vector<vector<cv::Point>>  contourBlobs;
	vector<ofImage>		vecContourBlobImagesForFlow;
	vector<ofVec4f>		vecContourBlobImagesBoudingBox;
	int					drawImageForFlowMilSecs = 1000;
	int					deltaSpeedFactor = 20;// how fast a contour blob move for make flow,smaller is quicker
	vector<int>			vecDrawImageForFlowOnce;
	
	
	
	ofxPanel			gui;
	ofParameter<float> 	threshold;
	ofParameter<bool> 	trackHs;
	ofParameter<bool> 	bStartFlow;
	ofParameter<bool> 	bContourFinderUpdate;
	ofParameter<bool> 	bShowContour;
	ofParameter<int>	blackWhiteThreshold;
	ofParameter<float>	rmsThreshold;

	bool				bDrawGui = true;
	
	
	void				goNextFlow();
	void				showAudioDebug();
	
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
