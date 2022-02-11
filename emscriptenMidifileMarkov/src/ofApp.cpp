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
#include "ofApp.h"
#include "emscripten.h"
#include "emscripten/bind.h"

using namespace emscripten;

ofEvent<std::string> midiChangedEvent; 	
ofEvent<void> loadMidiEvent;

void midiIn(std::string ev) {
	midiChangedEvent.notify(ev); 
}

void ofApp::midiChanged(std::string & ev) {
	unsigned char* uEv = (unsigned char*)ev.c_str();
	int inChannel = uEv[0];
	int inPitch = uEv[1];
	int inVelocity = uEv[2];
	pd.startMessage();
	pd.addFloat(inChannel);
	pd.addFloat(inPitch);
	pd.addFloat(inVelocity);
	pd.finishList(patch.dollarZeroStr()+"-midiIn");
	label_10.symbol = "Midi in: " + ofToString(inChannel) + " " + ofToString(inPitch) + " " + ofToString(inVelocity);
}

//--------------------------------------------------------------
void ofApp::bang_5onMousePressed(bool & e) { 
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.mid';
	input.onchange = function(e) {

		// getting a hold of the file reference
		var file = e.target.files[0]; 

		// setting up the reader
		var reader = new FileReader();
		reader.readAsArrayBuffer(file);

		// here we tell the reader what to do when it's done reading...
		reader.onload = function() {
			var arrayBuffer = reader.result;
			var uint8View = new Uint8Array(arrayBuffer);	
			FS.createDataFile("/data/", "data", uint8View, true, true);
			FS.syncfs(true, function (err) {
				Module.loadMidi();
				assert(!err);
        		});	
		}
	};
	input.click();
	);
}

void loadMidi() {        
	loadMidiEvent.notify(); 
}

void ofApp::loadMidiX() {
	pd.sendBang(patch.dollarZeroStr() + "-loadMidiFile");
	EM_ASM(FS.unlink("/data/data"));
}

//--------------------------------------------------------------
EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("midiIn", &midiIn);
	emscripten::function("loadMidi", &loadMidi);
}

//--------------------------------------------------------------
void ofApp::toggle_1onMousePressed(bool & e){
	pd.sendFloat(patch.dollarZeroStr() + "-play", e);
}

//--------------------------------------------------------------
void ofApp::toggle_2onMousePressed(bool & e){
	pd.sendFloat(patch.dollarZeroStr() + "-drums", e);
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-bpm", e);
}

//--------------------------------------------------------------
void ofApp::number_2onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-markovOrder", e);
}

//--------------------------------------------------------------
void ofApp::number_3onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-noteLength", e);
}

//--------------------------------------------------------------
void ofApp::number_4onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-pgmChange", e);
}

//--------------------------------------------------------------
void ofApp::number_5onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-velocity", e);
}

//--------------------------------------------------------------
void ofApp::number_6onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-transpose", e);
}

//--------------------------------------------------------------
void ofApp::number_7onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-randomPitch", e);
}

//--------------------------------------------------------------
void ofApp::number_8onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-root", e);
}

//--------------------------------------------------------------
void ofApp::hRadio_1onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-scale", e);
	if(e == 0)
	label_16.symbol = "Scale: Major";
	if(e == 1)
	label_16.symbol = "Scale: Middle East Minor";
	if(e == 2)
	label_16.symbol = "Scale: Lydian dominant";
	if(e == 3)
	label_16.symbol = "Scale: Harmonic minor";
	if(e == 4)
	label_16.symbol = "Scale: Chromatic blues";
	if(e == 5)
	label_16.symbol = "Scale: Whole tones";
	if(e == 6)
	label_16.symbol = "Scale: Diminished";
	if(e == 7)
	label_16.symbol = "Scale: Pentatonic";
	if(e == 8)
	label_16.symbol = "Scale: Pentatonic blues";
	if(e == 9)
	label_16.symbol = "Scale: Gaku Joshi";
	if(e == 10)
	label_16.symbol = "Scale: In Sen";
	if(e == 11)
	label_16.symbol = "Scale: Hira Joshi";
	if(e == 12)
	label_16.symbol = "Scale: Yo";
	if(e == 13)
	label_16.symbol = "Scale: Ryo";
	if(e == 14)
	label_16.symbol = "Scale: Iwato";
	if(e == 15)
	label_16.symbol = "Scale: Tamuke";
}

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-markovMake");
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-markovReset");
}

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-markovRandom");
}

