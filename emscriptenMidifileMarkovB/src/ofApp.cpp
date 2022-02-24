#include "ofApp.h"
#include "emscripten.h"
#include "emscripten/bind.h"

//--------------------------------------------------------------
ofEvent<std::string &> loadMidiEvent;

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
			FS.createDataFile("/data/pd/", file.name, uint8View, true, true);
			FS.syncfs(true, function (err) {
				Module.loadMidi(file.name);
				assert(!err);
        		});	
		}
	};
	input.click();
	);
}

void loadMidi(std::string file) {        
	loadMidiEvent.notify(file); 
}

void ofApp::loadMidiX(std::string & file) {
	midifile = file;
	puredata.sendSymbol(patch.dollarZeroStr() + "-loadMidiFile", file);
	EM_ASM(FS.unlink("/data/pd/" + UTF8ToString($0)), file.c_str());
}

//--------------------------------------------------------------
EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("loadMidi", &loadMidi);
}

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e){
	puredata.sendBang(patch.dollarZeroStr() + "-play");
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e){
	puredata.sendBang(patch.dollarZeroStr() + "-stop");
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
		a.download = UTF8ToString($0).substring(0, UTF8ToString($0).length - 4) + "-markovRemix-" + time + ".mid";
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
	, midifile.c_str());
}

//--------------------------------------------------------------
void ofApp::bang_5onMousePressed(bool & e){
	puredata.sendBang(patch.dollarZeroStr() + "-markovMake");
}

//--------------------------------------------------------------
void ofApp::bang_6onMousePressed(bool & e){
	puredata.sendBang(patch.dollarZeroStr() + "-markovReset");
}

//--------------------------------------------------------------
void ofApp::bang_7onMousePressed(bool & e){
	puredata.sendBang(patch.dollarZeroStr() + "-markovRandom");
}

//--------------------------------------------------------------
void ofApp::hSlider_1onMousePressed(float & e){
	puredata.sendFloat(patch.dollarZeroStr() + "-tempo", e);
}

//--------------------------------------------------------------
void ofApp::hSlider_2onMousePressed(float & e){
	puredata.sendFloat(patch.dollarZeroStr() + "-velocity", e);
}

//--------------------------------------------------------------
void ofApp::hSlider_3onMousePressed(float & e){
	puredata.sendFloat(patch.dollarZeroStr() + "-note_length", e);
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float & e){
	puredata.sendFloat(patch.dollarZeroStr() + "-markovOrder", e);
}

