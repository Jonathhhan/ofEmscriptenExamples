#include "ofApp.h"
#include "emscripten.h"
#include "emscripten/bind.h"

ofEvent<std::string> loadAudioUrlEvent; 	
ofEvent<std::string> loadVideoUrlEvent; 
ofEvent<void> loadImageEvent; 


void loadAudioUrl(const std::string & url) {
	std::string urlCopy = url;         
	loadAudioUrlEvent.notify(urlCopy); 
} 

void loadVideoUrl(const std::string & url) {
	std::string urlCopy = url;         
	loadVideoUrlEvent.notify(urlCopy); 
} 

void loadImage() {        
	loadImageEvent.notify(); 
} 

EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("loadAudioUrl", & loadAudioUrl);
	emscripten::function("loadVideoUrl", & loadVideoUrl);
	emscripten::function("loadImage", & loadImage);
}

void ofApp::loadAudioUrlX(std::string & url) {
	audioPlayer.unload();
	audioPlayer.loadUrl(url);
	audioPlayer.setLoop(true);
	audioPlayer.play();
}

void ofApp::loadVideoUrlX(std::string & url) {
	videoPlayer.loadUrl(url);
	videoPlayer.play();
}

void ofApp::loadImageX() {
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen("data/data", "r");
	
	if (pFile==NULL) {fputs ("File error", stderr); ::exit (1);}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc(sizeof(char) * lSize);
	if (buffer == NULL) {fputs("Memory error", stderr); ::exit (2);}

	// copy the file into the buffer:
	result = fread (buffer, 1, lSize, pFile);
	if (result != lSize) {fputs ("Reading error", stderr); ::exit (3);}

	/* the whole file is now loaded in the memory buffer. */

	// terminate
	fclose(pFile);
	EM_ASM(FS.unlink("/data/data"));
	ofBuffer buffer1(buffer, lSize);
	image.load(buffer1);
	ofLog(OF_LOG_NOTICE, "Image buffer size: " + ofToString(lSize));
	free(buffer);
}

//--------------------------------------------------------------
void ofApp::hSlider_1onMousePressed(float & e) {
	label_6.symbol = ofToString(e); 
	audioPlayer.setPosition(e);
	videoPlayer.setPosition(e);
}

//--------------------------------------------------------------
void ofApp::hSlider_2onMousePressed(float & e) { 
	audioPlayer.setPan(e);
	videoPlayer.setPan(e);
	label_8.symbol = ofToString(e);
}

//--------------------------------------------------------------
void ofApp::hSlider_3onMousePressed(float & e) { 
	audioPlayer.setVolume(e);
	videoPlayer.setVolume(e);
	label_10.symbol = ofToString(e); 
}   

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e) { 
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.wav, .aiff, .mp3, .mp4, adts., .ogg, .webm, .caf, .flac';
	input.onchange = function(e) {
		var file = e.target.files[0]; 
		var url = URL.createObjectURL(file);
		Module.loadAudioUrl(url);
		};
	input.click();
	);
}   

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e) { 
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.mp4, .webm, .mpeg, .mov, .ogg, .caf, .flac, .h264';
	input.onchange = function(e) {
		var file = e.target.files[0]; 
		var url = URL.createObjectURL(file);
		Module.loadVideoUrl(url);
	};
	input.click();
	);
}   

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool & e) { 
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.apng, .avif, .gif, .jpg, .jpeg, .jpe, .jfif, .png, .svg, .webp, .bmp, .ico, .tif, .tiff';
	input.onchange = function(e) {

		// getting a hold of the file reference
		var file = e.target.files[0]; 

		// setting up the reader
		var reader = new FileReader();
		reader.readAsArrayBuffer(file);

		// here we tell the reader what to do when it's done reading...
		reader.onload = function() {
			var arrayBuffer = reader.result;
			var uint8View = new Uint8Array(arrayBuffer);
			
			FS.createDataFile("/data/", "data", uint8View, true, true);
			Module.loadImage();
		}
	};
	input.click();
	);
}   

