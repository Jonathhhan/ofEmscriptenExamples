#pragma once

#include "ofMain.h"
#include "hSlider.h"
#include "label.h"
#include "bang.h"
#include "toggle.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void hSlider_1onMousePressed(float & e);
		void hSlider_2onMousePressed(float & e);
		void hSlider_3onMousePressed(float & e);
		void bang_1onMousePressed(bool & e);
		void bang_2onMousePressed(bool & e);
		void bang_3onMousePressed(bool & e);
		void toggle_1onMousePressed(bool & e);
		void loadAudioUrlX(std::string & rv);
		void loadVideoUrlX(std::string & rv);
		void loadImageX();

		ofxEmscriptenVideoPlayer videoPlayer;
		ofxEmscriptenSoundPlayer audioPlayer;
		ofImage image;
		hSlider hSlider_1;
		hSlider hSlider_2;
		hSlider hSlider_3;
		label label_1;	
		label label_2;
		label label_3;
		label label_4;
		label label_5;
		label label_6;
		label label_7;
		label label_8;
		label label_9;
		label label_10;
		label label_11;
		bang bang_1;
		bang bang_2;
		bang bang_3;
		toggle toggle_1;
};
