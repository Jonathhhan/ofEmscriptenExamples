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

#define NBANGS 7
#define NHSLIDERS 3
#define NLABELS 14

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
		void bang_1onMousePressed(bool & e);
		void bang_2onMousePressed(bool & e);
		void bang_3onMousePressed(bool & e);
		void bang_4onMousePressed(bool & e);	
		void bang_5onMousePressed(bool & e);
		void bang_6onMousePressed(bool & e);	
		void bang_7onMousePressed(bool & e);	
		void loadMidiX(std::string & file);
		
		ofxPd puredata;
		Patch patch;
		bang groupOfBangs[NBANGS];
		hSlider groupOfHSliders[NHSLIDERS];
		label groupOfLabels[NLABELS];
		number number_1;
		
		std::string midifile;
};
