// Refer to the README.md in the example's root folder for more information on usage

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "bang.h"
#include "toggle.h"
#include "number.h"
#include "hRadio.h"
#include "label.h"

constexpr auto NCELLS = 9;
constexpr auto NLABELS = 12;

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void mousePressed(ofMouseEventArgs& args);

	float now;
	float width;
	float height;
	int oldGridNumX;
	int oldGridNumY;
	long long counter;

	ofXml xml;
	ofShader shaderGameOfLife;
	ofShader shaderRaster;
	ofFbo fboCells;
	ofFbo fboCellsCopy;
	ofFbo fboLines;
	ofPixels pixels;

	bang bang_copy;
	bang bang_paste;
	bang bang_save;
	bang bang_reset;
	bang bang_iterate;
	toggle toggle_grid;
	toggle toggle_sequence;
	toggle groupOfLivingCells[NCELLS];
	toggle groupOfDeadCells[NCELLS];
	label groupOfLabels[NLABELS];
	number number_gridNumX;
	number number_gridNumY;
	number number_interval;
	hRadio hRadio_pattern;
};
