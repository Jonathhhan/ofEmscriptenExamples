#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Puzzle");
	ofSetBackgroundColor(240, 220, 100);
	puzzleWidthScreen = 800;
	puzzleHeightScreen = 600;
	scale = 2;
	puzzleWidth = puzzleWidthScreen * scale;
	puzzleHeight = puzzleHeightScreen * scale;
	xPieces = 4;
	yPieces = 2;
	puzzlePieceWidth = puzzleWidth / xPieces;
	puzzlePieceHeight = puzzleHeight / yPieces;
	puzzlePieceWidthScreen = puzzleWidthScreen / xPieces;
	puzzlePieceHeightScreen = puzzleHeightScreen / yPieces;
	puzzlePieceDiv = 10;
	puzzlePieceDivHalf = puzzlePieceDiv / 2;
	borderSize = 30;
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(number_2.onMousePressed, this, &ofApp::number_2onMousePressed);
	bang_1.setup(40, 650, 50);
	bang_2.setup(40, 710, 50);
	number_1.setup(210, 650, 100, 30, 1, 100);
	number_1.value = xPieces;
	number_2.setup(210, 730, 100, 30, 1, 100);
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
	image_data.allocate(puzzleWidthScreen, puzzleHeightScreen, OF_IMAGE_COLOR);
	image_data.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	ofLoadImage(texture_mask_1, "mask_1.png");
	ofLoadImage(texture_mask_2, "mask_2.png");
	ofLoadImage(texture_mask_3, "mask_3.png");
	fbo_puzzleState.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fbo_puzzleState.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	fbo_puzzleState.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fbo_puzzleOriginal.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fbo_puzzleOriginal.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	fbo_puzzleOriginal.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fbo_data.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fbo_data.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	fbo_data.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fbo_mask_1.allocate(puzzleWidth, puzzleHeight, GL_RGBA);
	fbo_puzzlePiece.allocate(puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf, GL_RGBA);
	fbo_puzzlePiece.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	fbo_emptyPuzzlePiece.allocate(puzzleWidth, puzzleHeight, GL_RGB);
	fbo_emptyPuzzlePiece.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	for (int i = 0; i < xPieces * yPieces; i++) {
		vector_data.push_back(std::make_pair(i % xPieces, i / xPieces));
		image_data.setColor(i % xPieces, i / xPieces, ofColor(i % xPieces * 255 / xPieces, i / xPieces * 255 / yPieces, 0));
	}
	image_data.update();
	fbo_data.begin();
	image_data.draw(0, 0, pow(puzzleWidthScreen,2) / xPieces * scale, pow(puzzleHeightScreen,2) / yPieces * scale);
	fbo_data.end();
	fbo_puzzleState.begin();
	texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo_puzzleState.end();
	fbo_puzzleOriginal.begin();
	texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo_puzzleOriginal.end();
	fbo_mask_1.begin();
	texture_mask_1.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo_mask_1.end();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (play) {
		label_6.symbol = "Seconds: " + ofToString(floor(ofGetElapsedTimef() - playTime));
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
	fbo_puzzleState.draw(borderSize, borderSize, puzzleWidthScreen, puzzleHeightScreen);
	if (mouseIsPressed) {
		if (!isTouch) {
			fbo_emptyPuzzlePiece.draw(borderSize, borderSize, puzzleWidthScreen, puzzleHeightScreen);
			fbo_puzzlePiece.draw(ofGetMouseX() - (puzzlePieceWidthScreen + puzzlePieceWidthScreen / puzzlePieceDivHalf) / 2, ofGetMouseY() - (puzzlePieceHeightScreen + puzzlePieceHeightScreen / puzzlePieceDivHalf) / 2, puzzlePieceWidthScreen + puzzlePieceWidthScreen / puzzlePieceDivHalf, puzzlePieceHeightScreen + puzzlePieceHeightScreen / puzzlePieceDivHalf);
		}
		else {
			fbo_emptyPuzzlePiece.draw(borderSize, borderSize, puzzleWidthScreen, puzzleHeightScreen);
			fbo_puzzlePiece.draw(touchX - (puzzlePieceWidthScreen + puzzlePieceWidthScreen / puzzlePieceDivHalf) / 2, touchY - (puzzlePieceHeightScreen + puzzlePieceHeightScreen / puzzlePieceDivHalf) / 2, puzzlePieceWidthScreen + puzzlePieceWidthScreen / puzzlePieceDivHalf, puzzlePieceHeightScreen + puzzlePieceHeightScreen / puzzlePieceDivHalf);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs& args) {

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
			ofLoadImage(texture_puzzle, result.getPath());
			fbo_puzzleState.begin();
			texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
			fbo_puzzleState.end();
			fbo_puzzleOriginal.begin();
			texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
			fbo_puzzleOriginal.end();
			vector_data.clear();
			for (int i = 0; i < xPieces * yPieces; i++) {
				vector_data.push_back(std::make_pair(i % xPieces, i / xPieces));
				image_data.setColor(i % xPieces, i / xPieces, ofColor(i % xPieces * 255 / xPieces, i / xPieces * 255 / yPieces, 0, 255));
			}
			image_data.update();
			fbo_data.begin();
			image_data.draw(0, 0, pow(puzzleWidthScreen, 2) / xPieces * scale, pow(puzzleHeightScreen, 2) / yPieces * scale);
			fbo_data.end();
			play = false;
		}
	}
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool& e) {
	play = true;
	playTime = ofGetElapsedTimef();
	moves = 0;
	label_5.symbol = "Moves: " + ofToString(moves);
	label_6.symbol = "Seconds: " + ofToString(floor(ofGetElapsedTimef() - playTime));
	label_7.symbol = "Original positions: 0";
	std::shuffle(vector_data.begin(), vector_data.end(), rng);
	int originalPositions = 0;
	for (int i = 0; i < xPieces * yPieces; i++) {
		image_data.setColor(i % xPieces, i / xPieces, ofColor(vector_data[i].first * 255 / xPieces, vector_data[i].second * 255 / yPieces, 0));
		if (vector_data[i].first == i % xPieces && vector_data[i].second == i / xPieces) {
			originalPositions++;
		}
	}
	label_7.symbol = "Original positions: " + ofToString(originalPositions);
	image_data.update();
	fbo_data.begin();
	image_data.draw(0, 0, pow(puzzleWidthScreen, 2) / xPieces * scale, pow(puzzleHeightScreen, 2) / yPieces * scale);
	fbo_data.end();
	fbo_puzzleState.begin();
	ofClear(0);
	shader_puzzle.begin();
	shader_puzzle.setUniformTexture("texture_image", fbo_puzzleOriginal.getTexture(), 1);
	shader_puzzle.setUniformTexture("texture_data", fbo_data.getTexture(), 2);
	shader_puzzle.setUniformTexture("texture_mask", fbo_mask_1.getTexture(), 3);
	shader_puzzle.setUniform2f("resolution", puzzleWidth, puzzleHeight);
	shader_puzzle.setUniform2f("puzzlePieces", xPieces, yPieces);
	shader_puzzle.setUniform2f("puzzlePieceSize", puzzleWidth / xPieces, puzzleHeight / yPieces);
	ofDrawRectangle(0, 0, puzzleWidth, puzzleHeight);
	shader_puzzle.end();
	fbo_puzzleState.end();
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float& e) {
	play = false;
	label_5.symbol = "Moves: " + ofToString(0);
	label_6.symbol = "Seconds: " + ofToString(0);
	xPieces = e;
	vector_data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		vector_data.push_back(std::make_pair(i % xPieces, i / xPieces));
		image_data.setColor(i % xPieces, i / xPieces, ofColor(i % xPieces * 255 / xPieces, i / xPieces * 255 / yPieces, 0));
	}
	image_data.update();
	fbo_data.begin();
	image_data.draw(0, 0, pow(puzzleWidthScreen, 2) / xPieces * scale, pow(puzzleHeightScreen, 2) / yPieces * scale);
	fbo_data.end();
	puzzlePieceWidth = puzzleWidth / xPieces;
	puzzlePieceWidthScreen = puzzleWidthScreen / xPieces;
	fbo_puzzlePiece.allocate(puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf, GL_RGBA);
	fbo_puzzleState.begin();
	texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo_puzzleState.end();
}

