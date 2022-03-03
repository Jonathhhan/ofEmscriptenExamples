#pragma once

#include "ofMain.h"
#include "srtparser.h"
#include "ofxWord2Vec.h"

#include "bang.h"
#include "hSlider.h"
#include "number.h"
#include "toggle.h"
#include "label.h"

#define NBANGS 4
#define NTOGGLES 2

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		ofxEmscriptenVideoPlayer videoPlayer;
	
		SubtitleParserFactory* subParserFactory;
		SubtitleParser* parser;
		std::vector<SubtitleItem*> sub;

		ofxWord2VecVector Vec;
		ofxWord2VecEmbedding embed;
		vector<ofxWord2VecSorterItem> match;
		
		int selectSubtitle;
		int random;
		int vectorWords;
		float weight;
		bool randomStart;
		std::map<std::string, float> mapWordWeight;
		std::multimap<float, int> multimapSubWeight;
		std::vector<int> used_indices;
		std::vector<string> v1;
		std::vector<int> choosenSubs;
		std::string title;
		std::string fileName;
		std::string drawSubtitleDialogue;
		std::vector<int> subIndex;
		std::string currentDialogue;
		std::string lowerCurrentDialogue;
		std::vector<std::string> stopWords;
		
		void bang_1onMousePressed(bool & e);
		void bang_2onMousePressed(bool & e);
		void bang_3onMousePressed(bool & e);
		void bang_4onMousePressed(bool & e);	
		void toggle_1onMousePressed(bool & e);
		void toggle_2onMousePressed(bool & e);	
		void hSlider_1onMousePressed(float & e);
		void number_1onMousePressed(float & e);	
		
		void loadBinX(std::string & file);
		void loadSubtitleX(std::string & file);
		void loadVideoX(std::string & file);
		
		bang groupOfBangs[NBANGS];
		toggle groupOfToggles[NTOGGLES];
		number number_1;
		hSlider hSlider_1;
};
