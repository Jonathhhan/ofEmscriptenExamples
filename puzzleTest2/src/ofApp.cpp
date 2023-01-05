// Refer to the README.md in the example's root folder for more information on usage

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
        ofSetWindowTitle("Puzzle");
        ofSetBackgroundColor(200);
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(number_2.onMousePressed, this, &ofApp::number_2onMousePressed);
	bang_1.setup(50, 640, 30);
	bang_2.setup(50, 680, 30);
	number_1.setup(210, 640, 100, 30, 0, 5);
	number_1.value = 1;
	number_2.setup(210, 680, 100, 30, 0, 5);
	number_2.value = 1;
	label_1.setup(85, 640, 120, 30, "Load image");
	label_2.setup(85, 680, 120, 30, "Play");
	label_3.setup(315, 640, 120, 30, "Pieces X");
	label_4.setup(315, 680, 120, 30, "Pieces Y");
	xPieces = 2;
	yPieces = 2;
	puzzleWidth = 800;
	puzzleHeight = 576;
	puzzlePieceWidth = puzzleWidth / xPieces;
	puzzlePieceHeight = puzzleHeight / yPieces;
	image.load("wald3.jpg");
	fbo.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fboImg.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(i);
	}
	fbo.begin();
	ofClear(255);
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
	fboImg.begin();
	ofClear(255);
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fboImg.end();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(240, 220, 100);
	ofDrawRectangle(20, 20, 800, 800 / 1.5);
	ofSetColor(20, 170, 150);
	ofDrawRectangle(20, (ofGetHeight() - 60)/ 3 * 2 + 40, 800, ((ofGetHeight()- 60)/ 3 ));
	bang_1.draw();
	bang_2.draw();
	number_1.draw();
	number_2.draw();
	label_1.draw();
	label_2.draw();
	label_3.draw();
	label_4.draw();
	ofSetColor(255);
	fbo.draw(20, 20);
	if(mouseButton == 0){
		fbo.getTexture().drawSubsection(ofGetMouseX() - puzzlePieceWidth / 2, ofGetMouseY() - puzzlePieceHeight / 2, puzzlePieceWidth, puzzlePieceHeight, column * puzzlePieceWidth, row * puzzlePieceHeight);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed  (ofKeyEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::keyReleased  (ofKeyEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs & args){
	if(mouseIsPressed){
		xPos = ((int)args.x - xVal) % puzzleWidth;
		xNumber = (abs((int)args.x - xVal ) +  puzzlePieceWidth / 2) % puzzleWidth;
		if(xPos < 0){
			xPos = xPos + puzzleWidth;
		}
		yPos = ((int)args.y - yVal) % puzzleHeight;
		yNumber = (abs((int)args.y - yVal ) + puzzlePieceHeight / 2) % puzzleHeight;
		if(yPos < 0){
			yPos = yPos + puzzleHeight;
		}
	}
}

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e){

}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e){
    auto rng = std::default_random_engine {};
    std::shuffle(data.begin(), data.end(), rng);
		fbo.begin();
		ofClear(255);
		for (int i = 0; i < xPieces * yPieces; i++) {
			fboImg.getTexture().drawSubsection(i % xPieces * puzzlePieceWidth, i / xPieces * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, (data[i] % xPieces) * puzzlePieceWidth, (data[i] / xPieces) * puzzlePieceHeight);
		}
		fbo.end();
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float & e){
	xPieces = e;
	data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(i);
	}
	puzzlePieceWidth = puzzleWidth / xPieces;
	fbo.begin();
	ofClear(255);
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::number_2onMousePressed(float & e){
	yPieces = e;
	data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(i);
	}
	puzzlePieceHeight = puzzleHeight / yPieces;
	fbo.begin();
	ofClear(255);
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
	if(args.x > 20 && args.x < puzzleWidth + 20 && args.y > 20 && args.y < puzzleHeight + 20){
		mouseIsPressed = true;
		row = (args.y - 20) / puzzlePieceHeight;
		column = (args.x - 20) / puzzlePieceWidth;
	}
	xVal = args.x;
	yVal = args.y;
	xPos = 0;
	yPos = 0;
	xNumber = 0;
	yNumber = 0;
	mouseButton = args.button;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){
	if(mouseButton==0){
		fbo.begin();
			fboImg.getTexture().drawSubsection(((int)args.x - 20) - ((int)args.x - 20) % puzzlePieceWidth, ((int)args.y - 20) - ((int)args.y - 20) % puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, data[row * xPieces + column] % xPieces * puzzlePieceWidth, data[row * xPieces + column] / xPieces * puzzlePieceHeight);
			fboImg.getTexture().drawSubsection(data[row * xPieces + column] % xPieces * puzzlePieceWidth, data[row * xPieces + column] / xPieces * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, ((int)args.x - 20) - ((int)args.x - 20) % puzzlePieceWidth, ((int)args.y - 20) - ((int)args.y - 20) % puzzlePieceHeight);
int a = data[row * xPieces + column];
int b = data[(((int)args.y - 20) - ((int)args.y - 20) % puzzlePieceHeight)/puzzlePieceHeight * xPieces +  (((int)args.x - 20) - ((int)args.x - 20) % puzzlePieceWidth)/puzzlePieceWidth];
data[row * xPieces + column] = b;
data[(((int)args.y - 20) - ((int)args.y - 20) % puzzlePieceHeight)/puzzlePieceHeight * xPieces +  (((int)args.x - 20) - ((int)args.x - 20) % puzzlePieceWidth)/puzzlePieceWidth] = a;
ofLog(OF_LOG_NOTICE, "row " + ofToString(row));
ofLog(OF_LOG_NOTICE, "col " + ofToString(column));
ofLog(OF_LOG_NOTICE, "res " + ofToString(row*xPieces+ column));
ofLog(OF_LOG_NOTICE, "res2 " + ofToString(data[(row*xPieces+ column)]));
ofLog(OF_LOG_NOTICE, "res3 " + ofToString((((int)args.x - 20) - ((int)args.x - 20) % puzzlePieceWidth)/puzzlePieceWidth));
ofLog(OF_LOG_NOTICE, "res4 " + ofToString((((int)args.y - 20) - ((int)args.y - 20) % puzzlePieceHeight)/puzzlePieceHeight));
ofLog(OF_LOG_NOTICE, "a " + ofToString(a));
ofLog(OF_LOG_NOTICE, "b " + ofToString(b));
		fbo.end();
	}
	mouseButton = 3;
	mouseIsPressed = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseExited(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & args){

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
