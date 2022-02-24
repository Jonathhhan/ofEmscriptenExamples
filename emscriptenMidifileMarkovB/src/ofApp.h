#pragma once

#include "ofMain.h"
#include "ofxPd.h"
#include "ofxPdExternals.h"

#include "bang.h"
#include "hSlider.h"
#include "number.h"
#include "hRadio.h"
#include "label.h"

#define NBANGS 7
#define NHSLIDERS 3
#define NLABELS 15

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
		void receiveList(const std::string &dest, const pd::List &list);

		void hSlider_1onMousePressed(float & e);
		void hSlider_2onMousePressed(float & e);
		void hSlider_3onMousePressed(float & e);
		void number_1onMousePressed(float & e);
		void hRadio_1onMousePressed(float & e);
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
		hRadio hRadio_1;
		
		std::string midifile;
};