//--------------------------------------------------------------
void ofApp::bang_4onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-markovClear");
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(100, 100, 100);
	ofAddListener(toggle_1.onMousePressed, this, &ofApp::toggle_1onMousePressed);
	ofAddListener(toggle_2.onMousePressed, this, &ofApp::toggle_2onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(number_2.onMousePressed, this, &ofApp::number_2onMousePressed);
	ofAddListener(number_3.onMousePressed, this, &ofApp::number_3onMousePressed);
	ofAddListener(number_4.onMousePressed, this, &ofApp::number_4onMousePressed);
	ofAddListener(number_5.onMousePressed, this, &ofApp::number_5onMousePressed);
	ofAddListener(number_6.onMousePressed, this, &ofApp::number_6onMousePressed);
	ofAddListener(number_7.onMousePressed, this, &ofApp::number_7onMousePressed);
	ofAddListener(number_8.onMousePressed, this, &ofApp::number_8onMousePressed);
	ofAddListener(hRadio_1.onMousePressed, this, &ofApp::hRadio_1onMousePressed);
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(bang_3.onMousePressed, this, &ofApp::bang_3onMousePressed);
	ofAddListener(bang_4.onMousePressed, this, &ofApp::bang_4onMousePressed);
	ofAddListener(bang_5.onMousePressed, this, &ofApp::bang_5onMousePressed);
	ofAddListener(midiChangedEvent, this, &ofApp::midiChanged);
	ofAddListener(loadMidiEvent, this, &ofApp::loadMidiX);
	label_1.setup(20, 20, 680, 20, "Markov Generator");
	label_2.setup(120, 80, 200, 20, "Play");
	label_3.setup(120, 120, 200, 20, "Beats per Minute");
	label_4.setup(120, 200, 200, 20, "Program change");
	label_5.setup(120, 240, 200, 20, "Velocity");
	label_6.setup(120, 280, 200, 20, "Transpose");
	label_7.setup(120, 320, 200, 20, "Random");
	label_8.setup(20, 360, 300, 20, "Scale: Major");
	label_9.setup(120, 440, 200, 20, "Root");
	label_10.setup(500, 80, 200, 20, "Load midifile");
	label_11.setup(500, 120, 200, 20, "Load drum track only");
	label_12.setup(500, 160, 200, 20, "Markov order");
	label_13.setup(500, 200, 200, 20, "Create the chain");
	label_14.setup(500, 240, 200, 20, "Start position");
	label_15.setup(500, 280, 200, 20, "Random position");
	label_16.setup(500, 320, 200, 20, "Reset");
	label_17.setup(120, 160, 200, 20, "Note length");
	label_18.setup(400, 400, 300, 20, "Midi in: 0 0 0");
	label_19.setup(400, 440, 300, 20, "Midi out: 0 0 0");
	toggle_1.setup(20, 80, 20);
	toggle_2.setup(400, 120, 20);
	bang_1.setup(400, 200, 20);
	bang_2.setup(400, 240, 20);
	bang_3.setup(400, 280, 20);
	bang_4.setup(400, 320, 20);
	bang_5.setup(400, 80, 20);
	number_1.setup(20, 120, 80, 20, 10, 500);
	number_1.value = 120;
	number_2.setup(400, 160, 80, 20, 1, 100);
	number_2.value = 2;
	number_3.setup(20, 160, 80, 20, 50, 2000);
	number_3.value = 500;
	number_4.setup(20, 200, 80, 20, 1, 127);
	number_4.value = 8;
	number_5.setup(20, 240, 80, 20, 0, 127);
	number_5.value = 50;
	number_6.setup(20, 280, 80, 20, -24, 24);
	number_6.value = 0;
	number_7.setup(20, 320, 80, 20, 0, 10);
	number_7.value = 0;
	number_8.setup(20, 440, 80, 20, 0, 11);
	number_8.value = 0;
	hRadio_1.setup(20, 400, 20, 15);
	
	//ofSetLogLevel("Pd", OF_LOG_VERBOSE); // see verbose info inside

	// double check where we are ...
	cout << ofFilePath::getCurrentWorkingDirectory() << endl;

	// the number of libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)
	#ifdef TARGET_LINUX_ARM
		// longer latency for Raspberry PI
		int ticksPerBuffer = 32; // 32 * 64 = buffer len of 2048
		int numInputs = 0; // no built in mic
	#else
		int ticksPerBuffer = 8; // 8 * 64 = buffer len of 512
		int numInputs = 0;
	#endif

	// setup OF sound stream
	ofSoundStreamSettings settings;
	settings.numInputChannels = 0;
	settings.numOutputChannels = 2;
	settings.sampleRate = 44100;
	settings.bufferSize = ofxPd::blockSize() * ticksPerBuffer;
	settings.setInListener(this);
	settings.setOutListener(this);
	ofSoundStreamSetup(settings);

	// setup Pd
	//
	// set 4th arg to true for queued message passing using an internal ringbuffer,
	// this is useful if you need to control where and when the message callbacks
	// happen (ie. within a GUI thread)
	//
	// note: you won't see any message prints until update() is called since
	// the queued messages are processed there, this is normal
	//
	if(!pd.init(2, numInputs, 44100, ticksPerBuffer, false)) {
		OF_EXIT_APP(1);
	}
	
	seq_setup();
	
	midiChan = 1; // midi channels are 1-16

	// subscribe to receive source names
	pd.subscribe("toOF");
	pd.subscribe("env");

	// add message receiver, required if you want to recieve messages
	pd.addReceiver(*this); // automatically receives from all subscribed sources
	pd.ignoreSource(*this, "env");        // don't receive from "env"
	//pd.ignoreSource(*this);             // ignore all sources
	//pd.receiveSource(*this, "toOF");	  // receive only from "toOF"

	// add midi receiver, required if you want to recieve midi messages
	pd.addMidiReceiver(*this); // automatically receives from all channels
	//pd.ignoreMidiChannel(*this, 1);     // ignore midi channel 1
	//pd.ignoreMidiChannel(*this);        // ignore all channels
	//pd.receiveMidiChannel(*this, 1);    // receive only from channel 1

	// add the data/pd folder to the search path
	//pd.addToSearchPath("pd/abs");

	// audio processing on
	pd.start();

	// -----------------------------------------------------
	//cout << endl << "BEGIN Patch Test" << endl;
	// open patch
	patch = pd.openPatch("pd/test.pd");
	cout << patch << endl;
	//cout << "FINISH Patch Test" << endl;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(50, 50, 50);
        ofDrawRectangle(10, 10, 700, 460);
	label_1.draw();
	label_2.draw();
	label_3.draw();
	label_4.draw();
	label_5.draw();
	label_6.draw();
	label_7.draw();
	label_8.draw();
	label_9.draw();
	label_10.draw();
	label_11.draw();
	label_12.draw();
	label_13.draw();
	label_14.draw();
	label_15.draw();
	label_16.draw();
	label_17.draw();
	label_18.draw();
	label_19.draw();
	number_1.draw();
	number_2.draw();
	number_3.draw();
	number_4.draw();
	number_5.draw();
	number_6.draw();
	number_7.draw();
	number_8.draw();
	hRadio_1.draw();
	toggle_1.draw();
	toggle_2.draw();
	bang_1.draw();
	bang_2.draw();
	bang_3.draw();
	bang_4.draw();
	bang_5.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {

	// cleanup
	ofSoundStreamStop();
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::print(const std::string &message) {
	cout << message << endl;
}

//--------------------------------------------------------------
void ofApp::receiveNoteOn(const int channel, const int pitch, const int velocity) {
	// cout << "OF MIDI: note on: " << channel << " " << pitch << " " << velocity << endl;
	int array[3] = {channel, pitch, velocity};
	size_t lengthOfArray = sizeof array / sizeof array[0];
	EM_ASM_(
	var data = new Uint32Array(HEAPU32.buffer, $0, $1);
	sendMIDI(data), array, lengthOfArray);
	label_11.symbol = "Midi out: " + ofToString(channel) + " " + ofToString(pitch) + " " + ofToString(velocity);
}

void ofApp::receiveProgramChange(const int channel, const int value) {
	// cout << "OF MIDI: program change: " << channel << " " << value << endl;
	int array[2] = {192, value};
	size_t lengthOfArray = sizeof array / sizeof array[0];
	EM_ASM_(
	var data = new Uint32Array(HEAPU32.buffer, $0, $1);
	sendMIDI(data), array, lengthOfArray);
	label_12.symbol = "Program change: " + ofToString(192) + " " + ofToString(value);
}
