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
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
	fboImg.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fboImg.end();
	EM_ASM(FS.unlink("/data/data"));
	data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(std::make_pair(i % xPieces / xPieces, floor(i / xPieces) / yPieces));
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
	number_1.value = 4;
	number_2.setup(210, 700, 100, 30, 1, 100);
	number_2.value = 2;
	label_1.setup(85, 660, 120, 30, "Load image");
	label_2.setup(85, 700, 120, 30, "Play");
	label_3.setup(315, 660, 120, 30, "Pieces X");
	label_4.setup(315, 700, 120, 30, "Pieces Y");
	label_5.setup(440, 660, 200, 30, "Moves: 0");
	label_6.setup(440, 700, 200, 30, "Seconds: 0");
	xPieces = 4;
	yPieces = 2;
	puzzlePieceWidth = puzzleWidth / xPieces;
	puzzlePieceHeight = puzzleHeight / yPieces;
	image.load("wald.jpg");
	fbo.allocate(puzzleWidth, puzzleHeight, GL_RGBA);
	fboImg.allocate(puzzleWidth, puzzleHeight, GL_RGBA);
	fboPuzzlePiece.allocate(puzzlePieceWidth + puzzlePieceWidth / 4, puzzlePieceHeight + puzzlePieceHeight / 4, GL_RGBA);
	for (int i = 0; i < xPieces * yPieces; i++) {
		data.push_back(std::make_pair(i % xPieces / xPieces, floor(i / xPieces) / yPieces));
	}
	fbo.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
	fboImg.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fboImg.end();
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
	shaderRandom.load("random");
	shaderReplace.load("replace");
	shaderEmptyPuzzlePiece.load("emptyPuzzlePiece");
	shaderPuzzlePiece.load("puzzlePiece");
	image2.allocate(puzzleWidth, puzzleHeight, OF_IMAGE_COLOR_ALPHA);
	image3.load("CPE15cm.png");
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
		if(!isTouch){
			fboPuzzlePiece.draw(ofGetMouseX() - (puzzlePieceWidth + puzzlePieceWidth / 4) / 2, ofGetMouseY() - (puzzlePieceHeight + puzzlePieceHeight / 4) / 2);
		}else{
			fboPuzzlePiece.draw(touchX - (puzzlePieceWidth + puzzlePieceWidth / 4) / 2, touchY - (puzzlePieceHeight + puzzlePieceHeight / 4) / 2);
		}
	}
	image2.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	image2.drawSubsection(0, 0, 100, 100, 0, 0);
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
	for (int i = 0; i < data.size(); i++) {
		image2.setColor(i % xPieces, floor(i / xPieces), ofFloatColor(data[i].first, data[i].second, 0, 1));
		// ofLog(OF_LOG_NOTICE, "the number " + ofToString(i) + " is x: " + ofToString(data[i].first * xPieces) + " is y: " + ofToString(data[i].second * yPieces));
	}
	image2.update();
	fbo.begin();
	ofClear(255,0,0,255);
	shaderRandom.begin();
	shaderRandom.setUniformTexture("texture_image", fboImg.getTexture(), 0);
	shaderRandom.setUniformTexture("texture_data", image2.getTexture(), 1);
	shaderRandom.setUniformTexture("texture_mask", image3.getTexture(), 2);
	shaderRandom.setUniform2f("resolution", puzzleWidth, puzzleHeight);
	shaderRandom.setUniform2f("puzzlePieces", xPieces, yPieces);
	ofDrawRectangle(0, 0, puzzleWidth, puzzleHeight);
	shaderRandom.end();
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
		data.push_back(std::make_pair(i % xPieces / xPieces, floor(i / xPieces) / yPieces));
	}
	puzzlePieceWidth = puzzleWidth / xPieces;
	fboPuzzlePiece.allocate(puzzlePieceWidth + puzzlePieceWidth / 4, puzzlePieceHeight + puzzlePieceHeight / 4, GL_RGBA);
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
		data.push_back(std::make_pair(i % xPieces / xPieces, floor(i / xPieces) / yPieces));
	}
	puzzlePieceHeight = puzzleHeight / yPieces;
	fboPuzzlePiece.allocate(puzzlePieceWidth + puzzlePieceWidth / 4, puzzlePieceHeight + puzzlePieceHeight / 4, GL_RGBA);
	fbo.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){

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
	touchX = args.x;
	touchY = args.y;
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