//--------------------------------------------------------------
void ofApp::toggle_1onMousePressed(bool & e) { 
	if (e == true && videoPlayer.getTexture() -> isAllocated()) {	
		videoPlayer.setPaused(true);
	} else {
		videoPlayer.setPaused(false);
	}
	if (e == true) {
		ofLog(OF_LOG_NOTICE, "Player paused!");
		ofLog(OF_LOG_NOTICE, "Audio duration: " + ofToString(audioPlayer.getDurationSecs()));
		ofLog(OF_LOG_NOTICE, "Audio position: " + ofToString(audioPlayer.getPosition()));
		ofLog(OF_LOG_NOTICE, "Video duration: " + ofToString(videoPlayer.getDuration()));
		ofLog(OF_LOG_NOTICE, "Video position: " + ofToString(videoPlayer.getPosition()));
		audioPlayer.setPaused(true);
	} else {
		audioPlayer.setPaused(false);
	}
}   
 
//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(100, 100, 100);
	videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
	videoPlayer.setUsePixels(false);
	ofAddListener(hSlider_1.onMousePressed, this, & ofApp::hSlider_1onMousePressed);
	ofAddListener(hSlider_2.onMousePressed, this, & ofApp::hSlider_2onMousePressed); 
	ofAddListener(hSlider_3.onMousePressed, this, & ofApp::hSlider_3onMousePressed);
	ofAddListener(bang_1.onMousePressed, this, & ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, & ofApp::bang_2onMousePressed);
	ofAddListener(bang_3.onMousePressed, this, & ofApp::bang_3onMousePressed);
	ofAddListener(toggle_1.onMousePressed, this, & ofApp::toggle_1onMousePressed);
	ofAddListener(loadAudioUrlEvent, this, & ofApp::loadAudioUrlX);
	ofAddListener(loadVideoUrlEvent, this, & ofApp::loadVideoUrlX);
	ofAddListener(loadImageEvent, this, & ofApp::loadImageX);
	bang_1.setup(50, 125, 20);
	bang_2.setup(50, 150, 20);
	bang_3.setup(50, 175, 20);
	toggle_1.setup(50, 200, 20);
	label_1.setup(50, 30, 205, 75, "");
	label_2.setup(155, 125, 100, 20, "Load audio");
	label_3.setup(155, 150, 100, 20, "Load video");
	label_4.setup(155, 175, 100, 20, "Load image");
	label_5.setup(155, 200, 100, 20, "Pause");
	label_6.setup(155, 225, 100, 20, "Position");
	label_7.setup(155, 250, 100, 20, "0");
	label_8.setup(155, 275, 100, 20, "Panorama");
	label_9.setup(155, 300, 100, 20, "0");
	label_10.setup(155, 325, 100, 20, "Volume");
	label_11.setup(155, 350, 100, 20, "0.5");
	hSlider_1.setup(50, 225, 100, 20, 0, 1);
	hSlider_1.slider = 0;
	hSlider_2.setup(50, 275, 100, 20, -1, 1);
	hSlider_2.slider = 0.5;
	hSlider_3.setup(50, 325, 100, 20, 0, 1);
	hSlider_3.slider = 0.5;
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSoundUpdate();
	videoPlayer.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(240, 220, 100);
        ofDrawRectangle(20, 20, 285, 450);
        ofDrawRectangle(325, 20, 340, 450);
        ofSetColor(0, 0, 0);
        ofDrawRectangle(345, 40, 300, 200);
        ofDrawRectangle(345, 260, 300, 200);
	label_1.draw();
	label_2.draw();
	label_3.draw();
	label_4.draw();
	label_5.draw();
	label_6.draw();
	label_7.draw();
	label_8.draw();
	label_9.draw();
	label_10.draw();
	label_11.draw();
	bang_1.draw();
	bang_2.draw();
	bang_3.draw();
	toggle_1.draw();
	hSlider_1.draw();
	hSlider_2.draw();
	hSlider_3.draw();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Import.", 60, 55);
	if (videoPlayer.getTexture() -> isAllocated()) {
		videoPlayer.getTexture() -> draw(345, 40, 300, 200);
	}
	if (image.isAllocated()) {
		image.draw(345, 260, 300, 200);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	image.clear();
}
