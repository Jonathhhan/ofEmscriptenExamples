#pragma once

#include "ofMain.h"
#include "srtparser.h"
#include "ofxWord2Vec.h"
#include "vec2WordThread.h"
#include "videoLoaderThread.h"
#include "subtitleLoaderThread.h"
#include "embedLoaderThread.h"

#include "bang.h"
#include "hSlider.h"
#include "toggle.h"
#include "label.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
	
		std::vector<std::vector<SubtitleItem*>> subVector;

		int selectedSubtitle;
		int numberOfSubtitles;
		int numberOfVideoPlayer;
		bool bReadMe;
		std::string title;
		std::vector<std::string> stopWords;

		ofVideoPlayer videoPlayer;
		std::vector<ofVideoPlayer*> videoPlayerVector;
		
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

		vec2WordThread vec2WordThread;
		videoLoaderThread videoLoaderThread;
		subtitleLoaderThread subtitleLoaderThread;
		embedLoaderThread embedLoaderThread;
};
