#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPd.h"
#include "ofxDropdown.h"
#include "emscripten.h"
#include "emscripten/bind.h"

using namespace pd;

class ofApp : public ofBaseApp, public PdReceiver, public PdMidiReceiver {
    public:
        void setup();
        void draw();

	static void loadAudioInDevices(std::string string);
	static void loadAudioOutDevices(std::string string);
	
	void selectAudioIn(string & file);
	void selectAudioOut(string & file);

	void audioReceived(float * input, int bufferSize, int nChannels);
	void audioRequested(float * output, int bufferSize, int nChannels);

	ofParameter<std::string> audioInputOptions;
	ofParameter<std::string> audioOutputOptions;
	
	ofxGuiGroup gui;
	ofxPd puredata;
	Patch patch;
};
