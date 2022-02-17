#include "ofApp.h"
#include "emscripten.h"
#include "emscripten/bind.h"

//--------------------------------------------------------------
ofEvent<void> loadMidiEvent;

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool & e) { 
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
	emscripten::function("loadMidi", &loadMidi);
}

//--------------------------------------------------------------
void ofApp::hSlider_1onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-tempo", e);
}

//--------------------------------------------------------------
void ofApp::hSlider_2onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-velocity", e);
}

//--------------------------------------------------------------
void ofApp::hSlider_3onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-note_length", e);
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-markovOrder", e);
}

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-play");
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-stop");
}

//--------------------------------------------------------------
void ofApp::bang_4onMousePressed(bool & e){
	EM_ASM(
	var found = false;
	for (const file of FS.readdir("/data/pd/")){
		if ("record.mid" == file) {
		found = true;
        	}        
	}
	if(found){
        	var content = FS.readFile("/data/pd/record.mid");      
		var today = new Date();
		var time = today.getFullYear() + "_" + (today.getMonth() + 1 ) + "_" + today.getDate() + "_" + today.getHours() + "_" + today.getMinutes() + "_" + today.getSeconds();
		var a = document.createElement('a');
		a.download = "markovRemix-" + time + ".mid";
		var blob = new Blob(
		[content],
		{
		type: "text/plain;charset=utf-8"
		}
		);
		a.href = URL.createObjectURL(blob);
		a.style.display = "none";
		document.body.appendChild(a);
		a.click();
		document.body.removeChild(a);
		URL.revokeObjectURL(a.href);

	} else {
	alert("Please play and stop the markov chains before download!")
	}
	);
}

//--------------------------------------------------------------
void ofApp::bang_5onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-markovMake");
}

//--------------------------------------------------------------
void ofApp::bang_6onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-markovReset");
}

