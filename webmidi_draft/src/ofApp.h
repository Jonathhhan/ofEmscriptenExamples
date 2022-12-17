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

	static void loadMidiInDevices(std::string string);
	static void loadMidiOutDevices(std::string string);
	static void midiIn(std::string string);

	void selectMidiIn(string & file);
	void selectMidiOut(string & file);

	void toggle_1_event(bool & e);

	void receiveList(const std::string &dest, const pd::List &list);
	void print(const std::string &message);	
	void audioReceived(float * input, int bufferSize, int nChannels);
	void audioRequested(float * output, int bufferSize, int nChannels);

	ofParameter<string> midiInputOptions;
	ofParameter<string> midiOutputOptions;

	ofxGuiGroup gui;
	toggle toggle_1;
	ofxPd puredata;
	Patch patch;
};
