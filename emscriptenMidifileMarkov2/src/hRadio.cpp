#include "hRadio.h"

hRadio::hRadio(){
	bRegisteredEvents = false;
}

hRadio::~hRadio() {
	clear();
}

void hRadio::setup(float sendXpos, float sendYpos, float sendSize, float sendNumber){
	if(!bRegisteredEvents) {
		ofRegisterMouseEvents(this);
		ofRegisterTouchEvents(this);
		bRegisteredEvents = true;
	}
	xpos = sendXpos;
	ypos = sendYpos;
	number = sendNumber;
	size = sendSize;
	isTouch = false;
	value = false;
}

void hRadio::update(){

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

void hRadio::clear() {
	if(bRegisteredEvents) {
		ofUnregisterMouseEvents(this);
		ofUnregisterTouchEvents(this);
	bRegisteredEvents = false;
	}
}

void hRadio::mouseMoved(ofMouseEventArgs & args){

}

void hRadio::mouseDragged(ofMouseEventArgs & args){

}

void hRadio::mousePressed(ofMouseEventArgs & args){
	if (args.x > xpos && args.x < xpos + size * number && args.y > ypos && args.y < ypos + size && !isTouch){
		value =  roundf((args.x - xpos) / size - 0.5);
		ofNotifyEvent(onMousePressed, value, this); 
	}
}

void hRadio::mouseReleased(ofMouseEventArgs & args){

}

void hRadio::mouseScrolled(ofMouseEventArgs & args){

}

void hRadio::mouseEntered(ofMouseEventArgs & args){

}

void hRadio::mouseExited(ofMouseEventArgs & args){

}

void hRadio::touchDown(ofTouchEventArgs & args){
	if (args.x > xpos && args.x < xpos + size * number && args.y > ypos && args.y < ypos + size){
		isTouch = 1;
		value = roundf((args.x - xpos) / size - 0.5);
		ofNotifyEvent(onMousePressed, value, this); 
	}
}

void hRadio::touchMoved(ofTouchEventArgs & args){

}

void hRadio::touchUp(ofTouchEventArgs & args){

}

void hRadio::touchDoubleTap(ofTouchEventArgs & args){

}

void hRadio::touchCancelled(ofTouchEventArgs & args){

}
