#include "ofApp.h"

std::string animation;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(100, 100, 100);
	animation = "Generating animation... (be patient).";
	tex1.allocate(256, 256, GL_RGBA);
	tex2.allocate(256, 256, GL_RGBA);
	tex3.allocate(256, 256, GL_RGBA);
	tex4.allocate(256, 256, GL_RGBA);
	EM_ASM(
		loadTextCLIP();
	);
}
 
//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 20, 20);
	ofDrawBitmapString("Prompt: " + textInput, 20, 602);
	tex1.draw(50, 50);
	tex2.draw(50, 50 + 256);
	tex3.draw(50 + 256, 50);
	tex4.draw(50 + 256, 50 + 256);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_RETURN) {
		EM_ASM(
			interferenceText(UTF8ToString($4), $0, $1, $2, $3), tex1.getTextureData().textureID, tex2.getTextureData().textureID, tex3.getTextureData().textureID, tex4.getTextureData().textureID, textInput.c_str()
		);
		textInput = "";
	} else {
		if (key==8 && textInput.size()>0) { // backspace
			textInput = textInput.substr(0, textInput.size()-1); // delete one character
		} else if (textInput.size() < 50) {
			textInput.append (1, (char)key );
		}
	}
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
