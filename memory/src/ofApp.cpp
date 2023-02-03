#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Puzzle");
	ofSetBackgroundColor(240, 220, 100);
	puzzleWidth = 800;
	puzzleHeight = 600;
	xPieces = 4;
	yPieces = 3;
	puzzlePieceWidth = puzzleWidth / xPieces;
	puzzlePieceHeight = puzzleHeight / yPieces;
	borderSize = 30;
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(number_2.onMousePressed, this, &ofApp::number_2onMousePressed);
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
	label_7.setup(100, 690, 430, 30, "Pairs: 0");
	ofDisableArbTex();
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
	shader_puzzle.load("puzzle");
	ofLoadImage(texture_puzzle, "wald.jpg");
	image_data.allocate(puzzleWidth, puzzleHeight, OF_IMAGE_COLOR);
	image_data.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	image_duplicates.allocate(puzzleWidth, puzzleHeight, OF_IMAGE_COLOR);
	image_duplicates.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	image_duplicates.setColor(ofFloatColor(1, 1, 0));
	fbo_puzzleState.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fbo_puzzleState.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fbo_puzzleOriginal.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fbo_puzzleOriginal.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fbo_data.allocate(puzzleWidth, puzzleHeight, GL_RGB32F);
	fbo_duplicates.allocate(puzzleWidth, puzzleHeight, GL_RGB32F);
	fbo_puzzlePiece.allocate(puzzleWidth, puzzleHeight, GL_RGBA);
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
	isOpen = false;
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
		image_duplicates.update();
		fbo_duplicates.begin();
		image_duplicates.draw(0, 0, pow(puzzleWidth, 2) / xPieces, pow(puzzleHeight, 2) / yPieces);
		fbo_duplicates.end();
		fbo_puzzleState.begin();
		ofClear(0);
		shader_puzzle.begin();
		shader_puzzle.setUniformTexture("texture_image", fbo_puzzleOriginal.getTexture(), 1);
		shader_puzzle.setUniformTexture("texture_data", fbo_data.getTexture(), 2);
		shader_puzzle.setUniformTexture("texture_duplicates", fbo_duplicates.getTexture(), 3);
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
	fbo_duplicates.draw(650, 660, 160, 90);
	fbo_puzzleState.draw(borderSize, borderSize, puzzleWidth, puzzleHeight);
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
void ofApp::bang_1onMousePressed(bool& e) {
	ofFileDialogResult result = ofSystemLoadDialog("Select an image");
	if (result.bSuccess) {
		ofFile file(result.getPath());
		string fileExtension = ofToUpper(file.getExtension());
		if (fileExtension == "JPG" || fileExtension == "PNG" || fileExtension == "IMG" || fileExtension == "BMP" || fileExtension == "TIF" || fileExtension == "GIF") {
			isPlaying = false;
			ofLoadImage(texture_puzzle, result.getPath());
			label_5.symbol = "Moves: 0";
			label_6.symbol = "Seconds: 0";
			label_7.symbol = "Pairs: 0";
			fbo_puzzleOriginal.begin();
			texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
			fbo_puzzleOriginal.end();
			std::sort(vector_data.begin(), vector_data.begin() + xPieces * yPieces);
			for (float i = 0; i < xPieces * yPieces; i++) {
				image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
			}
			image_duplicates.setColor(ofFloatColor(1, 1, 0));
			isPuzzleShader = true;
			isOpen = false;
		}
	}
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool& e) {
	isPlaying = true;
	playTime = ofGetElapsedTimef();
	moves = 0;
	label_5.symbol = "Moves: 0";
	label_6.symbol = "Seconds: 0";
	label_7.symbol = "Pairs: 0";
	std::shuffle(vector_data.begin(), vector_data.begin() + xPieces * yPieces, rng);
	vector_small_data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		vector_small_data.push_back(vector_data[i/2]);
		std::shuffle(vector_small_data.begin(), vector_small_data.end(), rng);
	}
	for (int i = 0; i < xPieces * yPieces; i++) {
		image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(vector_small_data[i], xPieces) / xPieces, floor(vector_small_data[i] / xPieces) / yPieces, 0));
	}
	image_duplicates.setColor(0);
	isPuzzleShader = true;
	isOpen = false;
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float& e) {
	if (e != xPieces) {
		isPlaying = false;
		label_5.symbol = "Moves: 0";
		label_6.symbol = "Seconds: 0";
		label_7.symbol = "Pairs: 0";
		std::sort(vector_data.begin(), vector_data.begin() + xPieces * yPieces);
		xPieces = e;
		puzzlePieceWidth = puzzleWidth / xPieces;
		for (int i = 0; i < xPieces * yPieces; i++) {
			image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
		}
		image_duplicates.setColor(ofFloatColor(1, 1, 0));
		isPuzzleShader = true;
		isOpen = false;
	}
}

