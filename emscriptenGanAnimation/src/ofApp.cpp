#include "ofApp.h"

std::string animation;

void isGenerated(std::string loaded) {    
	animation = loaded; 
}

//--------------------------------------------------------------
EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("isGenerated", &isGenerated);
}

//--------------------------------------------------------------
void ofApp::setup(){
	animation = "Generating animation... (be patient).";
	tex.allocate(512, 512, GL_RGBA);
	EM_ASM(model_runner.setup_model('resnet256'); model_runner.animate($0), tex.getTextureData().textureID);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100, 100, 100);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(animation + " FPS: " + ofToString(ofGetFrameRate()), 20, 20);
	tex.draw(50, 50, 512, 512);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
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