//--------------------------------------------------------------
void ofApp::number_2onMousePressed(float& e) {
	play = false;
	label_5.symbol = "Moves: " + ofToString(0);
	label_6.symbol = "Seconds: " + ofToString(0);
	yPieces = e;
	vector_data.clear();
	for (int i = 0; i < xPieces * yPieces; i++) {
		vector_data.push_back(std::make_pair(i % xPieces, i / xPieces));
		image_data.setColor(i % xPieces, i / xPieces, ofColor(i % xPieces * 255 / xPieces, i / xPieces * 255 / yPieces, 0));
	}
	image_data.update();
	fbo_data.begin();
	image_data.draw(0, 0, pow(puzzleWidthScreen, 2) / xPieces * scale, pow(puzzleHeightScreen, 2) / yPieces * scale);
	fbo_data.end();
	puzzlePieceHeight = puzzleHeight / yPieces;
	puzzlePieceHeightScreen = puzzleHeightScreen / yPieces;
	fbo_puzzlePiece.allocate(puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf, GL_RGBA);
	fbo_puzzleState.begin();
	texture_puzzle.draw(0, 0, puzzleWidth, puzzleHeight);
	fbo_puzzleState.end();
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs& args) {
	if (args.x > borderSize && args.x < puzzleWidthScreen + borderSize && args.y > borderSize && args.y < puzzleHeightScreen + borderSize && !isTouch) {
		mouseIsPressed = true;
		row = (args.y - borderSize) / puzzlePieceHeightScreen;
		column = (args.x - borderSize) / puzzlePieceWidthScreen;
		fbo_puzzlePiece.begin();
		fbo_puzzleState.getTexture().drawSubsection(0, 0, puzzleWidth + puzzleWidth / puzzlePieceDivHalf, puzzleHeight + puzzleHeight / puzzlePieceDivHalf, column * puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, row * puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv);
		fbo_puzzlePiece.end();
		fbo_puzzlePiece.getTexture().setAlphaMask(texture_mask_2);
		fbo_emptyPuzzlePiece.begin();
		fbo_puzzleState.draw(0, 0);
		texture_mask_3.draw(column * puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, row * puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		if (column == xPieces - 1 && xPieces > 1) {
			texture_mask_3.draw(-puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, row * puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		}
		else if (column == 0 && xPieces > 1) {
			texture_mask_3.draw((xPieces)*puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, row * puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		}
		if (row == yPieces - 1 && yPieces > 1) {
			texture_mask_3.draw(column * puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, -puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		}
		else if (row == 0 && yPieces > 1) {
			texture_mask_3.draw(column * puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, (yPieces)*puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		}
		fbo_emptyPuzzlePiece.end();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs& args) {
	if (mouseIsPressed && !isTouch) {
		if (args.x > borderSize && args.x < puzzleWidthScreen + borderSize && args.y > borderSize && args.y < puzzleHeightScreen + borderSize) {
			int a1 = vector_data[row * xPieces + column].first;
			int a2 = vector_data[row * xPieces + column].second;
			int b1 = vector_data[(args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen * xPieces + (args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen].first;
			int b2 = vector_data[(args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen * xPieces + (args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen].second;
			vector_data[(args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen * xPieces + (args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen].first = a1;
			vector_data[(args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen * xPieces + (args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen].second = a2;
			vector_data[row * xPieces + column].first = b1;
			vector_data[row * xPieces + column].second = b2;
			if (play && (a1 != b1 || a2 != b2)) {
				moves++;
				label_5.symbol = "Moves: " + ofToString(moves);
			}
			image_data.setColor((args.x - 30 - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen, (args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen, ofColor(a1 * 255 / xPieces, a2 * 255 / yPieces, 0));
			image_data.setColor(column, row, ofColor(b1 * 255 / xPieces, b2 * 255 / yPieces, 0));
			image_data.update();
			fbo_data.begin();
			image_data.draw(0, 0, pow(puzzleWidthScreen, 2) / xPieces * scale, pow(puzzleHeightScreen, 2) / yPieces * scale);
			fbo_data.end();
			fbo_puzzleState.begin();
			shader_puzzle.begin();
			shader_puzzle.setUniformTexture("texture_image", fbo_puzzleOriginal.getTexture(), 1);
			shader_puzzle.setUniformTexture("texture_data", fbo_data.getTexture(), 2);
			shader_puzzle.setUniformTexture("texture_mask", fbo_mask_1.getTexture(), 3);
			shader_puzzle.setUniform2f("resolution", puzzleWidth, puzzleHeight);
			shader_puzzle.setUniform2f("puzzlePieces", xPieces, yPieces);
			shader_puzzle.setUniform2f("puzzlePieceSize", puzzleWidth / xPieces, puzzleHeight / yPieces);
			ofDrawRectangle(0, 0, puzzleWidth, puzzleHeight);
			shader_puzzle.end();
			fbo_puzzleState.end();
			bool win = true;
			int originalPositions = 0;
			for (int i = 0; i < xPieces * yPieces; i++) {
				if (vector_data[i].first != i % xPieces || vector_data[i].second != i / xPieces) {
					win = false;
				}
				else {
					originalPositions++;
				}
			}
			if (win && play) {
				play = false;
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
	if (args.x > borderSize && args.x < puzzleWidthScreen + borderSize && args.y > borderSize && args.y < puzzleHeightScreen + borderSize && !isTouch) {
		isTouch = true;
		mouseIsPressed = true;
		row = (args.y - borderSize) / puzzlePieceHeightScreen;
		column = (args.x - borderSize) / puzzlePieceWidthScreen;
		fbo_puzzlePiece.begin();
		fbo_puzzleState.getTexture().drawSubsection(0, 0, puzzleWidth + puzzleWidth / puzzlePieceDivHalf, puzzleHeight + puzzleHeight / puzzlePieceDivHalf, column * puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, row * puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv);
		fbo_puzzlePiece.end();
		fbo_puzzlePiece.getTexture().setAlphaMask(texture_mask_2);
		fbo_emptyPuzzlePiece.begin();
		fbo_puzzleState.draw(0, 0);
		texture_mask_3.draw(column * puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, row * puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		if (column == xPieces - 1 && xPieces > 1) {
			texture_mask_3.draw(-puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, row * puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		}
		else if (column == 0 && xPieces > 1) {
			texture_mask_3.draw((xPieces)*puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, row * puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		}
		if (row == yPieces - 1 && yPieces > 1) {
			texture_mask_3.draw(column * puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, -puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		}
		else if (row == 0 && yPieces > 1) {
			texture_mask_3.draw(column * puzzlePieceWidth - puzzlePieceWidth / puzzlePieceDiv, (yPieces)*puzzlePieceHeight - puzzlePieceHeight / puzzlePieceDiv, puzzlePieceWidth + puzzlePieceWidth / puzzlePieceDivHalf, puzzlePieceHeight + puzzlePieceHeight / puzzlePieceDivHalf);
		}
		fbo_emptyPuzzlePiece.end();
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
		if (args.x > borderSize && args.x < puzzleWidthScreen + borderSize && args.y > borderSize && args.y < puzzleHeightScreen + borderSize) {
			int a1 = vector_data[row * xPieces + column].first;
			int a2 = vector_data[row * xPieces + column].second;
			int b1 = vector_data[(args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen * xPieces + (args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen].first;
			int b2 = vector_data[(args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen * xPieces + (args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen].second;
			vector_data[(args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen * xPieces + (args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen].first = a1;
			vector_data[(args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen * xPieces + (args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen].second = a2;
			vector_data[row * xPieces + column].first = b1;
			vector_data[row * xPieces + column].second = b2;
			if (play && (a1 != b1 || a2 != b2)) {
				moves++;
				label_5.symbol = "Moves: " + ofToString(moves);
			}
			image_data.setColor((args.x - borderSize - fmodf(args.x - borderSize, puzzlePieceWidthScreen)) / puzzlePieceWidthScreen, (args.y - borderSize - fmodf(args.y - borderSize, puzzlePieceHeightScreen)) / puzzlePieceHeightScreen, ofColor(a1 * 255 / xPieces, a2 * 255 / yPieces, 0));
			image_data.setColor(column, row, ofColor(b1 * 255 / xPieces, b2 * 255 / yPieces, 0));
			image_data.update();
			fbo_data.begin();
			image_data.draw(0, 0, pow(puzzleWidthScreen, 2) / xPieces * scale, pow(puzzleHeightScreen, 2) / yPieces * scale);
			fbo_data.end();
			fbo_puzzleState.begin();
			shader_puzzle.begin();
			shader_puzzle.setUniformTexture("texture_image", fbo_puzzleOriginal.getTexture(), 1);
			shader_puzzle.setUniformTexture("texture_data", fbo_data.getTexture(), 2);
			shader_puzzle.setUniformTexture("texture_mask", fbo_mask_1.getTexture(), 3);
			shader_puzzle.setUniform2f("resolution", puzzleWidth, puzzleHeight);
			shader_puzzle.setUniform2f("puzzlePieces", xPieces, yPieces);
			shader_puzzle.setUniform2f("puzzlePieceSize", puzzleWidth / xPieces, puzzleHeight / yPieces);
			ofDrawRectangle(0, 0, puzzleWidth, puzzleHeight);
			shader_puzzle.end();
			fbo_puzzleState.end();
			bool win = true;
			int originalPositions = 0;
			for (int i = 0; i < xPieces * yPieces; i++) {
				if (vector_data[i].first != i % xPieces || vector_data[i].second != i / xPieces) {
					win = false;
				}
				else {
					originalPositions++;
				}
			}
			if (win && play) {
				play = false;
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