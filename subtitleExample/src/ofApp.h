// Refer to the README.md in the example's root folder for more information on usage

#pragma once

#include "ofMain.h"
#include "srtparser.h"


class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void draw();

		SubtitleParserFactory* subParserFactory;
		SubtitleParser* parser;
		std::vector<SubtitleItem*> sub;
};
	