//--------------------------------------------------------------
void ofApp::hRadio_1onMousePressed(float & e){
	if (e == 0) {
	EM_ASM_(sendMIDI([201, 1]));
	groupOfLabels[6].symbol = "Standard";
	} else if (e == 1) {
	EM_ASM_(sendMIDI([201, 9]));
	groupOfLabels[6].symbol = "Room";
	} else if (e == 2) {
	EM_ASM_(sendMIDI([201, 17]));
	groupOfLabels[6].symbol = "Power";
	} else if (e == 3) {
	EM_ASM_(sendMIDI([201, 25]));
	groupOfLabels[6].symbol = "Electronic";
	} else if (e == 4) {
	EM_ASM_(sendMIDI([201, 26]));
	groupOfLabels[6].symbol = "TR-808";
	} else if (e == 5) {
	EM_ASM_(sendMIDI([201, 33]));
	groupOfLabels[6].symbol = "Jazz";
	} else if (e == 6) {
	EM_ASM_(sendMIDI([201, 41]));
	groupOfLabels[6].symbol = "Brush";
	} else if (e == 7) {
	EM_ASM_(sendMIDI([201, 49]));
	groupOfLabels[6].symbol = "Orchestra";
	} else if (e == 8) {
	EM_ASM_(sendMIDI([201, 57]));
	groupOfLabels[6].symbol = "Sound FX";
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(100, 100, 100);
	EM_ASM_(sendMIDI([201, 1]));
	
	ofAddListener(groupOfBangs[0].onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(groupOfBangs[1].onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(groupOfBangs[2].onMousePressed, this, &ofApp::bang_3onMousePressed);
	ofAddListener(groupOfBangs[3].onMousePressed, this, &ofApp::bang_4onMousePressed);
	ofAddListener(groupOfBangs[4].onMousePressed, this, &ofApp::bang_5onMousePressed);
	ofAddListener(groupOfBangs[5].onMousePressed, this, &ofApp::bang_6onMousePressed);
	ofAddListener(groupOfBangs[6].onMousePressed, this, &ofApp::bang_7onMousePressed);
	ofAddListener(groupOfHSliders[0].onMousePressed, this, &ofApp::hSlider_1onMousePressed);
	ofAddListener(groupOfHSliders[1].onMousePressed, this, &ofApp::hSlider_2onMousePressed);
	ofAddListener(groupOfHSliders[2].onMousePressed, this, &ofApp::hSlider_3onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(hRadio_1.onMousePressed, this, &ofApp::hRadio_1onMousePressed);
	ofAddListener(loadMidiEvent, this, &ofApp::loadMidiX);
	
	groupOfLabels[0].setup(20, 20, 680, 20, "Markov Generator");
	groupOfLabels[1].setup(120, 80, 200, 20, "Play");
	groupOfLabels[2].setup(120, 120, 200, 20, "Stop");
	groupOfLabels[3].setup(120, 200, 200, 20, "Tempo");
	groupOfLabels[4].setup(120, 240, 200, 20, "Velocity");
	groupOfLabels[5].setup(120, 280, 200, 20, "Note Length");
	groupOfLabels[6].setup(220, 320, 100, 20, "Standard");
	groupOfLabels[7].setup(500, 80, 200, 20, "Load midifile");
	groupOfLabels[8].setup(500, 120, 200, 20, "Download midifile");
	groupOfLabels[9].setup(500, 200, 200, 20, "Markov order");
	groupOfLabels[10].setup(500, 240, 200, 20, "Create the chains");
	groupOfLabels[11].setup(500, 280, 200, 20, "Start position");
	groupOfLabels[12].setup(500, 320, 200, 20, "Random position");
	groupOfLabels[13].setup(20, 400, 680, 20, "Index: 0");
	groupOfLabels[14].setup(20, 440, 680, 20, "Possibilities: 0");
	groupOfBangs[0].setup(20, 80, 20);
	groupOfBangs[1].setup(20, 120, 20);
	groupOfBangs[2].setup(400, 80, 20);
	groupOfBangs[3].setup(400, 120, 20);
	groupOfBangs[4].setup(400, 240, 20);
	groupOfBangs[5].setup(400, 280, 20);
	groupOfBangs[6].setup(400, 320, 20);
	groupOfHSliders[0].setup(20, 200, 80, 20, 0.5, 1.5);
	groupOfHSliders[0].value = 1;
	groupOfHSliders[0].slider = 0.5;
	groupOfHSliders[1].setup(20, 240, 80, 20, 0, 1);
	groupOfHSliders[1].value = 1;
	groupOfHSliders[1].slider = 1;
	groupOfHSliders[2].setup(20, 280, 80, 20, 0, 1000);
	groupOfHSliders[2].value = 200;
	groupOfHSliders[2].slider = 0.2;
	number_1.setup(400, 200, 80, 20, 1, 100);
	number_1.value = 4;
	hRadio_1.setup(20, 320, 20, 9);
	
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
	if(!puredata.init(2, numInputs, 44100, ticksPerBuffer, false)) {
		OF_EXIT_APP(1);
	}
	
	// setup the externals
	midi_setup();

	// subscribe to receive source names
	puredata.subscribe("index");
	puredata.subscribe("possibilities");
	puredata.subscribe("drumkit");
	puredata.subscribe("markov_list");
	puredata.subscribe("midi_out");

	// add message receiver, required if you want to recieve messages
	// automatically receives from all subscribed sources
	puredata.addReceiver(*this); 

	// add midi receiver, required if you want to recieve midi messages
	puredata.addMidiReceiver(*this); // automatically receives from all channels

	// add the data/pd folder to the search path
	//puredata.addToSearchPath("pd/abs");

	// audio processing on
	puredata.start();

	// -----------------------------------------------------
	
	// open patch
	patch = puredata.openPatch("pd/test.pd");
	cout << "Markov Generator" << endl;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(50, 50, 50);
        ofDrawRectangle(10, 10, 700, 460);
	for(int i=0; i<NBANGS; i++){
		groupOfBangs[i].draw();
	}
	for(int i=0; i<NHSLIDERS; i++){
		groupOfHSliders[i].draw();
	}
	for(int i=0; i<NLABELS; i++){
		groupOfLabels[i].draw();
	}
	number_1.draw();
	hRadio_1.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {

	// cleanup
	ofSoundStreamStop();
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
	puredata.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
	puredata.audioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::print(const std::string &message) {
	cout << message << endl;
}

//--------------------------------------------------------------
void ofApp::receiveFloat(const std::string &dest, float value) {
	if (dest == "index") {
	groupOfLabels[13].symbol = "Index: " + ofToString(value);
	} else if (dest == "possibilities") {
	groupOfLabels[14].symbol = "Possibilities: " + ofToString(value);
	}
}

//--------------------------------------------------------------
void ofApp::receiveList(const std::string &dest, const pd::List &list) {
	if (dest == "midi_out") {
		if (list.len() == 2) {
			int val_0 = list.getFloat(0);
			int val_1 = list.getFloat(1);
			int array[2] = {val_0, val_1};
			size_t lengthOfArray = sizeof array / sizeof array[0];
			EM_ASM_(
			var data = new Uint32Array(HEAPU32.buffer, $0, $1);
			sendMIDI(data), array, lengthOfArray);
		} else if (list.len() == 3) {
			int val_0 = list.getFloat(0);
			int val_1 = list.getFloat(1);
			int val_2 = list.getFloat(2);
			int array[3] = {val_0, val_1, val_2};
			size_t lengthOfArray = sizeof array / sizeof array[0];
			EM_ASM_(
			var data = new Uint32Array(HEAPU32.buffer, $0, $1);
			sendMIDI(data), array, lengthOfArray);
		}
	}
}
