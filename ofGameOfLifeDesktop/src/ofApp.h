// Refer to the README.md in the example's root folder for more information on usage

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "hSlider.h"
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
		
		void toggle_1onMousePressed(bool & e);
		void toggle_2onMousePressed(bool & e);
		void bang_1onMousePressed(bool & e);	
		void bang_2onMousePressed(bool & e);
		void bang_3onMousePressed(bool & e);
		void bang_4onMousePressed(bool & e);
		void bang_5onMousePressed(bool & e);
		void number_1onMousePressed(float & e);
		void number_2onMousePressed(float & e);
		void number_3onMousePressed(float & e);	
		void hRadio_1onMousePressed(float & e);
		
		float interval;
		bool raster;
		bool sequence;
		int counter;
		int x;
		int y;
		int pattern;
		float now;
		float width;
		float height;
		int gridNumX;
		int gridNumY;
		int oldGridNumX;
		int oldGridNumY;
		
		ofXml xml;
		ofShader shaderGameOfLife;
		ofShader shaderRaster;
		ofFbo fboCells;
		ofFbo fboCellsCopy;
		ofFbo fboLines; 
		ofPixels pixels;
		
		class bang bang_1;
		class bang bang_2;
		class bang bang_3;
		class bang bang_4;
		class bang bang_5;
		class toggle toggle_1;
		class toggle toggle_2;
		toggle groupOfLivingCells[NCELLS];
		toggle groupOfDeadCells[NCELLS];
		class number number_1;
		class number number_2;
		class number number_3;
		class hRadio hRadio_1;
		class label label_1;
		class label label_2;
		class label label_3;
		class label label_4;
		class label label_5;
		class label label_6;
		class label label_7;
		class label label_8;
		class label label_9;
		class label label_10;
		class label label_11;
		class label label_12;
};