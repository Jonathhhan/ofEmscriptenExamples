// Refer to the README.md in the example's root folder for more information on usage

#include "ofApp.h"

ofEvent<void> loadImageEvent;

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e){
	EM_ASM(
	if (canvas.hasFocus) {
		var input = document.createElement('input');
		input.type = 'file';
		input.accept = '.apng, .avif, .bmp, .gif, .ico, .jfif, .jpe, .jpeg, .jpg, .png, .svg, .tif, .tiff, .webp';
		input.click();
		canvas.hasFocus = false;
		input.onchange = function(e) {
			canvas.hasFocus = true;
			var file = e.target.files[0]; 
			var reader = new FileReader();
			reader.readAsArrayBuffer(file);
			reader.onload = function() {
				var arrayBuffer = reader.result;
				var uint8View = new Uint8Array(arrayBuffer);	
				FS.createDataFile("/data/", "data", uint8View, true, true);
				FS.syncfs(true, function (err) {
					new Module.loadImage();
					assert(!err);
        			});
			}
		}
	});
}   

void loadImage() {        
	loadImageEvent.notify(); 
}

void ofApp::loadImageX() {
	image.load("data");
	fbo.begin();
	ofClear(255);
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
	fboImg.begin();
	ofClear(255);
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fboImg.end();
	EM_ASM(FS.unlink("/data/data"));
	data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(i);
	}
	play = false;
}

//--------------------------------------------------------------
void ofApp::setup(){
        ofSetWindowTitle("Puzzle");
        emscripten::function("loadImage", &loadImage);
        ofSetBackgroundColor(240, 220, 100);
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(number_2.onMousePressed, this, &ofApp::number_2onMousePressed);
	ofAddListener(loadImageEvent, this, & ofApp::loadImageX);
	bang_1.setup(50, 660, 30);
	bang_2.setup(50, 700, 30);
	number_1.setup(210, 660, 100, 30, 1, 100);
	number_1.value = 2;
	number_2.setup(210, 700, 100, 30, 1, 100);
	number_2.value = 1;
	label_1.setup(85, 660, 120, 30, "Load image");
	label_2.setup(85, 700, 120, 30, "Play");
	label_3.setup(315, 660, 120, 30, "Pieces X");
	label_4.setup(315, 700, 120, 30, "Pieces Y");
	label_5.setup(440, 660, 200, 30, "Moves: 0");
	label_6.setup(440, 700, 200, 30, "Seconds: 0");
	xPieces = 4;
	yPieces = 2;
	puzzlePieceWidth = ceil(puzzleWidth / xPieces);
	puzzlePieceHeight = ceil(puzzleHeight / yPieces);
	image.load("wald.jpg");
	fbo.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fboImg.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(i);
	}
	fbo.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
	fboImg.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fboImg.end();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(play){
		label_6.symbol= "Seconds: " + ofToString(floor(ofGetElapsedTimef() - playTime));
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(0);
	ofDrawRectangle(20, 20, 820, 740);
	ofSetColor(20, 170, 100);
	ofDrawRectangle(30, 640, 800, 110);
	bang_1.draw();
	bang_2.draw();
	number_1.draw();
	number_2.draw();
	label_1.draw();
	label_2.draw();
	label_3.draw();
	label_4.draw();
	label_5.draw();
	label_6.draw();
	ofSetColor(255);
	fbo.draw(30, 30);
	if(mouseIsPressed){
		ofSetColor(0);
		ofDrawRectangle(column * puzzlePieceWidth + 30, row * puzzlePieceHeight + 30, puzzlePieceWidth, puzzlePieceHeight);
		ofSetColor(255);
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

}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e){
	play = true;
	playTime = ofGetElapsedTimef();
	moves = 0;
	label_5.symbol= "Moves: " + ofToString(moves);
	label_6.symbol= "Seconds: " + ofToString(floor(ofGetElapsedTimef() - playTime));
	std::shuffle(data.begin(), data.end(), rng);
	fbo.begin();
	for (int i = 0; i < xPieces * yPieces; i++) {
		fboImg.getTexture().drawSubsection(i % xPieces * puzzlePieceWidth, i / xPieces * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, data[i] % xPieces * puzzlePieceWidth, data[i] / xPieces * puzzlePieceHeight);
	}
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float & e){
	play = false;
	label_5.symbol= "Moves: " + ofToString(0);
	label_6.symbol= "Seconds: " + ofToString(0);
	xPieces = e;
	data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(i);
	}
	puzzlePieceWidth = puzzleWidth / xPieces;
	fbo.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::number_2onMousePressed(float & e){
	play = false;
	label_5.symbol= "Moves: " + ofToString(0);
	label_6.symbol= "Seconds: " + ofToString(0);
	yPieces = e;
	data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(i);
	}
	puzzlePieceHeight = puzzleHeight / yPieces;
	fbo.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
	if(args.x > 30 && args.x < puzzleWidth + 30 && args.y > 30 && args.y < puzzleHeight + 30){
		mouseIsPressed = true;
		row = (args.y - 30) / puzzlePieceHeight;
		column = (args.x - 30) / puzzlePieceWidth;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){
	if(args.x > 30 && args.x < puzzleWidth + 30 && args.y > 30 && args.y < puzzleHeight + 30 && mouseIsPressed){
		int a = data[row * xPieces + column];
		int b = data[(args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight)) / puzzlePieceHeight * xPieces +  (args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth)) / puzzlePieceWidth];
		if(a != b && play){
			moves++;
			label_5.symbol= "Moves: " + ofToString(moves);
		}
		fbo.begin();
			fboImg.getTexture().drawSubsection(args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth), args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight), puzzlePieceWidth, puzzlePieceHeight, data[row * xPieces + column] % xPieces * puzzlePieceWidth, data[row * xPieces + column] / xPieces * puzzlePieceHeight);
			fboImg.getTexture().drawSubsection(column * puzzlePieceWidth, row * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, b % xPieces * puzzlePieceWidth, b / xPieces * puzzlePieceHeight);
		fbo.end();
		data[row * xPieces + column] = b;
		data[(args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight)) / puzzlePieceHeight * xPieces +  (args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth)) / puzzlePieceWidth] = a;
	}
	bool win = true;
	for (int i = 0; i < xPieces * yPieces; i++) {
		if(data[i] != i){
			win = false;
		}
	}
	if (win && play){
		play = false;
		playTime = floor(ofGetElapsedTimef() - playTime);
		ofLog(OF_LOG_NOTICE, "you won in " + ofToString(playTime) + " seconds and with " + ofToString(moves) +  " moves.");
		moves = 0;
	}
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
