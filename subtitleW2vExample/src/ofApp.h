#pragma once

#include "ofMain.h"
#include "srtparser.h"
#include "ofxWord2Vec.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		ofVideoPlayer videoPlayer;
		SubtitleParserFactory* subParserFactory;
		SubtitleParser* parser;
		std::vector<SubtitleItem*> sub;
		ofxWord2VecEmbedding embed;
		
		int movieTime;
		int selectSubtitle;
		std::string fileName;
		std::set<int> subIndex;
		std::vector<std::string> individualWords;
		std::vector<std::string> stopWords;
};
