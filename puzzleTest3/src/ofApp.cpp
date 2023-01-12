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
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
	shaderRandom.load("random");
	shaderReplace.load("replace");
	image2.allocate(data.size(), 1, OF_IMAGE_COLOR_ALPHA);
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
		if(!isTouch){
			fbo.getTexture().drawSubsection(ofGetMouseX() - puzzlePieceWidth / 2, ofGetMouseY() - puzzlePieceHeight / 2, puzzlePieceWidth, puzzlePieceHeight, column * puzzlePieceWidth, row * puzzlePieceHeight);
		}else{
			fbo.getTexture().drawSubsection(touchX - puzzlePieceWidth / 2, touchY - puzzlePieceHeight / 2, puzzlePieceWidth, puzzlePieceHeight, column * puzzlePieceWidth, row * puzzlePieceHeight);
		}
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
	for (int i = 0; i < data.size(); i++) {
		image2.setColor(i, 0, ofColor(data[i] / 256 / 256 / 256, data[i] / 256 / 256, data[i] / 256, data[i]));
		//ofLog(OF_LOG_NOTICE, "the number is rgb " + ofToString((int)image2.getColor(i, 0).r * 256 * 256 + (int)image2.getColor(i, 0).g * 256 + (int)image2.getColor(i, 0).b));
		//ofLog(OF_LOG_NOTICE, "r " + ofToString((int)image2.getColor(i, 0).r * 256 * 256));
		//ofLog(OF_LOG_NOTICE, "g " + ofToString((int)image2.getColor(i, 0).g * 256));
		//ofLog(OF_LOG_NOTICE, "b " + ofToString((int)image2.getColor(i, 0).b));
	}
	image2.update();
	fbo.begin();
	ofClear(0);
	shaderRandom.begin();
	shaderRandom.setUniformTexture("Tex0", fboImg.getTexture(), 0);
	shaderRandom.setUniformTexture("Tex1", image2.getTexture(), 1);
	shaderRandom.setUniform2f("resolution", puzzleWidth, puzzleHeight);
	shaderRandom.setUniform2f("puzzlePieceSize", puzzlePieceWidth, puzzlePieceHeight);
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
		data.push_back(i);
	}
	puzzlePieceWidth = puzzleWidth / xPieces;
	fbo.begin();
	image.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo.end();
	image2.allocate(data.size(), 1, OF_IMAGE_COLOR_ALPHA);
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
	image2.allocate(data.size(), 1, OF_IMAGE_COLOR_ALPHA);
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
	if(args.x > 30 && args.x < puzzleWidth + 30 && args.y > 30 && args.y < puzzleHeight + 30 && !isTouch){
		mouseIsPressed = true;
		row = (args.y - 30) / puzzlePieceHeight;
		column = (args.x - 30) / puzzlePieceWidth;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){
	if(args.x > 30 && args.x < puzzleWidth + 30 && args.y > 30 && args.y < puzzleHeight + 30 && mouseIsPressed && !isTouch){
		int a = data[row * xPieces + column];
		int b = data[(args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight)) / puzzlePieceHeight * xPieces +  (args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth)) / puzzlePieceWidth];
		data[(args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight)) / puzzlePieceHeight * xPieces +  (args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth)) / puzzlePieceWidth] = a;
		data[row * xPieces + column] = b;
		if(a != b && play){
			moves++;
			label_5.symbol= "Moves: " + ofToString(moves);
		}
		fbo.begin();
		shaderReplace.begin();
		shaderReplace.setUniformTexture("Tex0", fboImg.getTexture(), 0);
		shaderReplace.setUniform2f("resolution", puzzleWidth, puzzleHeight);
		shaderReplace.setUniform2f("puzzlePieceSize", puzzlePieceWidth, puzzlePieceHeight);
		shaderReplace.setUniform2f("offsetA", a % xPieces * puzzlePieceWidth, (a / xPieces) * puzzlePieceHeight);
		shaderReplace.setUniform2f("positionA", args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth), args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight));
		shaderReplace.setUniform2f("offsetB", b % xPieces * puzzlePieceWidth, (b / xPieces) * puzzlePieceHeight);
		shaderReplace.setUniform2f("positionB", column * puzzlePieceWidth, row * puzzlePieceHeight);
		ofDrawRectangle(0, 0, puzzleWidth, puzzleHeight);
		shaderReplace.end();
		fbo.end();
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
	if(args.x > 30 && args.x < puzzleWidth + 30 && args.y > 30 && args.y < puzzleHeight + 30){
		isTouch = true;
		mouseIsPressed = true;
		touchX = args.x;
		touchY = args.y;
		row = (args.y - 30) / puzzlePieceHeight;
		column = (args.x - 30) / puzzlePieceWidth;
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & args){
	touchX = args.x;
	touchY = args.y;
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & args){
	if(args.x > 30 && args.x < puzzleWidth + 30 && args.y > 30 && args.y < puzzleHeight + 30 && mouseIsPressed && !isTouch){
		int a = data[row * xPieces + column];
		int b = data[(args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight)) / puzzlePieceHeight * xPieces +  (args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth)) / puzzlePieceWidth];
		data[(args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight)) / puzzlePieceHeight * xPieces +  (args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth)) / puzzlePieceWidth] = a;
		data[row * xPieces + column] = b;
		if(a != b && play){
			moves++;
			label_5.symbol= "Moves: " + ofToString(moves);
		}
		fbo.begin();
		shaderReplace.begin();
		shaderReplace.setUniformTexture("Tex0", fboImg.getTexture(), 0);
		shaderReplace.setUniform2f("resolution", puzzleWidth, puzzleHeight);
		shaderReplace.setUniform2f("puzzlePieceSize", puzzlePieceWidth, puzzlePieceHeight);
		shaderReplace.setUniform2f("offsetA", a % xPieces * puzzlePieceWidth, floor(a / xPieces) * puzzlePieceHeight);
		shaderReplace.setUniform2f("positionA", args.x - 30 - fmodf(args.x - 30, puzzlePieceWidth), args.y - 30 - fmodf(args.y - 30, puzzlePieceHeight));
		shaderReplace.setUniform2f("offsetB", b % xPieces * puzzlePieceWidth, floor(b / xPieces) * puzzlePieceHeight);
		shaderReplace.setUniform2f("positionB", column * puzzlePieceWidth, row * puzzlePieceHeight);
		ofDrawRectangle(0, 0, puzzleWidth, puzzleHeight);
		shaderReplace.end();
		fbo.end();
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