//--------------------------------------------------------------
void ofApp::bang_7onMousePressed(bool & e){
	pd.sendBang(patch.dollarZeroStr() + "-markovRandom");
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(100, 100, 100);
	ofAddListener(hSlider_1.onMousePressed, this, &ofApp::hSlider_1onMousePressed);
	ofAddListener(hSlider_2.onMousePressed, this, &ofApp::hSlider_2onMousePressed);
	ofAddListener(hSlider_3.onMousePressed, this, &ofApp::hSlider_3onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(bang_3.onMousePressed, this, &ofApp::bang_3onMousePressed);
	ofAddListener(bang_4.onMousePressed, this, &ofApp::bang_4onMousePressed);
	ofAddListener(bang_5.onMousePressed, this, &ofApp::bang_5onMousePressed);
	ofAddListener(bang_6.onMousePressed, this, &ofApp::bang_6onMousePressed);
	ofAddListener(bang_7.onMousePressed, this, &ofApp::bang_7onMousePressed);
	ofAddListener(loadMidiEvent, this, &ofApp::loadMidiX);
	label_1.setup(20, 20, 680, 20, "Markov Generator");
	label_2.setup(120, 80, 200, 20, "Play");
	label_3.setup(120, 120, 200, 20, "Stop");
	label_4.setup(120, 200, 200, 20, "Tempo");
	label_5.setup(120, 240, 200, 20, "Velocity");
	label_6.setup(120, 280, 200, 20, "Note Length");
	label_7.setup(500, 80, 200, 20, "Load midifile");
	label_8.setup(500, 120, 200, 20, "Download midifile");
	label_9.setup(500, 200, 200, 20, "Markov order");
	label_10.setup(500, 240, 200, 20, "Create the chains");
	label_11.setup(500, 280, 200, 20, "Start position");
	label_12.setup(500, 320, 200, 20, "Random position");
	label_13.setup(20, 400, 680, 20, "Possibilities: 0");
	label_14.setup(20, 440, 680, 20, "Midi out: 0");
	bang_1.setup(20, 80, 20);
	bang_2.setup(20, 120, 20);
	bang_3.setup(400, 80, 20);
	bang_4.setup(400, 120, 20);
	bang_5.setup(400, 240, 20);
	bang_6.setup(400, 280, 20);
	bang_7.setup(400, 320, 20);
	number_1.setup(400, 200, 80, 20, 1, 100);
	number_1.value = 4;
	hSlider_1.setup(20, 200, 80, 20, 0.5, 1.5);
	hSlider_1.value = 1;
	hSlider_1.slider = 0.5;
	hSlider_2.setup(20, 240, 80, 20, 0, 1);
	hSlider_2.value = 1;
	hSlider_2.slider = 1;
	hSlider_3.setup(20, 280, 80, 20, 0, 1000);
	hSlider_3.value = 200;
	hSlider_3.slider = 0.2;
	
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
		int ticksPerBuffer = 4; // 8 * 64 = buffer len of 512
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
	
	midi_setup();

	// subscribe to receive source names
	pd.subscribe("toOF");
	pd.subscribe("env");

	// add message receiver, required if you want to recieve messages
	// automatically receives from all subscribed sources
	pd.addReceiver(*this); 
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
	hSlider_1.draw();
	hSlider_2.draw();
	hSlider_3.draw();
	number_1.draw();
	bang_1.draw();
	bang_2.draw();
	bang_3.draw();
	bang_4.draw();
	bang_5.draw();
	bang_6.draw();
	bang_7.draw();
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
void ofApp::receiveFloat(const std::string &dest, float value) {
	if (dest == "toOF") {
	label_13.symbol = "Possibilities: " + ofToString(value);
	}
	if (dest == "midiOut") {
	EM_ASM_(
	sendMIDI($0), value);
	label_14.symbol = "Midi out: " + ofToString(value);
	}
}

//--------------------------------------------------------------
void ofApp::receiveNoteOn(const int channel, const int pitch, const int velocity) {
	int array[3] = {channel, pitch, velocity};
	size_t lengthOfArray = sizeof array / sizeof array[0];
	EM_ASM_(
	var data = new Uint32Array(HEAPU32.buffer, $0, $1);
	sendMIDI(data), array, lengthOfArray);
	label_14.symbol = "Midi out: " + ofToString(channel) + " " + ofToString(pitch) + " " + ofToString(velocity);
}

//--------------------------------------------------------------
void ofApp::receiveControlChange(const int channel, const int controller, const int value) {
	int array[3] = {channel, controller, value};
	size_t lengthOfArray = sizeof array / sizeof array[0];
	EM_ASM_(
	var data = new Uint32Array(HEAPU32.buffer, $0, $1);
	sendMIDI(data), array, lengthOfArray);
	label_14.symbol = "Midi out: " + ofToString(channel) + " " + ofToString(controller) + " " + ofToString(value);
}

//--------------------------------------------------------------
void ofApp::receiveProgramChange(const int channel, const int value) {
	int array[2] = {channel, value};
	size_t lengthOfArray = sizeof array / sizeof array[0];
	EM_ASM_(
	var data = new Uint32Array(HEAPU32.buffer, $0, $1);
	sendMIDI(data), array, lengthOfArray);
	label_14.symbol = "Midi out: " + ofToString(channel) + " " + ofToString(value);
}

//--------------------------------------------------------------
void ofApp::receivePitchBend(const int channel, const int value) {
	int array[2] = {channel, value};
	size_t lengthOfArray = sizeof array / sizeof array[0];
	EM_ASM_(
	var data = new Uint32Array(HEAPU32.buffer, $0, $1);
	sendMIDI(data), array, lengthOfArray);
	label_14.symbol = "Midi out: " + ofToString(channel) + " " + ofToString(value);
}

//--------------------------------------------------------------
void ofApp::receiveAftertouch(const int channel, const int value) {
	int array[2] = {channel, value};
	size_t lengthOfArray = sizeof array / sizeof array[0];
	EM_ASM_(
	var data = new Uint32Array(HEAPU32.buffer, $0, $1);
	sendMIDI(data), array, lengthOfArray);
	label_14.symbol = "Midi out: " + ofToString(channel) + " " + ofToString(value);
}

//--------------------------------------------------------------
void ofApp::receivePolyAftertouch(const int channel, const int pitch, const int value) {
	int array[3] = {channel, pitch, value};
	size_t lengthOfArray = sizeof array / sizeof array[0];
	EM_ASM_(
	var data = new Uint32Array(HEAPU32.buffer, $0, $1);
	sendMIDI(data), array, lengthOfArray);
	label_14.symbol = "Midi out: " + ofToString(channel) + " " + ofToString(pitch) + " " + ofToString(value);
}
