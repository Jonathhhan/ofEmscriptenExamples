#pragma once

#include "ofMain.h"
#include "srtparser.h"
#include "ofxWord2Vec.h"

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void draw();

		SubtitleParserFactory* subParserFactory;
		SubtitleParser* parser;
		std::vector<SubtitleItem*> sub;
		int subNo;
		string s2;
		string s3;
		string s;
		 std::vector<std::string> word;
		ofxWord2VecEmbedding embed;
};
