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
	
	
	
	// init camera
	simpleCam.setup(640, 480, true);
	didCamUpdate = false;
	cameraFbo.allocate(640, 480);
	cameraFbo.black();
	
	drawWidth = ofGetWindowWidth();
	drawHeight = ofGetWindowHeight();
	//    flowWidth = drawWidth;
	//    flowHeight = drawHeight;
	
	
	float ratio = 1.0;
	float dissipation = 0.005;
	for(int i = 0;i < 9;i++){
		MyFlowTools * f = new MyFlowTools();
		f->setup(drawWidth, drawHeight, ratio, "myflow");
		f->setFlowColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
		// set fluid parameters =====================
//		f->getFluidSimulation().setDissipation(dissipation);
		vecMyFlowTools.push_back(f);
	}
//	f1.setup(drawWidth, drawHeight, ratio, "myflow");
//	f1.setFlowColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
//	vecMyFlowTools.push_back(f1);
//	
//	f2.setup(drawWidth, drawHeight, ratio, "myflow");
//	f2.setFlowColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
//	vecMyFlowTools.push_back(f2);
//	
//	f3.setup(drawWidth, drawHeight, ratio, "myflow");
//	f3.setFlowColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
//	vecMyFlowTools.push_back(f3);
	
	
	flowFbo.allocate(drawWidth,drawHeight);
	obsticleFbo.allocate(drawWidth,drawHeight);
	
	
	
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
	
	
	
	
	
	
	
	simpleCam.update();
	//	if (doFlipCamera){
	//		simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());
	//
	//	}  // Flip Horizontal
	//	else{
	//		simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
	//
	//	}
	//
	
	
	
	
	// flow tools =====================================
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	flowFbo.begin();
	if (doFlipCamera){
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
	
	
	
	curFlowIndex = int(ofGetElapsedTimef()) % vecMyFlowTools.size();
	
	for(int i = 0;i < vecMyFlowTools.size();i++){
		if(i == curFlowIndex){
			vecMyFlowTools[i]->update(&flowFbo, &obsticleFbo);

		}else{
			vecMyFlowTools[i]->update(&obsticleFbo, &obsticleFbo);
		}
	}
	
	
	
//	for (int i = 0; i < vecMyFlowTools.size(); i++) {
//
//		// need delete/release reource very carefully ++++++++++++++++++++++++++++++
//		if(!vecMyFlowTools[i].isActive()){
//			//			vecMyFlowTools.erase(vecMyFlowTools.begin() + i);
//
//		}
//		vecMyFlowTools[i].update(&flowFbo, &obsticleFbo);
//	}
//
//
//	flowTools1.update(&flowFbo,&obsticleFbo);
	
	
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	
	
	//	if(drawBackImg){
	//		imgTest1.draw(0,0);
	//	}
	
	
	for (int i = 0; i < vecMyFlowTools.size(); i++) {
		vecMyFlowTools[i]->drawColorFlow();
	}
	
	
	if(bDrawGui){
		
		gui.draw();
		
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
