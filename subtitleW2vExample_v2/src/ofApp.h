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
		vector<ofxWord2VecSorterItem> match;
		
		int movieTime;
		int selectSubtitle;
		std::string title;
		std::string fileName;
		std::set<int> subIndex;
		std::string individualWords;
		std::vector<std::string> stopWords;
};
