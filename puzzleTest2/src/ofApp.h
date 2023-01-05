/* Refer to the README.md in the example's root folder for more information on usage */

#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		
		// we will have a dynamic number of images, based on the content of a directory:
		ofDirectory dir;
		vector<ofImage> images;
		ofImage image;
		ofFbo fbo;
		ofFbo fboImg;
		std::vector<int> data;
		bool mouseIsPressed;
		int mouseButton;
		int puzzleWidth;
		int puzzleHeight;
		int xPieces;
		int yPieces;
		int puzzlePieceWidth;
		int puzzlePieceHeight;
		int xVal;
		int xPos;
		int xNumber;
		int yVal;
		int yPos;
		int yNumber;
		int row;
		int column;
		int currentImage;
};

