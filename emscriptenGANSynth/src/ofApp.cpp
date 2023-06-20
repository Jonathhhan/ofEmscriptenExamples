#include "ofApp.h"

std::string ganSynthModelStatus;
bool ganSynthModelIsLoaded;

void ganSynthModelLoaded(){    
	ganSynthModelStatus = "GAN synth model loaded. Click for sound.";
	ganSynthModelIsLoaded = true;
}

//--------------------------------------------------------------
EMSCRIPTEN_BINDINGS(Module){
	emscripten::function("ganSynthModelLoaded", &ganSynthModelLoaded);
}

//--------------------------------------------------------------
void ofApp::setup(){
	ganSynthModelIsLoaded = false;
	ofSetBackgroundColor(100, 100, 100);
	ganSynthModelStatus = "GAN synth model loading.";
	tex1.allocate(512, 256, GL_RGBA);
	tex2.allocate(512, 256, GL_RGBA);
	EM_ASM(
		startGanSynth();
	);
}
 
//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(ganSynthModelStatus, 20, 20);
	tex1.draw(50, 50);
	tex2.draw(50, 50 + 256);
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
void ofApp::mousePressed(int x, int y, int button) {
	if (ganSynthModelIsLoaded) {
		EM_ASM(
			sampleGanNote($0, $1), tex1.getTextureData().textureID, tex2.getTextureData().textureID
		);
	}
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
