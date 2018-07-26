#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	// white background color
	ofSetBackgroundColor(128, 128, 128);
//	ofSetBackgroundColor(255, 255, 255);
	
	fbo.allocate(ofGetWindowWidth(),ofGetWindowHeight(),GL_RGBA);
	
	
	imgTest1.load("test-2.jpg");
	
	
	imgTest3.load("test-3.jpg");
	
	// init opencv
	contourFinder.setMinAreaRadius(5);
	contourFinder.setMaxAreaRadius(150);
	contourFinder.setInvert(true); // find black instead of white
	
	
	
	
	// init camera
	//	simpleCam.setup(640, 480, true);
	//	didCamUpdate = false;
	//	cameraFbo.allocate(640, 480);
	//	cameraFbo.black();
	
	drawWidth = ofGetWindowWidth();
	drawHeight = ofGetWindowHeight();
	//    flowWidth = drawWidth;
	//    flowHeight = drawHeight;
	
	ratio = 1;
	
	myFlowTools.setup(drawWidth, drawHeight, ratio,"myFlow");
	flowFbo.allocate(drawWidth,drawHeight);
	obsticleFbo.allocate(drawWidth,drawHeight);
	
	
	
	gui.setup("settings");
	
	// opencv gui
	gui.add(threshold.set("Threshold", 128, 0, 255));
	gui.add(trackHs.set("Track Hue/Saturation", false));
	
	
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	
	// opencv
	contourFinder.setTargetColor(targetColor, trackHs ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
	contourFinder.setThreshold(threshold);
	contourFinder.findContours(imgTest3);
	
	
	//	simpleCam.update();
	//		if (doFlipCamera){
	//			simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());
	//
	//		}  // Flip Horizontal
	//		else{
	//			simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
	//
	//		}
	
	
	
	
	// flow tools =====================================
	
//	ofPushStyle();
//	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
//	flowFbo.begin();
//
//	if(drawTestImage){
//		imgTest1.draw(0, sin(ofGetElapsedTimeMillis()));
//	}
//
//
//
//	flowFbo.end();
//	ofPopStyle();
//
	//	myFlowTools.update(&flowFbo,&obsticleFbo);
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
//	if(drawBackImg){
//		imgTest1.draw(0,0);
//	}
//	fbo.begin();
//	ofClear(0, 0, 0);
//	myFlowTools.draw();
//	fbo.end();
//
//	fbo.readToPixels(fboPixels);
//	for (int i = 0; i < fboPixels.getWidth(); i++) {
//		for (int j = 0; j < fboPixels.getHeight(); j++) {
//			ofColor color = fboPixels.getColor(i, j);
//			if(color[0] > 0){
//				fboPixels.setColor(i, j,ofColor(0,0,0,color[3]));
//
//			}
//		}
//	}
//
//	fboImg.setFromPixels(fboPixels);
//	fboImg.draw(0, 0);
	
	
//	imgTest3.draw(0, 0);
	// opencv
	contourFinder.draw();
	
	gui.draw();
	
	myFlowTools.drawGui();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 'd':
			drawTestImage = true;
			drawBackImg = false;
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
		case 'e':
			drawTestImage = false;
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	
}
