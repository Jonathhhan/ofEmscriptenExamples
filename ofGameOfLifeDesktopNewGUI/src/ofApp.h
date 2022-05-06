// Refer to the README.md in the example's root folder for more information on usage

#pragma once

#include "ofMain.h"
#include "bang.h"
#include "toggle.h"
#include "number.h"
#include "hRadio.h"
#include "label.h"

constexpr auto NCELLS = 9;

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
		
		ofXml xml;
		ofShader shaderGameOfLife;
		ofShader shaderRaster;
		ofFbo fboCells;
		ofFbo fboCellsCopy;
		ofFbo fboLines; 
		ofPixels pixels;
		
		class bang bang_copy;
		class bang bang_paste;
		class bang bang_save;
		class bang bang_reset;
		class bang bang_iterate;
		class toggle toggle_grid;
		class toggle toggle_sequence;
		toggle groupOfLivingCells[NCELLS];
		toggle groupOfDeadCells[NCELLS];
		label groupOfLabels[12];
		class number number_gridNumX;
		class number number_gridNumY;
		class number number_interval;
		class hRadio hRadio_pattern;
};