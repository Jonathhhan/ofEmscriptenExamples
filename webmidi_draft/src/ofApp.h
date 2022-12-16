#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPd.h"
#include "ofxDropdown.h"
#include "toggle.h"
#include "emscripten.h"
#include "emscripten/bind.h"

using namespace pd;

class ofApp : public ofBaseApp, public PdReceiver, public PdMidiReceiver {
    public:
        void setup();
        void draw();

	void selectMidiIn(string & file);
	void selectMidiOut(string & file);

	void toggle_1_event(bool & e);

	void print(const std::string &message);
	void receiveList(const std::string &dest, const pd::List &list);	
	void audioReceived(float * input, int bufferSize, int nChannels);
	void audioRequested(float * output, int bufferSize, int nChannels);
	
	ofxGuiGroup gui;

	ofParameter<string> midiInputOptions;
	ofParameter<string> midiOutputOptions;

	toggle toggle_1;
	ofxPd puredata;
	Patch patch;
};
