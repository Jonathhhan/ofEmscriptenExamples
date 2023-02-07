#include "toggle.h"

void toggle::setup(float _xpos, float _ypos, float _size) {
	ofAddListener(ofEvents().mousePressed, this, &toggle::mousePressed);
	ofAddListener(ofEvents().touchDown, this, &toggle::touchDown);
	xpos = _xpos;
	ypos = _ypos;
	size = _size;
	isTouch = false;
	value = false;
}

void toggle::draw(){
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(xpos, ypos);
	ofSetColor(200);
	ofDrawRectangle(0, 0, size, size);
	ofSetColor(100);
	ofDrawRectangle(size / 40, size / 40, size - size / 20, size - size / 20);
	ofSetColor(255, 200, 200);
	if (value) {
		ofSetLineWidth(size / 40);
		ofDrawLine(size / 40, size / 40, size - size / 40, size - size / 40);
		ofDrawLine(size - size / 40, size / 40, size / 40, size - size / 40);
	}
	ofPopMatrix();
	ofPopStyle();
}

void toggle::mousePressed(ofMouseEventArgs & args){
	if (args.x > xpos && args.x < xpos + size && args.y > ypos && args.y < ypos + size && !isTouch){
		value = !value;
	}
}

void toggle::touchDown(ofTouchEventArgs & args){
	if (args.x > xpos && args.x < xpos + size && args.y > ypos && args.y < ypos + size){
		isTouch = true;
		value = !value;
	}
}