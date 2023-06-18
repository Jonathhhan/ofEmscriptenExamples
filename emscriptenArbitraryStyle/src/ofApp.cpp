#include "ofApp.h"

bool arbitraryStyleModelIsLoaded;
std::string arbitraryStyleModelStatus;

void arbitraryStyleModelLoaded(){    
	arbitraryStyleModelStatus = "Arbitrary style model loaded.";
	arbitraryStyleModelIsLoaded = true;
}

//--------------------------------------------------------------
EMSCRIPTEN_BINDINGS(Module){
	emscripten::function("arbitraryStyleModelLoaded", &arbitraryStyleModelLoaded);
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(100, 100, 100);
	arbitraryStyleModelStatus = "Arbitrary style model loading.";
	tex1.allocate(256, 256, GL_RGBA);
	tex2.allocate(256, 256, GL_RGBA);
	tex3.allocate(256, 256, GL_RGBA);
	ofLoadImage(tex1, "data/style/flower_of_life.jpg");
	ofLoadImage(tex2, "data/wald.jpg");
	EM_ASM(
		setupArbitraryStyle($0, $1, $2, $3, $4), tex1.getTextureData().textureID, tex2.getTextureData().textureID, tex3.getTextureData().textureID, tex2.getWidth(), tex2.getHeight()
	);
}
 
//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(arbitraryStyleModelStatus, 20, 20);
	ofDrawBitmapString("Change style with 1 - 8.", 20, 602);
	tex1.draw(50, 50, 256, 256);
	tex2.draw(50, 50 + 256, 256, 256);
	ofSetColor(0);
	ofDrawRectangle(50 + 256, 50, 512, 512);
	ofSetColor(255);
	tex3.draw(50 + 256, 50, 512, 512);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key >= 49 && key <= 56 && arbitraryStyleModelIsLoaded && oldStyleImageKey != key) {
		oldStyleImageKey = key;
		if (key == 49) {
			ofLoadImage(tex1, "data/style/antimonocromatismo.jpg");
		} else if (key == 50) {
			ofLoadImage(tex1, "data/style/chinese_style.jpg");
		} else if (key == 51) {
			ofLoadImage(tex1, "data/style/face_and_hands.jpg");
		} else if (key == 52) {
			ofLoadImage(tex1, "data/style/flower_of_life.jpg");
		} else if (key == 53) {
			ofLoadImage(tex1, "data/style/higher_than_mountains.jpg");
		} else if (key == 54) {
			ofLoadImage(tex1, "data/style/udnie.jpg");
		} else if (key == 55) {
			ofLoadImage(tex1, "data/style/gogh.jpg");
		} else if (key == 56) {
			ofLoadImage(tex1, "data/style/wall.jpg");
		}
		EM_ASM(
			runArbitraryStyle($0, $1, $2, $3, $4), tex1.getTextureData().textureID, tex2.getTextureData().textureID, tex3.getTextureData().textureID, tex2.getWidth(), tex2.getHeight()
		);
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
