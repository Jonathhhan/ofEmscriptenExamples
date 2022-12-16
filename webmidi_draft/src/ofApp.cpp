#include "ofApp.h"

unique_ptr<ofxDropdown> audioInputDropdown;
unique_ptr<ofxDropdown> audioOutputDropdown;
	
void loadMidiInDevices(std::string string){  
	std::vector<std::string> deviceList = ofSplitString(string, ",", true);      	
	audioInputDropdown->clear();
	for(int j = 0; j < deviceList.size();j += 2){
		audioInputDropdown->add(deviceList[j], deviceList[j + 1]);
	}
	audioInputDropdown->forceRedraw();
}

void loadMidiOutDevices(std::string string){  
	std::vector<std::string> deviceList = ofSplitString(string, ",", true);      	
	audioOutputDropdown->clear();
	for(int j = 0; j < deviceList.size();j += 2){
		audioOutputDropdown->add(deviceList[j], deviceList[j + 1]);
	}
	audioOutputDropdown->forceRedraw();
}

void midiIn(std::string midiIn){
	unsigned char* uMidiIn = (unsigned char*)midiIn.c_str();
	int channel= uMidiIn[0];
	int pitch= uMidiIn[1];
	int velocity= uMidiIn[2];
	std::cout << "midiin - channel: " << channel << " pitch: " << pitch << " velocity: " << velocity << std::endl;
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofAddListener(toggle_1.onMousePressed, this, &ofApp::toggle_1_event);
	emscripten::function("loadMidiInDevices", &loadMidiInDevices);
	emscripten::function("loadMidiOutDevices", &loadMidiOutDevices);
	emscripten::function("midiIn", &midiIn, emscripten::allow_raw_pointers());

	gui.setup("midi options");
	gui.setPosition(50, 50);
	gui.setWidthElements(250);
	gui.disableHeader();
	toggle_1.setup(400, 200, 50);

	audioInputOptions.setName("midi in");
	audioInputDropdown = make_unique<ofxDropdown>(audioInputOptions);
	audioInputDropdown->disableMultipleSelection();
	audioInputDropdown->enableCollapseOnSelection();
	audioInputDropdown->addListener(this, &ofApp::selectMidiIn);
	gui.add(audioInputDropdown.get());

	audioOutputOptions.setName("midi out");
	audioOutputDropdown =  make_unique<ofxDropdown>(audioOutputOptions);
	audioOutputDropdown->disableMultipleSelection();
	audioOutputDropdown->enableCollapseOnSelection();
	audioOutputDropdown->addListener(this, &ofApp::selectMidiOut);
	gui.add(audioOutputDropdown.get());
	EM_ASM(getMidiDevices());

	int ticksPerBuffer = 32;
	int numInputs = 0;

	ofSoundStreamSettings settings;
	settings.numInputChannels = 0;
	settings.numOutputChannels = 2;
	settings.sampleRate = 44100;
	settings.bufferSize = ofxPd::blockSize() * ticksPerBuffer;
	settings.setInListener(this);
	settings.setOutListener(this);
	ofSoundStreamSetup(settings);

	if(!puredata.init(2, numInputs, 44100, ticksPerBuffer, false)) {
		OF_EXIT_APP(1);
	}

	puredata.subscribe("midi_out");
	puredata.addReceiver(*this); 
	puredata.addMidiReceiver(*this);
	puredata.start();

	patch = puredata.openPatch("pd/test.pd");
	cout << "Markov Generator" << endl;
}

//--------------------------------------------------------------
void ofApp::draw() {
	gui.draw();
	toggle_1.draw();
}

//--------------------------------------------------------------
void ofApp::selectMidiIn(string &file) {
	EM_ASM(selectMidiIn(UTF8ToString($0)), file.c_str());
}

//--------------------------------------------------------------	      
void ofApp::selectMidiOut(string &file) {
	EM_ASM(selectMidiOut(UTF8ToString($0)), file.c_str());
}

//--------------------------------------------------------------
void ofApp::toggle_1_event(bool & e){
	puredata.sendFloat(patch.dollarZeroStr() + "-toggle", e);
}

void ofApp::receiveList(const std::string &dest, const pd::List &list) {
	if (dest == "midi_out") {
		if (list.len() == 2) {
			int channel = list.getFloat(0);
			int pitch = list.getFloat(1);
			int array[2] = {channel, pitch};
			size_t lengthOfArray = sizeof array / sizeof array[0];
			EM_ASM_(
			var data = new Uint32Array(HEAPU32.buffer, $0, $1);
			sendMidi(data), array, lengthOfArray);
			std::cout << "midiout - channel: " << channel << " pitch: " << pitch << std::endl;
		} else if (list.len() == 3) {
			int channel = list.getFloat(0);
			int pitch = list.getFloat(1);
			int velocity = list.getFloat(2);
			int array[3] = {channel, pitch, velocity};
			size_t lengthOfArray = sizeof array / sizeof array[0];
			EM_ASM_(
			var data = new Uint32Array(HEAPU32.buffer, $0, $1);
			sendMidi(data), array, lengthOfArray);
			std::cout << "midiout - channel: " << channel << " pitch: " << pitch << " velocity: " << velocity << std::endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::print(const std::string &message) {
	cout << message << endl;
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
	puredata.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
	puredata.audioOut(output, bufferSize, nChannels);
}
