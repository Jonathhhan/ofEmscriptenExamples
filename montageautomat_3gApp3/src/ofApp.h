#pragma once

#include "ofMain.h"
#include "srtparser.h"
#include "ofxWord2Vec.h"

#include "bang.h"
#include "hSlider.h"
#include "toggle.h"
#include "label.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		ofVideoPlayer videoPlayer;
		std::vector<ofVideoPlayer*> videoPlayerVector;
	
		std::vector<SubtitleItem*> sub;
		std::vector<std::vector<SubtitleItem*>> subVector;

		ofxWord2VecEmbedding embed;
		std::map<std::pair<int, int>, std::pair<ofxWord2VecVector, int>> mapSubVector;
		std::map<std::pair<int, int>, std::pair<ofxWord2VecVector, int>> mapSubVectorCopy;
		
		int selectedSubtitle;
		int numberOfSubtitles;
		int numberOfVideoPlayer;
		bool bRandomStart;
		bool bCustomWords;
		bool bReadMe;
		double weight;
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
		
		bang groupOfBangs[5];
		toggle groupOfToggles[4];
		hSlider hSlider_1;
};
