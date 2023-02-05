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
	ofLoadImage(texture_puzzle, "data");
	EM_ASM(FS.unlink("/data/data"));
	isPlaying = false;
	label_5.symbol = "Moves: " + ofToString(0);
	label_6.symbol = "Seconds: " + ofToString(0);
	label_7.symbol = "Original positions: " + ofToString(xPieces * yPieces);
	fbo_puzzleOriginal.begin();
	ofClear(0);
	texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo_puzzleOriginal.end();
	std::sort(vector_data.begin(), vector_data.begin() + xPieces * yPieces);
	for (float i = 0; i < xPieces * yPieces; i++) {
		image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
	}
	isPuzzleShader = true;
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Puzzle");
	ofSetBackgroundColor(240, 220, 100);
	puzzleWidth = 800;
	puzzleHeight = 600;
	xPieces = 4;
	yPieces = 2;
	puzzlePieceWidth = puzzleWidth / xPieces;
	puzzlePieceHeight = puzzleHeight / yPieces;
	borderSize = 30;
	emscripten::function("loadImage", &loadImage);
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(number_2.onMousePressed, this, &ofApp::number_2onMousePressed);
	ofAddListener(loadImageEvent, this, & ofApp::loadImageX);
	bang_1.setup(40, 650, 50);
	bang_2.setup(40, 710, 50);
	number_1.setup(210, 650, 100, 30, 1, 50);
	number_1.value = xPieces;
	number_2.setup(210, 730, 100, 30, 1, 50);
	number_2.value = yPieces;
	label_1.setup(100, 650, 100, 30, "Load image");
	label_2.setup(100, 730, 100, 30, "Play");
	label_3.setup(320, 650, 100, 30, "Pieces X");
	label_4.setup(320, 730, 100, 30, "Pieces Y");
	label_5.setup(430, 650, 200, 30, "Moves: 0");
	label_6.setup(430, 730, 200, 30, "Seconds: 0");
	label_7.setup(100, 690, 430, 30, "Original positions: " + ofToString(xPieces * yPieces));
	ofDisableArbTex();
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
	shader_puzzle.load("puzzle");
	ofLoadImage(texture_puzzle, "wald.jpg");
	image_data.allocate(puzzleWidth, puzzleHeight, OF_IMAGE_COLOR);
	image_data.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	fbo_puzzleState.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fbo_puzzleState.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fbo_puzzleOriginal.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fbo_puzzleOriginal.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fbo_data.allocate(puzzleWidth, puzzleHeight, GL_RGBA32F);
	fbo_puzzlePiece.allocate(puzzleWidth, puzzleHeight, GL_RGBA);
	fbo_puzzleState.begin();
	texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo_puzzleState.end();
	fbo_puzzleOriginal.begin();
	texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo_puzzleOriginal.end();
	for (int i = 0; i < 2500; i++) {
		vector_data.push_back(i);
		if (i < xPieces * yPieces) {
			image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
		}
	}
	isPuzzleShader = true;
}

