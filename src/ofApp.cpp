#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	
	
	// kinect
	// enable depth->video image calibration
	kinect.setRegistration(true);
	
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 245;
	farThreshold = 213;
	bThreshWithOpenCV = true;
	
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	
	
	
	// gray background for debug
	//	ofSetBackgroundColor(128, 128, 128);
	
	
	// white background color
	ofSetBackgroundColor(255, 255, 255);
	
//	fbo.allocate(ofGetWindowWidth(),ofGetWindowHeight(),GL_RGBA);
	
	// init shufa
	shufaImg1.load("shufa1.png");
	shufaImg1.resize(640, 480);
	
	
	// init camera
	simpleCam.setup(640, 480, true);
	
	cameraFbo.allocate(640, 480);
	cameraFbo.black();
	
	// init for opencv
	imitate(shufaDiff,simpleCam);
	imitate(overlap,simpleCam);
	
	// init for diff
	imitate(previous, simpleCam);
	imitate(diff, simpleCam);
	
	
	drawWidth = 800;
	drawHeight = 600;
	
	//    flowWidth = drawWidth;
	//    flowHeight = drawHeight;
	
	
	float ratio = 2.0;
	int flowToolsNum = 1;
	for(int i = 0;i < flowToolsNum;i++){
		MyFlowTools * f = new MyFlowTools();
		f->setup(drawWidth, drawHeight, ratio, "myflow_" + ofToString(i));
		f->setFlowColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));// for debug only
		
		f->particleFlow.activate(false);
		vecMyFlowTools.push_back(f);
	}
	
	
	
	flowFbo.allocate(drawWidth,drawHeight);
	obsticleFbo.allocate(drawWidth,drawHeight);
	
	
	
	gui.setup("settings");
	
	// opencv gui
	gui.add(threshold.set("Threshold", 128, 0, 255));
	gui.add(diffThreshold.set("diff Threshold", 0, 1, 60));
	gui.add(diffValueFactorForDissipation.set("diff dissipation", 1, 10, 1000));
	gui.add(dissipationTopValue.set("dissipation top value", 0.001, 0.0001, 0.1));

	gui.add(trackHs.set("Track Hue/Saturation", false));
	gui.add(bFlipCamera.set("flip camera", true));
	gui.add(blackWhiteThreshold.set("black white threshold",0,1,255));
	gui.add(rmsThreshold.set("rms threshold",0,0.02,1));
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	
//	kinect.update();
//
//	// there is a new frame and we are connected
//	if(kinect.isFrameNew()) {
//
//		// load grayscale depth image from the kinect source
//		grayImage.setFromPixels(kinect.getDepthPixels());
//
//		// we do two thresholds - one for the far plane and one for the near plane
//		// we then do a cvAnd to get the pixels which are a union of the two thresholds
//		if(bThreshWithOpenCV) {
//			grayThreshNear = grayImage;
//			grayThreshFar = grayImage;
//			grayThreshNear.threshold(nearThreshold, true);
//			grayThreshFar.threshold(farThreshold);
//			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
//		} else {
//
//			// or we do it ourselves - show people how they can work with the pixels
//			ofPixels & pix = grayImage.getPixels();
//			int numPixels = pix.size();
//			for(int i = 0; i < numPixels; i++) {
//				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
//					pix[i] = 255;
//				} else {
//					pix[i] = 0;
//				}
//			}
//		}
//
//		// update the cv images
//		grayImage.flagImageChanged();
//		//		Canny(grayImage, grayImage, 100, 200, 3);
//
//
//		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
//		// also, find holes is set to true so we will get interior contours as well....
//		contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20,
//								   // if use approximate points
//								   //								   true
//								   false
//								   );
//		//		for(int i = 0; i < contourFinder.blobs.at(0).nPts; i++) {
//
//		//		cout << contourFinder.blobs.at(0).pts.at(0)[0] << endl;
//		//		cout << contourFinder.blobs.at(0).pts.at(0)[1] << endl;
//
//		//		}
//
//	}
//
	
	
	
	// if update for flow
	//	if (diffValue >= diffThreshold.get()) {
	//	absdiff(shufaImg1,simpleCam,shufaDiff);
//	ofImage kinectDepthFrame = kinect.getDepthPixels();
//	absdiff(shufaImg1,kinectDepthFrame,shufaDiff);
//	shufaDiff.update();
//
//	subtract(shufaImg1,shufaDiff, overlap);
//	overlap.update();
//
//	float diffDelta = diffValue - diffThreshold.get();
//	//	if(diffDelta < 0){
//	//		diffDelta == 0;
//	//	}
//	//
//
//	//	}
//
//
//
	
	
	
	
	
	
	
	// simplecam ==========================================
	
	
	simpleCam.update();
//	if (bFlipCamera.get()){
//		flip(simpleCam, simpleCam, 1);
//	}  // Flip Horizontal
//
		// take the absolute difference of prev and cam and save it inside diff
		absdiff(simpleCam, previous, diff);
		diff.update();
	
		// like ofSetPixels, but more concise and cross-toolkit
		copy(simpleCam, previous);
	
		diffMean = mean(toCv(diff));
	
		diffValue = diffMean[0] + diffMean[1] + diffMean[2];
	
		diffValue = diffValue / 3;
	////	cout << diffValue << endl;
	
	
	// whne diff great then a threshold to update fbo for flow
	if (diffValue > diffThreshold.get()) {
		absdiff(shufaImg1,simpleCam,shufaDiff);
		shufaDiff.update();
		
		subtract(shufaImg1,shufaDiff, overlap);
		overlap.update();
	}
	


	
	
	
	
	// flow tools =====================================
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	flowFbo.begin();
	//	if (doFlipCamera){
	//		simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());
	//
	//	}  // Flip Horizontal
	//	else{
	//		simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
	//
	//	}
	
	overlap.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
	
	
	flowFbo.end();
	
	ofPopStyle();
	
	
	
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	obsticleFbo.begin();
	//obsticleImg.draw(0, 0);
	obsticleFbo.end();
	ofPopStyle();

	for (int i = 0; i < vecMyFlowTools.size(); i++) {
		
		vecMyFlowTools[i]->getFluidSimulation().setSpeed(20.0 + diffValue);
		float dissipation = dissipationTopValue.get() - diffValue / diffValueFactorForDissipation.get();
		
		if(dissipation < 0){
			dissipation = 0;
		}
		vecMyFlowTools[i]->getFluidSimulation().setDissipation(dissipation);
		vecMyFlowTools[i]->update(&flowFbo, &obsticleFbo);
		
	}
	
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	
	
	
	for (int i = 0; i < vecMyFlowTools.size(); i++) {
		vecMyFlowTools[i]->drawColorFlow();
	}
	
//	kinect.drawDepth(0,0);
//	kinect.draw(0, 0);
	
	//	overlap.draw(0,0);
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
	
//	kinect.setCameraTiltAngle(0); // zero the tilt on exit
//	kinect.close();
	
	
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
			
			
			
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
			;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
			
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
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
