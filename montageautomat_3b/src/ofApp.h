#pragma once

#include "ofMain.h"
#include "srtparser.h"
#include "ofxWord2Vec.h"

#include "bang.h"
#include "hSlider.h"
#include "toggle.h"
#include "label.h"

#define NBANGS 5
#define NTOGGLES 4

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		ofxEmscriptenVideoPlayer videoPlayer;
	
		SubtitleParserFactory* subParserFactory;
		SubtitleParser* parser;
		std::vector<SubtitleItem*> sub;

		ofxWord2VecEmbedding embed;
		
		int selectSubtitle;
		bool randomStart;
		bool bCustomWords;
		bool bReadMe;
		std::map<int, ofxWord2VecVector> mapSubVector;
		std::map<int, ofxWord2VecVector> mapSubVectorCopy;
		std::multimap<double, int> multimapWeightSub;
		std::vector<int> used_indices;
		std::vector<int> choosenSubs;
		std::string title;
		std::string fileName;
		std::string tempString;
		std::string drawSubtitleDialogue;
		std::vector<std::string> currentWords;
		std::vector<std::string> joinedWords;
		std::string currentWord;
		std::string customWords;
		std::vector<std::string> splitWords;
		std::vector<std::string> tempCurrentWords;
		std::string currentDialogue;
		std::vector<std::string> stopWords;
		
		void bang_1onMousePressed(bool & e);
		void bang_2onMousePressed(bool & e);
		void bang_3onMousePressed(bool & e);
		void bang_4onMousePressed(bool & e);	
		void bang_5onMousePressed(bool & e);	
		void toggle_1onMousePressed(bool & e);
		void toggle_2onMousePressed(bool & e);
		void toggle_3onMousePressed(bool & e);
		void toggle_4onMousePressed(bool & e);	
		void hSlider_1onMousePressed(float & e);	
		
		void loadBinX(std::string & file);
		void loadSubtitleX(std::string & file);
		void loadVideoX(std::string & file);
		void loadCustomWordsX(std::string & file);
		
		bang groupOfBangs[NBANGS];
		toggle groupOfToggles[NTOGGLES];
		hSlider hSlider_1;
};