//--------------------------------------------------------------
void ofApp::update() {
	if (isPlaying) {
		label_6.symbol = "Seconds: " + ofToString(floor(ofGetElapsedTimef() - playTime));
	}
	if (isPuzzleShader) {
		isPuzzleShader = false;
		image_data.update();
		fbo_data.begin();
		image_data.draw(0, 0, pow(puzzleWidth, 2) / xPieces, pow(puzzleHeight, 2) / yPieces);
		fbo_data.end();
		fbo_puzzleState.begin();
		ofClear(0);
		shader_puzzle.begin();
		shader_puzzle.setUniformTexture("texture_image", fbo_puzzleOriginal.getTexture(), 1);
		shader_puzzle.setUniformTexture("texture_data", fbo_data.getTexture(), 2);
		shader_puzzle.setUniform2f("resolution", puzzleWidth, puzzleHeight);
		shader_puzzle.setUniform2f("puzzlePieces", xPieces, yPieces);
		shader_puzzle.setUniform2f("puzzlePieceSize", puzzleWidth / xPieces, puzzleHeight / yPieces);
		ofDrawRectangle(0, 0, puzzleWidth, puzzleHeight);
		shader_puzzle.end();
		fbo_puzzleState.end();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0);
	ofDrawRectangle(20, 20, 820, 760);
	ofSetColor(20, 170, 100);
	ofDrawRectangle(30, 640, 800, 130);
	ofSetColor(0);
	ofDrawRectangle(640, 650, 180, 110);
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
	label_7.draw();
	ofSetColor(255);
	fbo_data.draw(650, 660, 160, 90);
	fbo_puzzleState.draw(borderSize, borderSize, puzzleWidth, puzzleHeight);
	if (mouseIsPressed) {
		ofSetColor(0);
		ofDrawRectangle(column * puzzlePieceWidth + borderSize, row * puzzlePieceHeight + borderSize, puzzlePieceWidth, puzzlePieceHeight);
		ofSetColor(255);
		if (!isTouch) {
			fbo_puzzlePiece.draw(ofGetMouseX() - puzzlePieceWidth / 2, ofGetMouseY() - puzzlePieceHeight / 2, puzzlePieceWidth, puzzlePieceHeight);
		}
		else {
			fbo_puzzlePiece.draw(touchX - puzzlePieceWidth / 2, touchY - puzzlePieceHeight / 2, puzzlePieceWidth, puzzlePieceHeight);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs& args) {
	shader_puzzle.load("puzzle");
}

//--------------------------------------------------------------
void ofApp::keyReleased(ofKeyEventArgs& args) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs& args) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs& args) {

}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool& e) {
	isPlaying = true;
	playTime = ofGetElapsedTimef();
	moves = 0;
	label_5.symbol = "Moves: " + ofToString(moves);
	label_6.symbol = "Seconds: " + ofToString(floor(ofGetElapsedTimef() - playTime));
	label_7.symbol = "Original positions: " + ofToString(xPieces * yPieces);
	std::shuffle(vector_data.begin(), vector_data.begin() + xPieces * yPieces, rng);
	for (int i = 0; i < xPieces * yPieces; i++) {
		image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(vector_data[i], xPieces) / xPieces, floor(vector_data[i] / xPieces) / yPieces, 0));
	}
	isPuzzleShader = true;
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float& e) {
	if (e != xPieces) {
		isPlaying = false;
		label_5.symbol = "Moves: " + ofToString(0);
		label_6.symbol = "Seconds: " + ofToString(0);
		label_7.symbol = "Original positions: " + ofToString(e * yPieces);
		std::sort(vector_data.begin(), vector_data.begin() + xPieces * yPieces);
		xPieces = e;
		puzzlePieceWidth = puzzleWidth / xPieces;
		for (int i = 0; i < xPieces * yPieces; i++) {
			image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
		}
		isPuzzleShader = true;
	}
}

