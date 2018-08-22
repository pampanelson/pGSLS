#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	
	
	
	drawWidth = 1550;
	halfDrawWidth = 640;
	
	drawHeight = 512;
	

	// setup k1
	kinect1.setRegistration(true);
	//	kinect1.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	kinect1.init(false, false); // disable video image (faster fps)
	
	kinect1.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect1.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect1.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect1.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect1.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect1.getZeroPlaneDistance() << "mm";
	}
	
	
	//	colorImg.allocate(kinect1.width, kinect1.height);
	k1GrayImage.allocate(kinect1.width, kinect1.height,OF_IMAGE_GRAYSCALE);
	k1GrayImageThreshNear.allocate(kinect1.width, kinect1.height,OF_IMAGE_GRAYSCALE);
	k1GrayImageThreshFar.allocate(kinect1.width, kinect1.height,OF_IMAGE_GRAYSCALE);
	
	
	
	
	// gray background for debug
	//	ofSetBackgroundColor(128, 128, 128);
	
	
	// white background color
	ofSetBackgroundColor(255, 255, 255);
	
	
	// init shufa
	shufaImg1.load("shufa1.png");
	shufaImg1.resize(640, 512);
	
	
	// init camera
//	simpleCam.setup(640, 480, true);
//
//	cameraFbo.allocate(640, 480);
//	cameraFbo.black();
//
	// init for opencv
//	imitate(shufaDiff,shufaImg1);
//	imitate(overlap,shufaImg1);
//
//	// init for diff
//	imitate(previous, shufaImg1);
//	imitate(diff, shufaImg1);
//
	
//	shufaDiff.allocate(drawWidth, drawHeight, OF_IMAGE_COLOR);
//	overlap.allocate(drawWidth, drawHeight, OF_IMAGE_COLOR);


	// init sound analyzer ================================
	// check all device
	soundStream.printDeviceList();
	// setup soundflower as device
	
	ofSoundStreamSettings settings;
	
	// if you want to set the device id to be different than the default
//	auto devices = soundStream.getDeviceList();
//	settings.device = devices[0];
	
	// you can also get devices for an specific api
	// auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE);
	
	// or get the default device for an specific api:
	// settings.api = ofSoundDevice::Api::PULSE;
	
	// or by name
	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setInDevice(devices[0]);
	}

	
//	for (int i = 0; i < devices.size(); i++) {
	//		cout << "devices  : " << devices[i].name << endl;
//	}
	
	
	int sampleRate = 44100;
	int bufferSize = 512;
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	
	
	settings.setInListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);
	
	
	
	
	
	
	
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
		f->getFluidSimulation().setDissipation(defaultDissipation.get());
		vecMyFlowTools.push_back(f);
	}
	
	
	
	flowFbo.allocate(drawWidth,drawHeight);
	obsticleFbo.allocate(drawWidth,drawHeight);
	
	
	
	// syphon
	syphonFbo.allocate(drawWidth,drawHeight,GL_RGBA);
	syphonServer.setName("pGSLS");
	
	
	
	gui.setup("settings");
	
	// kinect
	// ...............  kinect
	gui.add(bThreshWithOpenCV.set("thresh with opencv",false));
	gui.add(k1GrayThreshNear.set("kinect1 near",0,1,255));
	gui.add(k1GrayThreshFar.set("kinect1 far",0,1,255));
	gui.add(k1Angle.set("kinect1 angle",0,2,90));
	
	
	// opencv gui
	gui.add(threshold.set("Threshold", 128, 0, 255));
	gui.add(fluidGravityY.set("fluid gravity y", 0, 1, 1500));
	gui.add(diffThreshold.set("diff Threshold", 0, 1, 60));
	gui.add(defaultDissipation.set("default  dissipation",0,0.0001,1));
	gui.add(rmsFactorForDissipation.set("rms factor  dissipation", 100,10, 1000));
	gui.add(rmsDamperForDissipation.set("rms damper  dissipation", 0,0.001,1));
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
	
	// kinect1
	kinect1.setCameraTiltAngle(k1Angle.get());
	kinect1.update();
	
	// there is a new frame and we are connected
	if(kinect1.isFrameNew()) {
		
		// load grayscale depth image from the kinect1 source
		k1GrayImage.setFromPixels(kinect1.getDepthPixels());
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
//		if(bThreshWithOpenCV) {
//			k1GrayImageThreshNear = k1GrayImage;
//			k1GrayImageThreshFar = k1GrayImage;
//			k1GrayImageThreshNear.threshold(k1GrayThreshNear.get(), true);
//			k1GrayImageThreshFar.threshold(k1GrayThreshFar.get());
//			cvAnd(k1GrayImageThreshNear.getCvImage(), k1GrayImageThreshFar.getCvImage(), k1GrayImage.getCvImage(), NULL);
//
//		} else {
		
			// or we do it ourselves - show people how they can work with the pixels
			ofPixels & pix = k1GrayImage.getPixels();
			int numPixels = pix.size();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < k1GrayThreshNear.get() && pix[i] > k1GrayThreshFar.get()) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
