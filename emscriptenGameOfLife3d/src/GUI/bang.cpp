#include "bang.h"

void bang::setup(float _xpos, float _ypos, float _size){
	ofAddListener(ofEvents().mousePressed, this, &bang::mousePressed);
	ofAddListener(ofEvents().touchDown, this, &bang::touchDown);
	xpos = _xpos;
	ypos = _ypos;
	size = _size;
	isTouch = false;
}

void bang::draw(){
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(xpos, ypos);
	ofSetCircleResolution(100);
	ofSetColor(200);
	ofDrawRectangle(0, 0, size, size);
	ofSetColor(100);
	ofDrawRectangle(size / 40, size / 40, size - size / 20, size - size / 20);
	ofSetColor(255, 200, 200);
	if (now < ofGetElapsedTimeMillis()){
		ofSetColor(200);
	}
	ofDrawCircle(size / 2, size / 2, size / 2.7);
	ofPopMatrix();
	ofPopStyle();
}

void bang::mousePressed(ofMouseEventArgs & args){
	if (ofDist(args.x, args.y, xpos + size / 2, ypos + size / 2) <= size / 2.5 && !isTouch){
		now = ofGetElapsedTimeMillis() + 200;
		value = true;
	}
}

void bang::touchDown(ofTouchEventArgs & args){
	if (ofDist(args.x, args.y, xpos + size / 2, ypos + size / 2) <= size / 2.5){
		isTouch = true;
		now = ofGetElapsedTimeMillis() + 200;
		value = true;
	}
}