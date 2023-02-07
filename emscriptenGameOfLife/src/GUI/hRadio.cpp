#include "hRadio.h"

void hRadio::setup(float _xpos, float _ypos, float _size, float _number) {
	ofAddListener(ofEvents().mousePressed, this, &hRadio::mousePressed);
	ofAddListener(ofEvents().touchDown, this, &hRadio::touchDown);
	xpos = _xpos;
	ypos = _ypos;
	number = _number;
	size = _size;
	isTouch = false;
	value = false;
}

void hRadio::draw(){
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(xpos, ypos);
	ofSetLineWidth(size / 10);
	ofSetColor(0);
	ofDrawRectangle(0, 0, size * number, size);
	ofSetColor(255);
	ofDrawRectangle(size / 20, size / 10, size * number - size / 10, size - size / 5);
	ofSetColor(0);
	for (int x = 1; x < number; x++){
		ofDrawLine(size * x, 0, size * x, size);
	}
	ofDrawRectangle(size * value + size / 5, size / 5, size - size / 2.5, size - size / 2.5);
	ofPopMatrix();
	ofPopStyle();
}

void hRadio::mousePressed(ofMouseEventArgs & args){
	if (args.x > xpos && args.x < xpos + size * number && args.y > ypos && args.y < ypos + size && !isTouch){
		value =  abs(roundf((args.x - xpos) / size - 0.5));
		boolean = true;
	}
}

void hRadio::touchDown(ofTouchEventArgs & args){
	if (args.x > xpos && args.x < xpos + size * number && args.y > ypos && args.y < ypos + size){
		isTouch = 1;
		value = abs(roundf((args.x - xpos) / size - 0.5));
		boolean = true;
	}
}