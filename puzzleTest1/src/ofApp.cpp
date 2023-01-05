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
        ofSetBackgroundColor(200);
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(number_2.onMousePressed, this, &ofApp::number_2onMousePressed);
	ofAddListener(loadImageEvent, this, & ofApp::loadImageX);
	bang_1.setup(50, 620, 30);
	bang_2.setup(50, 660, 30);
	number_1.setup(210, 620, 100, 30, 0, 5);
	number_1.value = 2;
	number_2.setup(210, 660, 100, 30, 0, 5);
	number_2.value = 1;
	label_1.setup(85, 620, 120, 30, "Load image");
	label_2.setup(85, 660, 120, 30, "Play");
	label_3.setup(315, 620, 120, 30, "Pieces X");
	label_4.setup(315, 660, 120, 30, "Pieces Y");
	xPieces = 4;
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
		fbo.getTexture().drawSubsection(20, 20 + row * puzzlePieceHeight, xPos, puzzlePieceHeight, puzzleWidth - xPos, row * puzzlePieceHeight);
		fbo.getTexture().drawSubsection(20 + xPos, 20 + row * puzzlePieceHeight, puzzleWidth - xPos, puzzlePieceHeight, 0, row * puzzlePieceHeight);
	}
	if(mouseButton == 2){
		fbo.getTexture().drawSubsection(20 + column * puzzlePieceWidth, 20, puzzlePieceWidth, yPos, column * puzzlePieceWidth, puzzleHeight - yPos);
		fbo.getTexture().drawSubsection(20 + column * puzzlePieceWidth, 20 + yPos, puzzlePieceWidth, puzzleHeight - yPos, column * puzzlePieceWidth, 0);
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
void ofApp::bang_2onMousePressed(bool & e){
	play = true;
	moves = 0;
	playTime = ofGetElapsedTimef();
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
	std::vector<int> tempData;
	if(mouseButton==0){
		for (int i = 0; i < xPieces; i++) {
			tempData.push_back(data[i + row * xPieces]);
		}
		if(xVal > args.x){
			std::rotate(tempData.begin(), tempData.begin() +  xNumber / puzzlePieceWidth, tempData.end());
		}else{
			std::rotate(tempData.begin(), tempData.begin() + xPieces - xNumber / puzzlePieceWidth, tempData.end());
		}
		fbo.begin();
		for (int i = 0; i < xPieces; i++) {
			data[i + row * xPieces] = tempData[i];
			fboImg.getTexture().drawSubsection(i * puzzlePieceWidth, row * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, (tempData[i] % xPieces) * puzzlePieceWidth, (tempData[i] / xPieces) * puzzlePieceHeight);
		}
		fbo.end();
		if(xNumber > 0 && play){
			moves++;
		}
	}
	if(mouseButton==2){
		for (int i = 0; i < yPieces; i++) {
			tempData.push_back(data[i * xPieces + column]);
		}
		if(yVal > args.y){
			std::rotate(tempData.begin(), tempData.begin() + yNumber / puzzlePieceHeight, tempData.end());
		}else{
			std::rotate(tempData.begin(), tempData.begin() + yPieces - yNumber / puzzlePieceHeight, tempData.end());
		}
		fbo.begin();
		for (int i = 0; i < yPieces; i++) {
			data[i * xPieces + column] = tempData[i];
			fboImg.getTexture().drawSubsection(column * puzzlePieceWidth, i * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight, (tempData[i] % xPieces) * puzzlePieceWidth, (tempData[i] / xPieces) * puzzlePieceHeight);
		}
		fbo.end();
		if(yNumber > 0 && play){
			moves++;
		}
	}
	bool win = true;
	for (int i = 0; i < xPieces * yPieces; i++) {
		if(data[i] != i){
			win = false;
		}
	}
	if (win && play){
		play = false;
		playTime = ofGetElapsedTimef()-playTime;
		ofLog(OF_LOG_NOTICE, "you won in " + ofToString(playTime) + " seconds and with " + ofToString(moves) +  " moves.");
		// ofLog(OF_LOG_NOTICE, "you won in" + ofToString(10));
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
