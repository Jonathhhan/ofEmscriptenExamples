#include "number.h"

void number::setup(float _xpos, float _ypos, float _width, float _height, float _minVal, float _maxVal) {
	ofAddListener(ofEvents().mousePressed, this, &number::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &number::mouseDragged);
	ofAddListener(ofEvents().touchDown, this, &number::touchDown);
	ofAddListener(ofEvents().touchMoved, this, &number::touchMoved);
	ofAddListener(ofEvents().keyPressed, this, &number::keyPressed);
	xpos = _xpos;
	ypos = _ypos;
	width = _width;
	height = _height;
	minVal = _minVal;
	maxVal = _maxVal;
	isTouch = false;
	mouseState = false;
	key = 0;
	value = 0;
	previewValue = 0;
	preview = false;
	count = 0;
	now = 0;
	previousMouseY = 0;
}

void number::draw() {
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(xpos, ypos);
	ofSetColor(0);
	ofDrawRectangle(0, 0, width, height);
	ofSetColor(255);
	ofDrawRectangle(2, 2, width - 4, height - 4);
	if (!mouseState) {
		ofSetColor(0);
	} else {
		ofSetColor(100, 200, 255);
	}
	if (preview) {
		ofDrawBitmapString(ofToString(previewValue) + ">", 8, 20);
	} else {
		ofDrawBitmapString(ofToString(value), 8, 20);
	}
	ofPopMatrix();
	ofPopStyle();
	if (now < ofGetElapsedTimeMillis()) {
		preview = false;
		previewValue = 0;
		count = 0;
	}
}

void number::mouseDragged(ofMouseEventArgs & args) {
	if (mouseState && !isTouch) {
		value = (previousMouseY - args.y) + value;
		previousMouseY = args.y;
		if (value > maxVal) {
			value = maxVal;
		}
		if (value < minVal) {
			value = minVal;
		}
	}
}

void number::mousePressed(ofMouseEventArgs & args) {
	if (args.x > xpos && args.x < xpos + width && args.y > ypos && args.y < ypos + height && !isTouch) {
		count = 0;
		previewValue = 0;
		preview = false;
		mouseState = true;
		previousMouseY = args.y;
	} else {
		mouseState = 0;
		previousMouseY = args.y;
	}
}

void number::touchDown(ofTouchEventArgs & args) {
	if (args.x > xpos && args.x < xpos + width && args.y > ypos && args.y < ypos + height) {
		isTouch = true;
		count = 0;
		previewValue = 0;
		preview = false;
		mouseState = true;
		previousMouseY = args.y;
	} else {
		mouseState = false;
		previousMouseY = args.y;
	}
}

void number::touchMoved(ofTouchEventArgs & args) {
	if (mouseState) {
		value = (previousMouseY - args.y) + value;
		previousMouseY = args.y;
		if (value > maxVal) {
			value = maxVal;
		}
		if (value < minVal) {
			value = minVal;
		}
	}
}

void number::keyPressed(ofKeyEventArgs & args) {
	key = args.key;
	if (key >= 48 && key <= 57 && mouseState) {
		now = ofGetElapsedTimeMillis() + 3000;
		preview = true;
		previewValue = previewValue * 10 + (key - 48);
	}
	if (key == 13 && preview) {
		value = previewValue;
		if (value > maxVal) {
			value = maxVal;
		}
		if (value < minVal) {
			value = minVal;
		}
		preview = false;
		mouseState = false;
	}
}