//--------------------------------------------------------------
void ofApp::number_2onMousePressed(float& e) {
	if (e != yPieces) {
		isPlaying = false;
		label_5.symbol = "Moves: 0";
		label_6.symbol = "Seconds: 0";
		label_7.symbol = "Pairs: 0";
		std::sort(vector_data.begin(), vector_data.begin() + xPieces * yPieces);
		yPieces = e;
		puzzlePieceHeight = puzzleHeight / yPieces;
		for (int i = 0; i < xPieces * yPieces; i++) {
			image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
		}
		image_duplicates.setColor(ofFloatColor(1, 1, 0));
		isPuzzleShader = true;
		isOpen = false;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs& args) {
	if (args.x > borderSize && args.x < puzzleWidth + borderSize && args.y > borderSize && args.y < puzzleHeight + borderSize && isPlaying && !isTouch) {
		row = (args.y - borderSize) / puzzlePieceHeight;
		column = (args.x - borderSize) / puzzlePieceWidth;
			if (image_duplicates.getColor(column, row).g == 0) {
				if (!isOpen) {
					image_duplicates.setColor(column, row, ofFloatColor(0, 0, 1, 1));
					oldPosition = vector_small_data[row * xPieces + column];
					oldRow = row;
					oldColumn = column;
					moves++;
					isOpen = true;
				}
				else {
					if (oldPosition == vector_small_data[row * xPieces + column] && oldRow * xPieces + oldColumn != row * xPieces + column) {
						image_duplicates.setColor(oldColumn, oldRow, ofFloatColor(1, 1, 0, 1));
						image_duplicates.setColor(column, row, ofFloatColor(1, 1, 0, 1));
						moves++;
						isOpen = false;
					}
					else if (image_duplicates.getColor(column, row).b == 1) {
						image_duplicates.setColor(column, row, ofFloatColor(0, 0, 0, 1));
						isOpen = false;
					}
					else {
						image_duplicates.setColor(column, row, ofFloatColor(0, 1, 1, 1));
						moves++;
					}
				}
				isPuzzleShader = true;
				label_5.symbol = "Moves: " + ofToString(moves);
			}
			bool win = true;
			int pairs = 0;
			for (int i = 0; i < xPieces * yPieces; i++) {
				if (image_duplicates.getColor(fmodf(i, xPieces), floor(i / xPieces)).g == 0) {
					win = false;
				}
				else {
					pairs++;
				}
			}
			if (win && isPlaying) {
				isPlaying = false;
				playTime = floor(ofGetElapsedTimef() - playTime);
				label_7.symbol = ofToString("You won in " + ofToString(playTime) + " seconds and with " + ofToString(moves) + " moves!");
				moves = 0;
				std::sort(vector_data.begin(), vector_data.begin() + xPieces * yPieces);
				for (int i = 0; i < xPieces * yPieces; i++) {
					if (i < xPieces * yPieces) {
						image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
					}
				}
				isPuzzleShader = true;
			}
			else {
				label_7.symbol = "Pairs: " + ofToString(pairs / 2);
			}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs& args) {
	if (image_duplicates.getColor(column, row).g == 1 && image_duplicates.getColor(column, row).b == 1) {
		image_duplicates.setColor(column, row, ofFloatColor(0, 0, 0, 1));
		isPuzzleShader = true;
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
	if (args.x > borderSize && args.x < puzzleWidth + borderSize && args.y > borderSize && args.y < puzzleHeight + borderSize && isPlaying) {
		row = (args.y - borderSize) / puzzlePieceHeight;
		column = (args.x - borderSize) / puzzlePieceWidth;
		if (image_duplicates.getColor(column, row).g == 0) {
			if (!isOpen) {
				image_duplicates.setColor(column, row, ofFloatColor(0, 0, 1, 1));
				oldPosition = vector_small_data[row * xPieces + column];
				oldRow = row;
				oldColumn = column;
				moves++;
				isOpen = true;
			}
			else {
				if (oldPosition == vector_small_data[row * xPieces + column] && oldRow * xPieces + oldColumn != row * xPieces + column) {
					image_duplicates.setColor(oldColumn, oldRow, ofFloatColor(1, 1, 0, 1));
					image_duplicates.setColor(column, row, ofFloatColor(1, 1, 0, 1));
					moves++;
					isOpen = false;
				}
				else if (image_duplicates.getColor(column, row).b == 1) {
					image_duplicates.setColor(column, row, ofFloatColor(0, 0, 0, 1));
					isOpen = false;
				}
				else {
					image_duplicates.setColor(column, row, ofFloatColor(0, 1, 1, 1));
					moves++;
				}
			}
			isPuzzleShader = true;
			label_5.symbol = "Moves: " + ofToString(moves);
		}
		bool win = true;
		int pairs = 0;
		for (int i = 0; i < xPieces * yPieces; i++) {
			if (image_duplicates.getColor(fmodf(i, xPieces), floor(i / xPieces)).g == 0) {
				win = false;
			}
			else {
				pairs++;
			}
		}
		if (win && isPlaying) {
			isPlaying = false;
			playTime = floor(ofGetElapsedTimef() - playTime);
			label_7.symbol = ofToString("You won in " + ofToString(playTime) + " seconds and with " + ofToString(moves) + " moves!");
			moves = 0;
			std::sort(vector_data.begin(), vector_data.begin() + xPieces * yPieces);
			for (int i = 0; i < xPieces * yPieces; i++) {
				if (i < xPieces * yPieces) {
					image_data.setColor(fmodf(i, xPieces), i / xPieces, ofFloatColor(fmodf(i, xPieces) / xPieces, floor(i / xPieces) / yPieces, 0));
				}
			}
			isPuzzleShader = true;
		}
		else {
			label_7.symbol = "Pairs: " + ofToString(pairs / 2);
		}
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs& args) {
	touchX = args.x;
	touchY = args.y;
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs& args) {
	if (image_duplicates.getColor(column, row).g == 1 && image_duplicates.getColor(column, row).b == 1) {
		image_duplicates.setColor(column, row, ofFloatColor(0, 0, 0, 1));
		isPuzzleShader = true;
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
