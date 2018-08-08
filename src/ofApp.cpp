#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	
	
	//syphon server
	individualTextureSyphonServer.setName("pGSLS_Syphon_Output");
	syphonFbo.allocate(ofGetWindowWidth(),ofGetWindowHeight(), GL_RGBA);
	


	
	
	// gray background for debug
	//	ofSetBackgroundColor(128, 128, 128);
	
	
	// white background color
	ofSetBackgroundColor(255, 255, 255);
	
	fbo.allocate(ofGetWindowWidth(),ofGetWindowHeight(),GL_RGBA);
	
	
	//	imgTest1.load("test-2.jpg");
	
	
	//	imgTest3.load("test-4.jpg");
	
	
	imgTest3.load("alpha1024x768.jpg");
	
	// init opencv
	contourFinder.setMinAreaRadius(5);
	contourFinder.setMaxAreaRadius(150);
	contourFinder.setInvert(true); // find black instead of white
	
	
	
	// init sound analyzer ================================
	// check all device
	soundStream.printDeviceList();
	// setup soundflower as device
	
	ofSoundStreamSettings settings;
	
	// if you want to set the device id to be different than the default
	//	 auto devices = soundStream.getDeviceList();
	//	 settings.device = devices[0];
	
	// you can also get devices for an specific api
	// auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE);
	// settings.device = devices[0];
	
	// or get the default device for an specific api:
	// settings.api = ofSoundDevice::Api::PULSE;
	
	// or by name
	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setInDevice(devices[0]);
	}
	
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
	
	
	// setup the sound stream
	// setup chennl buffer
	chennalBuffer.allocate(bufferSize, 1);
	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
	
	// fft bin size = bufferSize / 2 + 1 ++++++++++++++++++++++
	//	cout << fft->getBinSize() << endl;
	
	
	
	
	
	
	
	
	
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
	gui.add(blackWhiteThreshold.set("black white threshold",0,1,255));
	gui.add(bShowContour.set("show contour", false));
	gui.add(bContourFinderUpdate.set("contourfinder update", false));
	gui.add(bStartFlow.set("start flow", false));
	gui.add(rmsThreshold.set("rms threshold",0,0.02,1));
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	
	// opencv analyse once for convinient ===================
	contourFinder.setTargetColor(targetColor, trackHs ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
	contourFinder.setThreshold(threshold);
	contourFinder.findContours(imgTest3);
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	// first handle on image
	
	//	ofImage		tmpGrayImg;
	//	tmpGrayImg.allocate(imgTest3.getWidth(), imgTest3.getHeight(), OF_IMAGE_GRAYSCALE);
	//
	//	ofPixels    tmpPixels;
	//
	//	convertColor(imgTest3, tmpGrayImg, CV_RGB2GRAY);
	//
	//	tmpPixels.allocate(imgTest3.getWidth(), imgTest3.getHeight(),3);
	//	for (int i = 0; i < imgTest3.getWidth(); i++) {
	//		for (int j = 0; j < imgTest3.getHeight(); j++) {
	//			ofColor color = tmpGrayImg.getColor(i, j);
	//			if(color[0] > blackWhiteThreshold.get()){
	//				tmpGrayImg.setColor(i, j,ofColor(255,255,255));
	//			}else{
	//				tmpGrayImg.setColor(i, j,ofColor(0,0,0));
	//
	//			}
	//		}
	//	}
	//
	//	imgTest3.setFromPixels(tmpPixels);
	
	
	
	
	
	// opencv
	
	if(bContourFinderUpdate.get()){
		contourFinder.setTargetColor(targetColor, trackHs ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
		contourFinder.setThreshold(threshold);
		contourFinder.findContours(imgTest3);
		
	}
	
	
	
	
	
	//	simpleCam.update();
	//		if (doFlipCamera){
	//			simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());
	//
	//		}  // Flip Horizontal
	//		else{
	//			simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
	//
	//		}
	
	
	
	
	
	
	
	
	if(rms >= rmsThreshold.get() && vecContourBlobImagesForFlow.size() > 0){
		goNextFlow();
	}
	
	
	
	
	
	// prepare back image below flow image ====================
	//  draw current contourindex contour blobs white
	
	ofPixels tmpPixels = imgTest3.getPixels();
	
	
	// according contour finder
	//	if(contourIndex >= 0 & contourIndex < contourFinder.getContours().size()){
	
	// according vecContourBlobImagesForFlow
	if(contourIndex >= 0 & contourIndex < vecContourBlobImagesForFlow.size()){
		
		
		// thread out error ======================  TODO
		for (int k = 0; k <= contourIndex; k++) {
			if(contourBlobs[k].size() > 0){
				for (int j = 0; j < contourBlobs[k].size(); j++) {
					
					tmpPixels.setColor(contourBlobs[k][j].x, contourBlobs[k][j].y, ofColor(255,255,255));
					
				}
			}
			
			
		}
		
		
	}
	
	backImg.setFromPixels(tmpPixels);
	
	
	
	
	// prepare obsticle image from back image invert
	// invert each pixel for obsticle;
	for (int i = 0; i < tmpPixels.getWidth(); i++) {
		for(int j = 0;j < tmpPixels.getHeight();j++){
			tmpPixels.setColor(i, j,tmpPixels.getColor(i, j).invert());
		}
	}
	
	obsticleImg.setFromPixels(tmpPixels);
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	obsticleFbo.begin();
	obsticleImg.draw(0, 0);
	obsticleFbo.end();
	ofPopStyle();
	
	
	
	
	
	
	
	
	// flow tools =====================================
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	flowFbo.begin();
	//		if(drawTestImage){
	//			imgTest1.draw(0, sin(ofGetElapsedTimeMillis()));
	//		}
	//
	
	
	// debug draw each contour blob for a certain time
	if(contourIndex >= 0 & contourIndex < vecContourBlobImagesForFlow.size()){
		
		int timeStamp = myTimer.counterMil;
		if(vecDrawImageForFlowOnce[contourIndex] == 0){
			vecDrawImageForFlowOnce[contourIndex] = timeStamp;
			
		}
		int delta = timeStamp - vecDrawImageForFlowOnce[contourIndex] + 1;
		if(delta < drawImageForFlowMilSecs){
			//			vecContourBlobImagesForFlow[contourIndex].draw(0,-1 * delta / deltaSpeedFactor);
			
			// - y direction for debug ==============================  TODO
			vecContourBlobImagesForFlow[contourIndex].draw(
														   vecContourBlobImagesBoudingBox[contourIndex].x,
														   vecContourBlobImagesBoudingBox[contourIndex].y -1 * delta / deltaSpeedFactor,
														   vecContourBlobImagesBoudingBox[contourIndex].z,
														   vecContourBlobImagesBoudingBox[contourIndex].w);
			
			
			// -x direction for vertical handwriting ==============
//			vecContourBlobImagesForFlow[contourIndex].draw(vecContourBlobImagesBoudingBox[contourIndex].x - 1 * delta / deltaSpeedFactor,
//														   vecContourBlobImagesBoudingBox[contourIndex].y,
//														   vecContourBlobImagesBoudingBox[contourIndex].z,
//														   vecContourBlobImagesBoudingBox[contourIndex].w);
			
		}
	}
	
	
	flowFbo.end();
	
	ofPopStyle();
	
	
	
	
	myFlowTools.update(&flowFbo,&obsticleFbo);
	
	
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	syphonFbo.begin();
	ofClear(0, 0, 0);
	
	
	//	if(drawBackImg){
	//		imgTest1.draw(0,0);
	//	}
	
	
	
	// draw image below flow
	backImg.draw(0, 0);
	
	
	// draw flow ============================================
	fbo.begin();
	ofClear(0, 0, 0);
	myFlowTools.draw();
	fbo.end();
	
	fbo.readToPixels(fboPixels);
	for (int i = 0; i < fboPixels.getWidth(); i++) {
		for (int j = 0; j < fboPixels.getHeight(); j++) {
			ofColor color = fboPixels.getColor(i, j);
			if(color[0] > 0){
				fboPixels.setColor(i, j,ofColor(0,0,0,color[4]));
				
			}
		}
	}
	
	fboImg.setFromPixels(fboPixels);
	fboImg.draw(0, 0);
	
	
	//		imgTest3.draw(0, 0);
	
	
	
	
	
	
	
	// opencv
	
	if(bShowContour.get()){
		ofSetColor(255, 0, 0);
		contourFinder.draw();
	}
	
	
	
	syphonFbo.end();
	
	individualTextureSyphonServer.publishTexture(&syphonFbo.getTexture());
	
	
	
	
	if(bDrawGui){
		
		gui.draw();
		
		myFlowTools.drawGui();
		
		showAudioDebug();
		
	}
	
}

//--------------------------------------------------------------

void ofApp::exit(){
	// audio analyzer ================
	ofSoundStreamStop();
	//	powerThresholdSlider.removeListener(this, &ofApp::powerThresholdChanged);
	
	myTimer.stopThread();
}


void ofApp::showAudioDebug(){
	//audio analyse debug ===========================
	int yPos = 500;
	
	if(fftData){
		int fftBinScale = 500;
		int fftBinWidth = 2;
		for (int i = 0; i < fft->getBinSize(); i++) {
			ofSetColor(0,255,0,255);
			ofDrawRectangle(fftBinWidth * i, yPos, fftBinWidth, - 1 * fftData[i] * fftBinScale);
		}
		
	}
	int rmsScale = 500;
	ofSetColor(0,255,0,255);
	ofDrawRectangle(10, yPos + 10, rmsScale * rms, 20);
	
	ofSetColor(255, 255, 255);
	
	
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
	
	input.getChannel(chennalBuffer, 0);
	fft->setSignal(chennalBuffer.getBuffer().data());
	float* curFft = fft->getAmplitude();
	
	fftData = curFft;
	
}

void ofApp::goNextFlow(){
	
	if(bStartFlow.get()){
		if(contourIndex >= vecContourBlobImagesForFlow.size()){
			contourIndex = -1;
			
			// reset draw timestamp
			for (int i = 0; i < vecDrawImageForFlowOnce.size(); i++) {
				vecDrawImageForFlowOnce[i] = 0;
			}
			
			
		}
		contourIndex += 1;
		
	}
	
	
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
			
		case 'g':
			bDrawGui = !bDrawGui;
			break;
		case 'd':
			drawTestImage = true;
			drawBackImg = false;
			break;
			
			// for debug test only ==================
		case 'c':
			goNextFlow();
			break;
			
			
		case 'b':
			
			// stop update contourfinder for effeciency 
			bContourFinderUpdate.set(false);
			
			contourIndex = -1;
			contourBlobs.clear();
			
			for (int i = 0; i < contourFinder.getContours().size(); i++) {
				vector<cv::Point> curBlob;
				
				// collect contour finder points  ================
				vector<cv::Point> points = contourFinder.getContour(i);
				
				for (int j = 0; j < points.size(); j++) {
					cv::Point point = points[j];
					
					curBlob.push_back(point);
					
				}
				
				
				
				
				
				
				
				// collect all point inside current contour polyline
				ofPolyline polyLine = contourFinder.getPolyline(i);
				
				for (int j = 0; j < imgTest3.getWidth(); j++) {
					for (int k = 0; k < imgTest3.getHeight(); k++) {
						cv::Point p = cv::Point(j,k);
						if(polyLine.inside(j, k)){
							curBlob.push_back(p);
						}
						
					}
				}
				
				
				
				// collect curBlob
				
				contourBlobs.push_back(curBlob);
				
				
				// get bouding box data left point position x y and width and height;
				ofVec4f boudingBox;
				boudingBox.set(contourFinder.getBoundingRect(i).x, contourFinder.getBoundingRect(i).y, contourFinder.getBoundingRect(i).width, contourFinder.getBoundingRect(i).height);
				
				
				// make a image with blob points and collect
				ofImage img;
				// make a img for contour as sub image
				
				
				img.allocate(boudingBox.z,boudingBox.w,OF_IMAGE_GRAYSCALE);
				
				//				img.allocate(imgTest3.getWidth(), imgTest3.getHeight(), OF_IMAGE_GRAYSCALE);
				ofPixels pixels;
				//				pixels.allocate(imgTest3.getWidth(), imgTest3.getHeight(), 1);
				pixels.allocate(boudingBox.z,boudingBox.w,1);
				pixels.setImageType(OF_IMAGE_GRAYSCALE);
				
				// draw all white background
				for (int i = 0; i < pixels.size(); i++) {
					pixels.setColor(i,ofColor(255));
				}
				
				// draw black point for contour blob
				for (int i = 0; i < curBlob.size(); i++) {
					
					//					pixels.setColor(curBlob[i].x, curBlob[i].y, ofColor(0));
					
					// move point according contour finder target image to saperate image (0,0)
					pixels.setColor(curBlob[i].x - boudingBox.x, curBlob[i].y - boudingBox.y, ofColor(0));
				}
				
				img.setFromPixels(pixels);
				
				vecContourBlobImagesForFlow.push_back(img);
				
				
				// draw once mark for this contour blob
				// if == 0 , not draw yet
				// else keep the timestamp of drawing
				// and stop draw after 1000 mil later
				vecDrawImageForFlowOnce.push_back(0);
				
				
				// save bouding box data to draw sub section but whole image
				vecContourBlobImagesBoudingBox.push_back(boudingBox);
			}// end iterate contourfinder
			
			
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
