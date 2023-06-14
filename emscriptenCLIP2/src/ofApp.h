#pragma once

#include "ofMain.h"
#include "hSlider.h"
#include "ofxGui.h"

#include "emscripten.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void slider_onMousePressed(float & e);
		void onTextChange(std::string & text);
		
		ofTexture tex1;
		ofTexture tex2;
		ofTexture tex3;
		ofTexture tex4;
		hSlider slider;
		ofxPanel _gui;
		ofParameterGroup _parameters;
		ofParameter<std::string> _textParameter;
		ofEventListener _textParameterListener;
		std::string textInput;
};
