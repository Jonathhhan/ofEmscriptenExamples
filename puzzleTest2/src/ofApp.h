// Refer to the README.md in the example's root folder for more information on usage

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "bang.h"
#include "number.h"
#include "label.h"
#include "random"
#include "emscripten.h"
#include "emscripten/bind.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(ofKeyEventArgs & args);
		void keyReleased(ofKeyEventArgs & args);
		void mouseMoved(ofMouseEventArgs & args);
		void mouseDragged(ofMouseEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		void mouseReleased(ofMouseEventArgs & args);
		void mouseEntered(ofMouseEventArgs & args);
		void mouseExited(ofMouseEventArgs & args);
		void touchMoved(ofTouchEventArgs & args);
		void touchDown(ofTouchEventArgs & args);
		void touchUp(ofTouchEventArgs & args);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void bang_1onMousePressed(bool & e);	
		void bang_2onMousePressed(bool & e);
		void number_1onMousePressed(float & e);
		void number_2onMousePressed(float & e);
		void loadImageX();
		
		class bang bang_1;
		class bang bang_2;
		class number number_1;
		class number number_2;
		class label label_1;
		class label label_2;
		class label label_3;
		class label label_4;
		class label label_5;
		class label label_6;
		
		ofImage image;
		ofFbo fbo;
		ofFbo fboImg;
		std::vector<int> data;
		bool mouseIsPressed;
		bool play;
		int puzzleWidth;
		int puzzleHeight;
		int xPieces;
		int yPieces;
		int puzzlePieceWidth;
		int puzzlePieceHeight;
		int row;
		int column;
		float playTime;
		int moves;
		std::default_random_engine rng;
};