//		}
		
		// update the cv images
//		k1GrayImage.flagImageChanged();
	}
	
	k1GrayImage.resize(drawWidth, drawHeight);
	k1GrayImage.update();


	
	
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
	
	
//	simpleCam.update();
////	if (bFlipCamera.get()){
////		flip(simpleCam, simpleCam, 1);
////	}  // Flip Horizontal
////
//		// take the absolute difference of prev and cam and save it inside diff
//		absdiff(simpleCam, previous, diff);
//		diff.update();
//
//		// like ofSetPixels, but more concise and cross-toolkit
//		copy(simpleCam, previous);
//
//		diffMean = mean(toCv(diff));
//
//		diffValue = diffMean[0] + diffMean[1] + diffMean[2];
//
//		diffValue = diffValue / 3;
//	////	cout << diffValue << endl;
//
//
//	// whne diff great then a threshold to update fbo for flow
//	if (diffValue > diffThreshold.get()) {
//		absdiff(shufaImg1,simpleCam,shufaDiff);
//		shufaDiff.update();
//
//		subtract(shufaImg1,shufaDiff, overlap);
//		overlap.update();
//	}
//


		// take the absolute difference of prev and cam and save it inside diff
//		absdiff(k1GrayImage, previous, diff);
//		diff.update();

		// like ofSetPixels, but more concise and cross-toolkit
//		copy(k1GrayImage, previous);

//		diffMean = mean(toCv(diff));
//
//		diffValue = diffMean[0] + diffMean[1] + diffMean[2];
//
//		diffValue = diffValue / 3;
//	////	cout << diffValue << endl;


	// whne diff great then a threshold to update fbo for flow
//	if (diffValue > diffThreshold.get()) {
//		absdiff(shufaImg1,k1GrayImage,shufaDiff);
//		shufaDiff.update();
//
//		subtract(shufaImg1,shufaDiff, overlap);
//		overlap.update();
//	}


	
	
	
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
	
//	overlap.draw(0, 0,drawWidth,drawHeight);
	
	// use kinect but minus from fonts --------------------------
	k1GrayImage.draw(0, 0,halfDrawWidth,drawHeight);
	
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
		
		
		vecMyFlowTools[i]->getFluidSimulation().setGravity(ofVec2f(0.0,fluidGravityY.get()));

		float dissipationDelta = rms / rmsFactorForDissipation.get() - rmsDamperForDissipation.get();
		
		if(dissipationDelta < 0){
			dissipationDelta = 0;
		}
		
		
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
	
	
	
	k1GrayImage.draw(0,0);
//	shufaDiff.draw(640, 0);
//	overlap.draw(0, 0);
	
//		overlap.draw(0,0);
	if(bDrawGui){
		
		gui.draw();
		for (int i = 0; i < vecMyFlowTools.size(); i++) {
			vecMyFlowTools[i]->drawGui();
		}
		
		
	}
	
	showAudioDebug();
}

