#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDropdown.h"

class ofApp : public ofBaseApp {
    public:
        void setup();
        void draw();

	void onSongChange(string & file);

	ofxGuiGroup gui;

	ofParameter<string> audioInputOptions;
	ofParameter<string> audioOutputOptions;

	unique_ptr<ofxDropdown> audioInputDropdown;
	unique_ptr<ofxDropdown> audioOutputDropdown;
};
