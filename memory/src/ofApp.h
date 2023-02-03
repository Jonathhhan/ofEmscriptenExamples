// Refer to the README.md in the example's root folder for more information on usage

#pragma once

#include "ofMain.h"
#include "bang.h"
#include "number.h"
#include "label.h"
#include "random"

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
	ofFloatImage image_duplicates;
	ofTexture texture_puzzle;
	ofFbo fbo_puzzleOriginal;
	ofFbo fbo_puzzleState;
	ofFbo fbo_data;
	ofFbo fbo_duplicates;
	ofFbo fbo_puzzlePiece;
	ofFbo fbo_emptyPuzzlePiece;
	std::vector<int> vector_data;
	std::vector<int> vector_small_data;
	bool mouseIsPressed;
	bool isPlaying;
	bool isTouch;
	bool isPuzzleShader;
	bool isOpen;
	float xPieces;
	float yPieces;
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
	float borderSize;
	int oldPosition;
	int newPosition;
	int oldRow;
	int oldColumn;
};
