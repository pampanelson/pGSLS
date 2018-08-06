#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	
	// gray background for debug
	//	ofSetBackgroundColor(128, 128, 128);
	
	
	// white background color
	ofSetBackgroundColor(255, 255, 255);
	
	fbo.allocate(ofGetWindowWidth(),ofGetWindowHeight(),GL_RGBA);
	
	
	//	imgTest1.load("test-2.jpg");
	
	
	
	
	
	
	
	//init camera
	simpleCam.setup(640, 480, true);
	didCamUpdate = false;
	cameraFbo.allocate(640, 480);
	cameraFbo.black();
	
	drawWidth = ofGetWindowWidth();
	drawHeight = ofGetWindowHeight();
	//    flowWidth = drawWidth;
	//    flowHeight = drawHeight;
	
	ratio = 1;
	
	myFlowTools.setup(drawWidth, drawHeight, ratio,"myFlow1");
	flowFbo.allocate(drawWidth,drawHeight);
	obsticleFbo.allocate(drawWidth,drawHeight);
	
	// set color for myflowtools
	myFlowTools.setFinalColor(ofColor(255,0,0));
	
	gui.setup("settings");
	
	// opencv gui
	gui.add(threshold.set("Threshold", 128, 0, 255));
	gui.add(trackHs.set("Track Hue/Saturation", false));
	gui.add(blackWhiteThreshold.set("black white threshold",0,1,255));
	gui.add(rmsThreshold.set("rms threshold",0,0.02,1));
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	// use camera to make flow for test ===============
	//	ofPushStyle();
	//	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	flowFbo.begin();
	ofClear(0, 0, 0);
	
	simpleCam.update();
	if (doFlipCamera){
		simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());
		
	}  // Flip Horizontal
	else{
		simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
		
	}
	
	
	flowFbo.end();
	
	ofPopStyle();
	//
	
	
	
	
	// prepare back image below flow image ====================
	//  draw current contourindex contour blobs white
	
	
	
	
	
	
	
	
	// flow tools =====================================
	

//	flowFbo.begin();
//	//		if(drawTestImage){
//	//			imgTest1.draw(0, sin(ofGetElapsedTimeMillis()));
//	//		}
//	//
//
//	flowFbo.end();
//
//	ofPopStyle();
//
	
	
	
	myFlowTools.update(&flowFbo,&obsticleFbo);
	
	
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	
	
	//	if(drawBackImg){
	//		imgTest1.draw(0,0);
	//	}
	
	
	myFlowTools.draw();
	
	//		imgTest3.draw(0, 0);
	
	
	
	// opencv
	//	contourFinder.draw();
	
	if(bDrawGui){
		
		gui.draw();
		
		myFlowTools.drawGui();
		
		
	}
	
}

//--------------------------------------------------------------

void ofApp::exit(){
	// audio analyzer ================
	ofSoundStreamStop();
	//	powerThresholdSlider.removeListener(this, &ofApp::powerThresholdChanged);
	
	myTimer.stopThread();
}





//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
			
		case 'g':
			bDrawGui = !bDrawGui;
			break;
			
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
		case 'e':
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
