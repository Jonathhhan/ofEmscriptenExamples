#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	subParserFactory = new SubtitleParserFactory("Alphaville.ENG.srt");
	parser = subParserFactory->getParser();

	//to get subtitles

	sub = parser->getSubtitles();
}

//--------------------------------------------------------------
void ofApp::draw(){
	for (SubtitleItem* element : sub)
	{
		if (element->getStartTime() <= (ofGetElapsedTimeMillis() + 35000) && element->getEndTime() >= (ofGetElapsedTimeMillis() + 35000)) {
			ofDrawBitmapString(element->getDialogue(), 50, 50);
		}
	}
}