//--------------------------------------------------------------

void ofApp::exit(){
	
	myTimer.stopThread();
	ofSoundStreamStop();
	
	kinect1.setCameraTiltAngle(0); // zero the tilt on exit
	kinect1.close();
	
	
}

//--------------------------------------------------------------

void ofApp::audioIn(ofSoundBuffer & input){
	//void ofApp::audioIn(float *input, int bufferSize, int nChannels)
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
	
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (size_t i = 0; i < input.getNumFrames(); i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
		
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	rms = curVol;
	
//	input.getChannel(chennalBuffer, 0);
//	fft->setSignal(chennalBuffer.getBuffer().data());
//	float* curFft = fft->getAmplitude();
//
//	fftData = curFft;
//
}


//--------------------------------------------------------------

void ofApp::showAudioDebug(){
	//audio analyse debug ===========================
	int yPos = 500;
	
//	if(fftData){
//		int fftBinScale = 500;
//		int fftBinWidth = 2;
//		for (int i = 0; i < fft->getBinSize(); i++) {
//			ofSetColor(0,255,0,255);
//			ofDrawRectangle(fftBinWidth * i, yPos, fftBinWidth, - 1 * fftData[i] * fftBinScale);
//		}
//
//	}
	int rmsScale = 500;
	ofSetColor(0,255,0,255);
	ofDrawRectangle(10, yPos + 10, rmsScale * rms, 20);
	
	ofSetColor(255, 255, 255);
	
	
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






//backup


// kinect
// enable depth->video image calibration
//kinect.setRegistration(true);
//
//kinect.init();
////kinect.init(true); // shows infrared instead of RGB video image
////kinect.init(false, false); // disable video image (faster fps)
//
//kinect.open();		// opens first available kinect
////kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
////kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
//
//// print the intrinsic IR sensor values
//if(kinect.isConnected()) {
//	ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
//	ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
//	ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
//	ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
//}
//
//
//colorImg.allocate(kinect.width, kinect.height);
//grayImage.allocate(kinect.width, kinect.height);
//grayThreshNear.allocate(kinect.width, kinect.height);
//grayThreshFar.allocate(kinect.width, kinect.height);
//
//nearThreshold = 245;
//farThreshold = 213;
//bThreshWithOpenCV = true;
//
//angle = 0;
//kinect.setCameraTiltAngle(angle);

//kinect.update();
//
//// there is a new frame and we are connected
//if(kinect.isFrameNew()) {
//
//	// load grayscale depth image from the kinect source
//	grayImage.setFromPixels(kinect.getDepthPixels());
//
//	// we do two thresholds - one for the far plane and one for the near plane
//	// we then do a cvAnd to get the pixels which are a union of the two thresholds
//	if(bThreshWithOpenCV) {
//		grayThreshNear = grayImage;
//		grayThreshFar = grayImage;
//		grayThreshNear.threshold(nearThreshold, true);
//		grayThreshFar.threshold(farThreshold);
//		cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
//	} else {
//
//		// or we do it ourselves - show people how they can work with the pixels
//		ofPixels & pix = grayImage.getPixels();
//		int numPixels = pix.size();
//		for(int i = 0; i < numPixels; i++) {
//			if(pix[i] < nearThreshold && pix[i] > farThreshold) {
//				pix[i] = 255;
//			} else {
//				pix[i] = 0;
//			}
//		}
//	}
//
//	// update the cv images
//	grayImage.flagImageChanged();
//	//		Canny(grayImage, grayImage, 100, 200, 3);
//
//
//	// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
//	// also, find holes is set to true so we will get interior contours as well....
//	contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20,
//							   // if use approximate points
//							   //								   true
//							   false
//							   );
//	//		for(int i = 0; i < contourFinder.blobs.at(0).nPts; i++) {
//
//	//		cout << contourFinder.blobs.at(0).pts.at(0)[0] << endl;
//	//		cout << contourFinder.blobs.at(0).pts.at(0)[1] << endl;
//
//	//		}
//
//}

