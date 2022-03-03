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

		ofxWord2VecVector Vec;
		ofxWord2VecEmbedding embed;
		vector<ofxWord2VecSorterItem> match;
		
		int movieTime;
		int selectSubtitle;
		int random;
		float weight;
		std::map<std::string, float> mapWordWeight;
		std::multimap<float, int> multimapSubWeight;
		std::vector<int> used_indices;
		std::vector<string> v1;
		std::vector<int> choosenSubs;
		std::string title;
		std::string fileName;
		std::vector<int> subIndex;
		std::string currentDialogue;
		std::string lowerCurrentDialogue;
		std::vector<std::string> stopWords;
};
