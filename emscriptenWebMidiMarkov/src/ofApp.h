/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxPd for documentation
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxPd.h"

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
		
		// pd midi receiver callbacks
		void receiveNoteOn(const int channel, const int pitch, const int velocity);
		void receiveProgramChange(const int channel, const int value);

		void number_1onMousePressed(float & e);
		void number_2onMousePressed(float & e);
		void number_3onMousePressed(float & e);
		void number_4onMousePressed(float & e);
		void number_5onMousePressed(float & e);
		void number_6onMousePressed(float & e);
		void number_7onMousePressed(float & e);
		void number_8onMousePressed(float & e);
		void toggle_1onMousePressed(bool & e);
		void bang_1onMousePressed(bool & e);
		void bang_2onMousePressed(bool & e);
		void bang_3onMousePressed(bool & e);
		void bang_4onMousePressed(bool & e);	
		void hRadio_1onMousePressed(float & e);		
		void midiChanged(std::string & value);

		ofxPd pd;
		Patch patch;
		bool toggle;
		vector<float> scopeArray;
		vector<Patch> instances;
		class toggle toggle_1;
		class bang bang_1;
		class bang bang_2;
		class bang bang_3;
		class bang bang_4;
		class number number_1;
		class number number_2;
		class number number_3;
		class number number_4;
		class number number_5;
		class number number_6;
		class number number_7;
		class number number_8;
		class label label_1;	
		class label label_2;
		class label label_3;
		class label label_4;
		class label label_5;
		class label label_6;
		class label label_7;
		class label label_8;
		class label label_9;
		class label label_10;
		class label label_11;
		class label label_12;
		class label label_13;
		class label label_14;
		class label label_15;
		class label label_16;
		class label label_17;
		class hRadio hRadio_1;
		int midiChan;
};
