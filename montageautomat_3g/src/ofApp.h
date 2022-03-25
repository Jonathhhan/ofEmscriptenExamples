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
	
		std::vector<SubtitleItem*> sub;

		ofxWord2VecEmbedding embed;
		std::map<int, std::tuple<ofxWord2VecVector, int>> mapSubVector;
		std::map<int, std::tuple<ofxWord2VecVector, int>> mapSubVectorCopy;
		
		int selectedSubtitle;
		int numberOfSubtitles;
		bool bRandomStart;
		bool bCustomWords;
		bool bReadMe;
		std::string title;
		std::string customWords;
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
