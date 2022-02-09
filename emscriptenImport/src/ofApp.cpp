#include "ofApp.h"
#include "emscripten.h"
#include "emscripten/bind.h"

ofEvent<std::string> loadAudioUrlEvent; 	
ofEvent<std::string> loadVideoUrlEvent; 
ofEvent<void> loadImageEvent;

//--------------------------------------------------------------
void ofApp::bang_1_event(bool & e) { 
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.adts, .aiff, .caf, .flac, .mp3, .mp4, .ogg, .wav, .webm';
	input.onchange = function(e) {
		var file = e.target.files[0]; 
		var url = URL.createObjectURL(file);
		Module.loadAudioUrl(url);
		};
	input.click();
	);
}   

void loadAudioUrl(std::string url) {    
	loadAudioUrlEvent.notify(url); 
} 

void ofApp::loadAudioUrlX(std::string & url) {
	audioPlayer.unload();
	audioPlayer.load(url);
	audioPlayer.setLoop(true);
	audioPlayer.play();
}

//--------------------------------------------------------------
void ofApp::bang_2_event(bool & e) { 
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.3gp, .h264, .mov, .mp4, .mpeg, .ogg, .webm';
	input.onchange = function(e) {
		var file = e.target.files[0]; 
		var url = URL.createObjectURL(file);
		Module.loadVideoUrl(url);
	};
	input.click();
	);
}

void loadVideoUrl(std::string url) {
	loadVideoUrlEvent.notify(url); 
}

void ofApp::loadVideoUrlX(std::string & url) {
	videoPlayer.load(url);
	videoPlayer.play();
}

//--------------------------------------------------------------
void ofApp::bang_3_event(bool & e) { 
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.apng, .avif, .bmp, .gif, .ico, .jfif, .jpe, .jpeg, .jpg, .png, .svg, .tif, .tiff, .webp';
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
			FS.syncfs(true, function (err) {
				Module.loadImage();
				assert(!err);
        		});	
		}
	};
	input.click();
	);
}   

void loadImage() {        
	loadImageEvent.notify(); 
}

void ofApp::loadImageX() {
	ofLoadImage(texture, "data");
	EM_ASM(FS.unlink("/data/data"));
}

//--------------------------------------------------------------
EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("loadAudioUrl", & loadAudioUrl);
	emscripten::function("loadVideoUrl", & loadVideoUrl);
	emscripten::function("loadImage", & loadImage);
}

//--------------------------------------------------------------
void ofApp::toggle_1_event(bool & e) { 
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
void ofApp::hSlider_1_event(float & e) {
	audioPlayer.setPosition(e);
	videoPlayer.setPosition(e);
	label_7.symbol = ofToString(e); 
}

//--------------------------------------------------------------
void ofApp::hSlider_2_event(float & e) { 
	audioPlayer.setPan(e);
	videoPlayer.setPan(e);
	label_9.symbol = ofToString(e);
}

//--------------------------------------------------------------
void ofApp::hSlider_3_event(float & e) { 
	audioPlayer.setVolume(e);
	videoPlayer.setVolume(e);
	label_11.symbol = ofToString(e); 
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(100, 100, 100);
	videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
	videoPlayer.setUsePixels(false);
	ofAddListener(hSlider_1.onMousePressed, this, & ofApp::hSlider_1_event);
	ofAddListener(hSlider_2.onMousePressed, this, & ofApp::hSlider_2_event); 
	ofAddListener(hSlider_3.onMousePressed, this, & ofApp::hSlider_3_event);
	ofAddListener(bang_1.onMousePressed, this, & ofApp::bang_1_event);
	ofAddListener(bang_2.onMousePressed, this, & ofApp::bang_2_event);
	ofAddListener(bang_3.onMousePressed, this, & ofApp::bang_3_event);
	ofAddListener(toggle_1.onMousePressed, this, & ofApp::toggle_1_event);
	ofAddListener(loadAudioUrlEvent, this, & ofApp::loadAudioUrlX);
	ofAddListener(loadVideoUrlEvent, this, & ofApp::loadVideoUrlX);
	ofAddListener(loadImageEvent, this, & ofApp::loadImageX);
	bang_1.setup(40, 160, 20);
	bang_2.setup(40, 185, 20);
	bang_3.setup(40, 210, 20);
	toggle_1.setup(40, 235, 20);
	label_1.setup(40, 40, 205, 75, "");
	label_2.setup(145, 160, 100, 20, "Load audio");
	label_3.setup(145, 185, 100, 20, "Load video");
	label_4.setup(145, 210, 100, 20, "Load image");
	label_5.setup(145, 235, 100, 20, "Pause");
	label_6.setup(145, 260, 100, 20, "Position");
	label_7.setup(145, 285, 100, 20, "0");
	label_8.setup(145, 310, 100, 20, "Panorama");
	label_9.setup(145, 335, 100, 20, "0");
	label_10.setup(145, 360, 100, 20, "Volume");
	label_11.setup(145, 385, 100, 20, "0.5");
	hSlider_1.setup(40, 260, 100, 20, 0, 1);
	hSlider_1.slider = 0;
	hSlider_2.setup(40, 310, 100, 20, -1, 1);
	hSlider_2.slider = 0.5;
	hSlider_3.setup(40, 360, 100, 20, 0, 1);
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
        ofDrawRectangle(20, 20, 245, 460);
        ofDrawRectangle(285, 20, 340, 460);
        ofSetColor(0, 0, 0);
        ofDrawRectangle(305, 40, 300, 200);
        ofDrawRectangle(305, 260, 300, 200);
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
	ofDrawBitmapString("Import.", 50, 65);
	if (videoPlayer.getTexture() -> isAllocated()) {
		videoPlayer.getTexture() -> draw(305, 40, 300, 200);
	}
	if (texture.isAllocated()) {
		texture.draw(305, 260, 300, 200);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	audioPlayer.unload();
	videoPlayer.close();
	texture.clear();
}
