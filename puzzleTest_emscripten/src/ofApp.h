#pragma once

#include "ofMain.h"
#include "bang.h"
#include "number.h"
#include "label.h"
#include "random"
#include "emscripten.h"
#include "emscripten/bind.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(ofKeyEventArgs& args);
	void keyReleased(ofKeyEventArgs& args);
	void mouseMoved(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void mouseEntered(ofMouseEventArgs& args);
	void mouseExited(ofMouseEventArgs& args);
	void touchMoved(ofTouchEventArgs& args);
	void touchDown(ofTouchEventArgs& args);
	void touchUp(ofTouchEventArgs& args);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void bang_1onMousePressed(bool& e);
	void bang_2onMousePressed(bool& e);
	void number_1onMousePressed(float& e);
	void number_2onMousePressed(float& e);
	void loadImageX();

	bang bang_1;
	bang bang_2;
	number number_1;
	number number_2;
	label label_1;
	label label_2;
	label label_3;
	label label_4;
	label label_5;
	label label_6;
	label label_7;
	ofShader shader_puzzle;
	ofFloatImage image_data;
	ofTexture texture_puzzle;
	ofTexture texture_mask_1;
	ofTexture texture_mask_2;
	ofTexture texture_mask_3;
	ofFbo fbo_puzzleOriginal;
	ofFbo fbo_puzzleState;
	ofFbo fbo_data;
	ofFbo fbo_mask_1;
	ofFbo fbo_puzzlePiece;
	ofFbo fbo_emptyPuzzlePiece;
	std::vector<std::pair<float, float>> vector_data;
	std::vector<std::pair<float, float>> vector_data_original;
	bool mouseIsPressed;
	bool play;
	bool isTouch;
	int xPieces;
	int yPieces;
	float puzzlePieceWidth;
	float puzzlePieceHeight;
	int row;
	int column;
	int moves;
	float playTime;
	std::default_random_engine rng;
	float puzzleWidth;
	float puzzleHeight;
	int touchX;
	int touchY;
	float puzzlePieceDiv;
	float puzzlePieceDivHalf;
	float borderSize;
};
