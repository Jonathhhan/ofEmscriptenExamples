// Refer to the README.md in the example's root folder for more information on usage

#pragma once

#include "ofMain.h"
#include "bang.h"
#include "toggle.h"
#include "number.h"
#include "hRadio.h"
#include "label.h"

#include "ofxVolumetrics.h"
#include "ofxTimeMeasurements.h"

constexpr auto NCELLS = 27;

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void mousePressed(ofMouseEventArgs & args);

		int pattern;
		float now;
		float width;
		float height;
		int oldGridNumX;
		int oldGridNumY;
		int oldGridNumZ;
		
		ofVideoPlayer videoPlayer;
		vector<ofFbo> fboArray;
		ofXml xml;
		ofShader shaderGameOfLife;
		ofShader shaderGameOfLifeSlice;
		ofShader shaderRaster;
		ofShader videoShader;
		ofFbo fboCells;
		ofFbo fboCellsCopy;
		ofxVolumetrics texture3d;
		ofxVolumetrics texture3dCopy;
		ofxVolumetrics texture3dVideo;
		ofFbo fboLines; 
		ofPixels pixels;
		ofEasyCam easyCam;
		std::deque<ofTexture> frames;
		ofTexture tex;

		class bang bang_copy;
		class bang bang_paste;
		class bang bang_save;
		class bang bang_reset;
		class bang bang_iterate;
		class bang bang_update;
		class toggle toggle_grid;
		class toggle toggle_sequence;
		toggle groupOfLivingCells[NCELLS];
		toggle groupOfDeadCells[NCELLS];
		label groupOfLabels[14];
		class number number_gridNumX;
		class number number_gridNumY;
		class number number_gridNumZ;
		class number number_interval;
		class hRadio hRadio_pattern;
};