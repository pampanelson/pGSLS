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
	
//	fbo.allocate(ofGetWindowWidth(),ofGetWindowHeight(),GL_RGBA);
	
	
	
	// init camera
	simpleCam.setup(640, 480, true);
	didCamUpdate = false;
	cameraFbo.allocate(640, 480);
	cameraFbo.black();
	
//	drawWidth = ofGetWindowWidth();
//	drawHeight = ofGetWindowHeight();
	
	drawWidth = 1024;
	drawHeight = 768;
	
	// opencv
	imitate(previous, simpleCam);
	imitate(diff, simpleCam);
	
	
	
	
	// init parameter for flow tools
	float ratio = 4.0; //
//	float dissipation = 0.05;
//	float speed = 70.0;
//	ofVec2f gravity = ofVec2f(0.0,0.0);
//	float viscocity = 1.0;
	
	for(int i = 0;i < 3;i++){
		MyFlowTools * f = new MyFlowTools();
		f->setup(drawWidth, drawHeight, ratio, "myflow_" + ofToString(i));
		f->setFlowColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
		// set fluid parameters =====================
//		f->getFluidSimulation().setDissipation(dissipation);
//		f->getFluidSimulation().setSpeed(speed);
//		f->getFluidSimulation().setGravity(gravity);
//		f->getFluidSimulation().setViscosity(viscocity);
		vecMyFlowTools.push_back(f);
	}
	
	flowFbo.allocate(drawWidth,drawHeight);
	obsticleFbo.allocate(drawWidth,drawHeight);
	
	
	
	// syphon
	
	syphonServerName = "pGSLS";
	syphonServer.setName(syphonServerName);
	syphonFbo.allocate(drawWidth, drawHeight);
	
	
	
	gui.setup("settings");
	
	// opencv gui
	gui.add(threshold.set("Threshold", 128, 0, 255));
	gui.add(bFlipCam.set("flip cam", true));
	gui.add(blackWhiteThreshold.set("black white threshold",0,1,255));
	gui.add(rmsThreshold.set("rms threshold",0,0.02,1));
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	
	
	
	
	
	
	simpleCam.update();

	// opencv diff
	// take the absolute difference of prev and cam and save it inside diff
	absdiff(simpleCam, previous, diff);
	diff.update();
	
	// like ofSetPixels, but more concise and cross-toolkit
	copy(simpleCam, previous);
	
	
	
	
	// flow tools =====================================

	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	flowFbo.begin();
	if (bFlipCam.get()){
		simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());

	}  // Flip Horizontal
	else{
		simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());

	}
	flowFbo.end();
	ofPopStyle();




	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	obsticleFbo.begin();
	//obsticleImg.draw(0, 0);
	obsticleFbo.end();
	ofPopStyle();


	for (int i = 0; i < vecMyFlowTools.size(); i++) {
		vecMyFlowTools[i]->update(&flowFbo, &obsticleFbo);
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	syphonFbo.begin();
//	ofColor(0,0,0);
	
	for (int i = 0; i < vecMyFlowTools.size(); i++) {
		vecMyFlowTools[i]->drawColorFlow();
	}
	
	syphonFbo.end();
	syphonServer.publishTexture(&syphonFbo.getTexture());
	
	diff.draw(0, 0);
	
	if(bDrawGui){
		
		gui.draw();
		for (int i = 0; i < vecMyFlowTools.size(); i++) {
			vecMyFlowTools[i]->drawGui();
		}
		
	}
	
}

//--------------------------------------------------------------

void ofApp::exit(){
	
	myTimer.stopThread();
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
			
		case 'g':
			bDrawGui = !bDrawGui;
			break;
		case 'd':
			
			break;
		case 'c':
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
