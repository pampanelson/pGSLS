#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
//#include "ofxFlowTools.h"
#include "MyFlowTools.h"
#include "ofxFft.h"


class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();

	// Camera
	ofVideoGrabber		simpleCam;
	bool				didCamUpdate;
	ftFbo				cameraFbo;
	ofParameter<bool>	doFlipCamera;
	
	
	// opencv
	ofxCv::ContourFinder 	contourFinder;
	ofColor 				targetColor;
	ofImage					grayImg;
	
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
	bool				drawTestImage = false;
	bool				drawBackImg = true;
	
	
	ofxPanel			gui;
	ofParameter<float> 	threshold;
	ofParameter<bool> 	trackHs;
	ofParameter<int>	blackWhiteThreshold;

	
	
	
	
	
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