//--------------------------------------------------------------
void ofApp::number_2onMousePressed(float& e) {
	if (e != yPieces) {
		isPlaying = false;
		label_5.symbol = "Moves: " + ofToString(0);
		label_6.symbol = "Seconds: " + ofToString(0);
		label_7.symbol = "Original positions: " + ofToString(xPieces * e);
		std::sort(vector_data.begin(), vector_data.begin() + xPieces * yPieces);
		yPieces = e;
		puzzlePieceHeight = puzzleHeight / yPieces;
		for (int i = 0; i < xPieces * yPieces; i++) {
			image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
		}
		isPuzzleShader = true;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs& args) {
	if (args.x > borderSize && args.x < puzzleWidth + borderSize && args.y > borderSize && args.y < puzzleHeight + borderSize && !isTouch) {
		mouseIsPressed = true;
		row = (args.y - borderSize) / puzzlePieceHeight;
		column = (args.x - borderSize) / puzzlePieceWidth;
		fbo_puzzlePiece.begin();
		fbo_puzzleState.draw(-column * puzzleWidth, -row * puzzleHeight, puzzleWidth * xPieces, puzzleHeight * yPieces);
		fbo_puzzlePiece.end();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs& args) {
	if (mouseIsPressed && !isTouch) {
		if (args.x > borderSize && args.x < puzzleWidth + borderSize && args.y > borderSize && args.y < puzzleHeight + borderSize) {
			int oldPosition = vector_data[row * xPieces + column];
			int newPosition = vector_data[floor((args.y - borderSize) / puzzlePieceHeight) * xPieces + floor((args.x - borderSize) / puzzlePieceWidth)];
			vector_data[floor((args.y - borderSize) / puzzlePieceHeight) * xPieces + floor((args.x - borderSize) / puzzlePieceWidth)] = oldPosition;
			vector_data[row * xPieces + column] = newPosition;
			if (isPlaying && (oldPosition != newPosition)) {
				moves++;
				label_5.symbol = "Moves: " + ofToString(moves);
			}
			image_data.setColor(floor((args.x - borderSize) / puzzlePieceWidth), floor((args.y - borderSize) / puzzlePieceHeight), ofFloatColor(fmodf(oldPosition, xPieces) / xPieces, floor(oldPosition / xPieces) / yPieces, 0));
			image_data.setColor(column, row, ofFloatColor(fmodf(newPosition, xPieces) / xPieces, floor(newPosition / xPieces) / yPieces, 0));
			isPuzzleShader = true;
			bool win = true;
			int originalPositions = 0;
			for (int i = 0; i < xPieces * yPieces; i++) {
				if (vector_data[i] != i) {
					win = false;
				}
				else {
					originalPositions++;
				}
			}
			if (win && isPlaying) {
				isPlaying = false;
				playTime = floor(ofGetElapsedTimef() - playTime);
				label_7.symbol = ofToString("You won in " + ofToString(playTime) + " seconds and with " + ofToString(moves) + " moves!");
				moves = 0;
			}
			else {
				label_7.symbol = "Original positions: " + ofToString(originalPositions);
			}
			mouseIsPressed = false;
		}
		else {
			mouseIsPressed = false;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(ofMouseEventArgs& args) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(ofMouseEventArgs& args) {

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs& args) {
	if (args.x > borderSize && args.x < puzzleWidth + borderSize && args.y > borderSize && args.y < puzzleHeight + borderSize) {
		mouseIsPressed = true;
		isTouch = true;
		touchX = args.x;
		touchY = args.y;
		row = (args.y - borderSize) / puzzlePieceHeight;
		column = (args.x - borderSize) / puzzlePieceWidth;
		fbo_puzzlePiece.begin();
		fbo_puzzleState.draw(-column * puzzleWidth, -row * puzzleHeight, puzzleWidth * xPieces, puzzleHeight * yPieces);
		fbo_puzzlePiece.end();
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs& args) {
	touchX = args.x;
	touchY = args.y;
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs& args) {
	if (mouseIsPressed) {
		if (args.x > borderSize && args.x < puzzleWidth + borderSize && args.y > borderSize && args.y < puzzleHeight + borderSize) {
			int oldPosition = vector_data[row * xPieces + column];
			int newPosition = vector_data[floor((args.y - borderSize) / puzzlePieceHeight) * xPieces + floor((args.x - borderSize) / puzzlePieceWidth)];
			vector_data[floor((args.y - borderSize) / puzzlePieceHeight) * xPieces + floor((args.x - borderSize) / puzzlePieceWidth)] = oldPosition;
			vector_data[row * xPieces + column] = newPosition;
			if (isPlaying && (oldPosition != newPosition)) {
				moves++;
				label_5.symbol = "Moves: " + ofToString(moves);
			}
			image_data.setColor(floor((args.x - borderSize) / puzzlePieceWidth), floor((args.y - borderSize) / puzzlePieceHeight), ofFloatColor(fmodf(oldPosition, xPieces) / xPieces, floor(oldPosition / xPieces) / yPieces, 0));
			image_data.setColor(column, row, ofFloatColor(fmodf(newPosition, xPieces) / xPieces, floor(newPosition / xPieces) / yPieces, 0));
			isPuzzleShader = true;
			bool win = true;
			int originalPositions = 0;
			for (int i = 0; i < xPieces * yPieces; i++) {
				if (vector_data[i] != i) {
					win = false;
				}
				else {
					originalPositions++;
				}
			}
			if (win && isPlaying) {
				isPlaying = false;
				playTime = floor(ofGetElapsedTimef() - playTime);
				label_7.symbol = ofToString("You won in " + ofToString(playTime) + " seconds and with " + ofToString(moves) + " moves!");
				moves = 0;
			}
			else {
				label_7.symbol = "Original positions: " + ofToString(originalPositions);
			}
			mouseIsPressed = false;
		}
		else {
			mouseIsPressed = false;
		}
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
