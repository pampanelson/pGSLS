#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	
	
	
	drawWidth = 640;
//	halfDrawWidth = 640;
	
	drawHeight = 512;

	
	// white background color
	ofSetBackgroundColor(255, 255, 255);
	

	
	// init camera
	simpleCam.setup(640, 480, true);

	cameraFbo.allocate(640, 480);
	cameraFbo.black();


	
	
	//    flowWidth = drawWidth;
	//    flowHeight = drawHeight;
	
	
	float ratio = 2.0;
	int flowToolsNum = 1;
	for(int i = 0;i < flowToolsNum;i++){
		MyFlowTools * f = new MyFlowTools();
		f->setup(drawWidth, drawHeight, ratio, "myflow_" + ofToString(i));
//		f->setFlowColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));// for debug only
		
		f->particleFlow.activate(false);
//		f->getFluidSimulation().setGravity(ofVec2f(0.0,9.8));

		vecMyFlowTools.push_back(f);
	}
	
	
	
	flowFbo.allocate(drawWidth,drawHeight);
	obsticleFbo.allocate(drawWidth,drawHeight);
	
	
	
	// syphon
	syphonFbo.allocate(drawWidth,drawHeight,GL_RGBA);
	syphonServer.setName("pGSLS");
	
	
	
	gui.setup("settings");
	gui.add(bFlipCamera.set("flip camera",false));
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	
	
	
	
	
	// simplecam ==========================================
	
	
	simpleCam.update();


	
	// flow tools =====================================
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	flowFbo.begin();
	if (bFlipCamera.get()){
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

//		vecMyFlowTools[i]->getFluidSimulation().setSpeed(20.0 + diffValue);
//		float dissipation = dissipationTopValue.get() - diffValue / diffValueFactorForDissipation.get();
//
//		if(dissipation < 0){
//			dissipation = 0;
//		}
//		vecMyFlowTools[i]->getFluidSimulation().setDissipation(dissipation);
		
//
//		vecMyFlowTools[i]->getFluidSimulation().setGravity(ofVec2f(0.0,fluidGravityY.get()));
//
//		float dissipationDelta = rms / rmsFactorForDissipation.get() - rmsDamperForDissipation.get();
//
//		if(dissipationDelta < 0){
//			dissipationDelta = 0;
//		}
		
		
//		vecMyFlowTools[i]->getFluidSimulation().setDissipation(defaultDissipation.get() + dissipationDelta);
		vecMyFlowTools[i]->update(&flowFbo, &obsticleFbo);

	}

	
	
	
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	syphonFbo.begin();
	ofClear(255, 255, 255);
	
	
	for (int i = 0; i < vecMyFlowTools.size(); i++) {
		vecMyFlowTools[i]->drawColorFlow();
	}
	
	syphonFbo.end();
	
	
	syphonServer.publishTexture(&syphonFbo.getTexture());
	
	
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
	ofSoundStreamStop();
	
	
	
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


