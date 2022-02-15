#pragma once

#include "ofMain.h"
#include "ofxPd.h"
#include "ofxPdExternals.h"

#include "hSlider.h"
#include "bang.h"
#include "toggle.h"
#include "label.h"
#include "number.h"
#include "hRadio.h"

// a namespace for the Pd types
using namespace pd;

// inherit pd receivers to receive message and midi events
class ofApp : public ofBaseApp, public PdReceiver, public PdMidiReceiver {

	public:

		// main
		void setup();
		void update();
		void draw();
		void exit();
		
		// audio callbacks
		void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);
		
		// pd message receiver callbacks
		void print(const std::string &message);
		void receiveFloat(const std::string &dest, float value);
		
		// pd midi receiver callbacks
		void receiveNoteOn(const int channel, const int pitch, const int velocity);
		void receiveControlChange(const int channel, const int controller, const int value);
		void receiveProgramChange(const int channel, const int value);
		void receivePitchBend(const int channel, const int value);
		void receiveAftertouch(const int channel, const int value);
		void receivePolyAftertouch(const int channel, const int pitch, const int value);

		void hSlider_1onMousePressed(float & e);
		void hSlider_2onMousePressed(float & e);
		void hSlider_3onMousePressed(float & e);
		void number_1onMousePressed(float & e);
		void toggle_1onMousePressed(bool & e);
		void bang_1onMousePressed(bool & e);
		void bang_2onMousePressed(bool & e);
		void bang_3onMousePressed(bool & e);
		void bang_4onMousePressed(bool & e);	
		void bang_5onMousePressed(bool & e);	
		void loadMidiX();

		ofxPd pd;
		Patch patch;
		vector<float> scopeArray;
		vector<Patch> instances;
		toggle toggle_1;
		bang bang_1;
		bang bang_2;
		bang bang_3;
		bang bang_4;
		bang bang_5;
		hSlider hSlider_1;
		hSlider hSlider_2;
		hSlider hSlider_3;
		number number_1;
		label label_1;	
		label label_2;
		label label_3;
		label label_4;
		label label_5;
		label label_6;
		label label_7;
		label label_8;
		label label_9;
		label label_10;
		label label_11;
		label label_12;
		label label_13;
};
