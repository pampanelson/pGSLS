#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//#include "ofxFlowTools.h"
#include "MyFlowTools.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	// Camera
	ofVideoGrabber		simpleCam;
	bool				didCamUpdate;
	ftFbo				cameraFbo;
	ofParameter<bool>	doFlipCamera;
	
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
	
	
	
	// test
	
	ofImage				imgTest1;
	bool				drawTestImage = false;
	bool				drawBackImg = true;
	
	
	ofxPanel			gui;
	
	
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
