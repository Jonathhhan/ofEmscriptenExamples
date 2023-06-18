#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(100, 100, 100);
	tex1.allocate(256, 256, GL_RGBA);
	tex2.allocate(256, 256, GL_RGBA);
	tex3.allocate(256, 256, GL_RGBA);
	ofLoadImage(tex1, "data/style/flower_of_life.jpg");
	ofLoadImage(tex2, "data/berlin.jpg");
	EM_ASM(
		setupArbitraryStyle($0, $1), tex1.getTextureData().textureID, tex2.getTextureData().textureID
	);
}
 
//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	tex1.draw(50, 50, 256, 256);
	tex2.draw(50, 50 + 256, 256, 256);
	tex3.draw(50 , 50, 512, 512);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	EM_ASM(
		runArbitraryStyle($0, $1, $2), tex3.getTextureData().textureID, tex2.getWidth(), tex2.getHeight()
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
