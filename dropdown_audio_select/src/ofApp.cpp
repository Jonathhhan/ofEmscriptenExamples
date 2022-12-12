#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	std::vector<std::string> list;
	list.push_back("audio 1");
	list.push_back("audio 2");
	list.push_back("audio 3");
	
	gui.setup("audio options");
	gui.setPosition(50, 50);
	
	audioInputOptions.setName("audioinput options");
	audioInputDropdown = make_unique<ofxDropdown>(audioInputOptions);
	audioInputDropdown->disableMultipleSelection();
	audioInputDropdown->enableCollapseOnSelection();
	for(int j = 0; j < list.size(); j++){
		audioInputDropdown->add(list[j]);
	}
	audioInputDropdown->addListener(this, &ofApp::onSongChange);
	gui.add(audioInputDropdown.get());
	
	audioOutputOptions.setName("audiooutput options");
	audioOutputDropdown =  make_unique<ofxDropdown>(audioOutputOptions);
	audioOutputDropdown->disableMultipleSelection();
	audioOutputDropdown->enableCollapseOnSelection();
	for(int j = 0; j < list.size(); j++){
		audioOutputDropdown->add(list[j]);
	}
	audioOutputDropdown->addListener(this, &ofApp::onSongChange);
	gui.add(audioOutputDropdown.get());
}

//--------------------------------------------------------------
void ofApp::draw() {
	gui.draw();
}

//--------------------------------------------------------------

void ofApp::onSongChange(string & file) {
	std::cout << "audio option changed" << std::endl;
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
	std::vector<std::string> list;
	list.push_back("video 1");
	list.push_back("audio 2");
	list.push_back("ton 3");
	list.push_back("test");
	
	audioInputDropdown->clear();
	for(int j = 0; j < list.size(); j++){
		audioInputDropdown->add(list[j]);
	}
	audioInputDropdown->forceRedraw();
		audioOutputDropdown->clear();
	audioOutputDropdown->clear();
	for(int j = 0; j < list.size(); j++){
		audioOutputDropdown->add(list[j]);
	}
	audioOutputDropdown->forceRedraw();
}
