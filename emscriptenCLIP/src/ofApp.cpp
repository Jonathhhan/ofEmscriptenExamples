#include "ofApp.h"

std::string animation;

void isGenerated(std::string loaded){    
	animation = loaded; 
}

//--------------------------------------------------------------
EMSCRIPTEN_BINDINGS(Module){
	emscripten::function("isGenerated", &isGenerated);
}

//--------------------------------------------------------------
void ofApp::slider_onMousePressed(float & e){

}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(100, 100, 100);
	ofAddListener(slider.onMousePressed, this, & ofApp::slider_onMousePressed);
	slider.setup(150, 590, 120, 15, 0.05, 0.55);
	slider.slider = 0.5;
	animation = "Generating animation... (be patient).";
	tex.allocate(512, 512, GL_RGBA);
	EM_ASM(
		loadCLIP();
	);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(animation + " FPS: " + ofToString(ofGetFrameRate()), 20, 20);
	ofDrawBitmapString("Animation speed", 20, 602);
	tex.draw(50, 50, 512, 512);
	slider.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	EM_ASM(
		interference("a cute cat");
	);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
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
