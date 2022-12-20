#include "ofApp.h"

unique_ptr<ofxDropdown> audioInputDropdown;
unique_ptr<ofxDropdown> audioOutputDropdown;

//--------------------------------------------------------------
void ofApp::setup(){
	emscripten::function("loadAudioInDevices", this->loadAudioInDevices);
	emscripten::function("loadAudioOutDevices", this->loadAudioOutDevices);

	gui.setup("audio options");
	gui.setPosition(50, 50);
	gui.setWidthElements(300);
	gui.disableHeader();

	audioInputOptions.setName("audio in");
	audioInputDropdown = make_unique<ofxDropdown>(audioInputOptions);
	audioInputDropdown->disableMultipleSelection();
	audioInputDropdown->enableCollapseOnSelection();
	audioInputDropdown->addListener(this, &ofApp::selectAudioIn);
	gui.add(audioInputDropdown.get());

	audioOutputOptions.setName("audio out");
	audioOutputDropdown =  make_unique<ofxDropdown>(audioOutputOptions);
	audioOutputDropdown->disableMultipleSelection();
	audioOutputDropdown->enableCollapseOnSelection();
	audioOutputDropdown->addListener(this, &ofApp::selectAudioOut);
	gui.add(audioOutputDropdown.get());
	
	int ticksPerBuffer = 4;
	int numInputs = 2;

	ofSoundStreamSettings settings;
	settings.numInputChannels = 2;
	settings.numOutputChannels = 2;
	settings.sampleRate = 44100;
	settings.bufferSize = ofxPd::blockSize() * ticksPerBuffer;
	settings.setInListener(this);
	settings.setOutListener(this);
	ofSoundStreamSetup(settings);

	if(!puredata.init(2, numInputs, 44100, ticksPerBuffer, false)){
		OF_EXIT_APP(1);
	}
	puredata.subscribe("midi_out");
	puredata.addReceiver(*this); 
	puredata.addMidiReceiver(*this);
	puredata.start();
	patch = puredata.openPatch("pd/test.pd");
	
	EM_ASM(getAudioDevices());
}

//--------------------------------------------------------------
void ofApp::draw(){
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::loadAudioInDevices(std::string string){  
	std::vector<std::string> deviceList = ofSplitString(string, ",", true);
	int numOptions = audioInputDropdown->getNumOptions();
	audioInputDropdown->clear();
	for(int j = 0; j < deviceList.size(); j += 2){
		audioInputDropdown->add(deviceList[j], deviceList[j + 1]);
	}
	if(numOptions == 0){
		audioInputDropdown->setSelectedValueByIndex(0, true);
	}
	audioInputDropdown->setDropdownElementsWidth(300);
	audioInputDropdown->forceRedraw();
}

//--------------------------------------------------------------
void ofApp::loadAudioOutDevices(std::string string){  
	std::vector<std::string> deviceList = ofSplitString(string, ",", true);  
	int numOptions = audioOutputDropdown->getNumOptions();    	
	audioOutputDropdown->clear();
	for(int j = 0; j < deviceList.size(); j += 2){
		audioOutputDropdown->add(deviceList[j], deviceList[j + 1]);
	}
	if(numOptions == 0){
		audioOutputDropdown->setSelectedValueByIndex(0, true);
	}
	audioOutputDropdown->setDropdownElementsWidth(300);
	audioOutputDropdown->forceRedraw();
}

//--------------------------------------------------------------
void ofApp::selectAudioIn(string &file){
	EM_ASM(selectAudioIn(UTF8ToString($0)), file.c_str());
}

//--------------------------------------------------------------	      
void ofApp::selectAudioOut(string &file){
	EM_ASM(selectAudioOut(UTF8ToString($0)), file.c_str());
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
	puredata.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
	puredata.audioOut(output, bufferSize, nChannels);
}
