#include "ofApp.h"
#include "emscripten.h"
#include "emscripten/bind.h"

ofEvent<std::string> loadAudioUrlEvent; 	
ofEvent<std::string> loadVideoUrlEvent; 

void loadAudioUrl(const std::string & url) {
	std::string urlCopy = url;         
	loadAudioUrlEvent.notify(urlCopy); 
} 

void loadVideoUrl(const std::string & url) {
	std::string urlCopy = url;         
	loadVideoUrlEvent.notify(urlCopy); 
} 

EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("loadAudioUrl", & loadAudioUrl);
	emscripten::function("loadVideoUrl", & loadVideoUrl);
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
	var file_selector = document.createElement('input');
	file_selector.setAttribute('type', 'file');
	file_selector.setAttribute('accept','.wav, .mp3, .mp4, adts. ,ogg. ,webm. ,caf. ,flac.');
	file_selector.addEventListener("change", function(e){
	var file = e.target.files[0]; 
	var url = URL.createObjectURL(file);
	Module.loadAudioUrl(url);
	});
	file_selector.click();
	);
}   

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e){ 
	EM_ASM(
	var file_selector = document.createElement('input');
	file_selector.setAttribute('type', 'file');
	file_selector.setAttribute('accept','.mp4, .webm, .3gp, mpeg. ,mov. ,ogg. ,caf. ,flac., .h264');
	file_selector.addEventListener("change", function(e){
	var file = e.target.files[0]; 
	var url = URL.createObjectURL(file);
	Module.loadVideoUrl(url);
	});
	file_selector.click();
	);
}   

//--------------------------------------------------------------
void ofApp::toggle_1onMousePressed(bool & e){ 
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
	ofAddListener(toggle_1.onMousePressed, this, & ofApp::toggle_1onMousePressed);
	ofAddListener(loadAudioUrlEvent, this, & ofApp::loadAudioUrlX);
	ofAddListener(loadVideoUrlEvent, this, & ofApp::loadVideoUrlX);
	bang_1.setup(50, 125, 20);
	bang_2.setup(50, 150, 20);
	toggle_1.setup(50, 175, 20);
	label_1.setup(50, 30, 205, 75, "");
	label_2.setup(155, 125, 100, 20, "Load audio");
	label_3.setup(155, 150, 100, 20, "Load video");
	label_4.setup(155, 175, 100, 20, "Pause");
	label_5.setup(155, 200, 100, 20, "Position");
	label_6.setup(155, 225, 100, 20, "0");
	label_7.setup(155, 250, 100, 20, "Panorama");
	label_8.setup(155, 275, 100, 20, "0");
	label_9.setup(155, 300, 100, 20, "Volume");
	label_10.setup(155, 325, 100, 20, "0.5");
	hSlider_1.setup(50, 200, 100, 20, 0, 1);
	hSlider_1.slider = 0;
	hSlider_2.setup(50, 250, 100, 20, -1, 1);
	hSlider_2.slider = 0.5;
	hSlider_3.setup(50, 300, 100, 20, 0, 1);
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
        ofDrawRectangle(20, 20, 270, 450);
        ofSetColor(0, 0, 0);
        ofDrawRectangle(50, 350, 205, 110);
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
	bang_1.draw();
	bang_2.draw();
	toggle_1.draw();
	hSlider_1.draw();
	hSlider_2.draw();
	hSlider_3.draw();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Load an audio file", 60, 55);
	ofDrawBitmapString("or a video file.", 60, 75);
	if (videoPlayer.getTexture() -> isAllocated()) {
	videoPlayer.getTexture() -> draw(50, 350, 205, 110);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {

}

