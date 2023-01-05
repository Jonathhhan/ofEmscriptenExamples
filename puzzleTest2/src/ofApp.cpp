#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	xPieces = 4;
	yPieces = 4;
	puzzleWidth = 800;
	puzzleHeight = 600;
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
	fbo.draw(100, 50);
	if(mouseButton == 0){
		fbo.getTexture().drawSubsection(100, 50 + row * puzzlePieceHeight, xPos, puzzlePieceHeight, puzzleWidth - xPos, row * puzzlePieceHeight);
		fbo.getTexture().drawSubsection(100 + xPos, 50 + row * puzzlePieceHeight, puzzleWidth - xPos, puzzlePieceHeight, 0, row * puzzlePieceHeight);
	}
	if(mouseButton == 2){
		fbo.getTexture().drawSubsection(100 + column * puzzlePieceWidth, 50, puzzlePieceWidth, yPos, column * puzzlePieceWidth, puzzleHeight - yPos);
		fbo.getTexture().drawSubsection(100 + column * puzzlePieceWidth, 50 + yPos, puzzlePieceWidth, puzzleHeight - yPos, column * puzzlePieceWidth, 0);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(mouseIsPressed){
		xPos = (x - xVal) % puzzleWidth;
		xNumber = (abs(x - xVal ) +  puzzlePieceWidth / 2) % puzzleWidth;
		if(xPos < 0){
			xPos = xPos + puzzleWidth;
		}
		yPos = (y - yVal) % puzzleHeight;
		yNumber = (abs(y - yVal ) + puzzlePieceHeight / 2) % puzzleHeight;
		if(yPos < 0){
			yPos = yPos + puzzleHeight;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(x > 100 && x < puzzleWidth + 100 && y > 50 && y < puzzleHeight + 50){
		mouseIsPressed = true;
		row = (y - 50) / puzzlePieceHeight;
		column = (x - 100) / puzzlePieceWidth;
	}
	xVal = x;
	yVal = y;
	xPos = 0;
	yPos = 0;
	xNumber = 0;
	yNumber = 0;
	mouseButton = button;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	std::vector<int> tempData;
	if(mouseButton==0){
		for (int i = 0; i < xPieces; i++) {
			tempData.push_back(data[i + row * xPieces]);
		}
		if(xVal > x){
			std::rotate(tempData.begin(), tempData.begin() +  xNumber / puzzlePieceWidth, tempData.end());
		}else{
			std::rotate(tempData.begin(), tempData.begin() + xPieces - xNumber / puzzlePieceWidth, tempData.end());
		}
		fbo.begin();
		for (int i = 0; i < xPieces; i++) {
			data[i + row * xPieces] = tempData[i];
			ofSetColor(255);
			fboImg.getTexture().drawSubsection(i * puzzlePieceWidth, row * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, (tempData[i] % xPieces) * puzzlePieceWidth, (tempData[i] / xPieces) * puzzlePieceHeight);
		}
		fbo.end();
	}
	if(mouseButton==2){
		for (int i = 0; i < yPieces; i++) {
			tempData.push_back(data[i * xPieces + column]);
		}
		if(yVal > y){
			std::rotate(tempData.begin(), tempData.begin() + yNumber / puzzlePieceHeight, tempData.end());
		}else{
			std::rotate(tempData.begin(), tempData.begin() + yPieces - yNumber / puzzlePieceHeight, tempData.end());
		}
		fbo.begin();
		for (int i = 0; i < yPieces; i++) {
			data[i * xPieces + column] = tempData[i];
			ofSetColor(255);
			fboImg.getTexture().drawSubsection(column * puzzlePieceWidth, i * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, (tempData[i] % xPieces) * puzzlePieceWidth, (tempData[i] / xPieces) * puzzlePieceHeight);
		}
		fbo.end();
	}
	mouseButton = 3;
	mouseIsPressed = false;
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
