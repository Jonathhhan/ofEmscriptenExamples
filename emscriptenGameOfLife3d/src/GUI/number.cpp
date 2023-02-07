#include "number.h"

number::number(){
    bRegisteredEvents = false;
}

number::~number() {
    clear();
}

void number::setup(float sendXpos, float sendYpos, float sendWidth, float sendHeight, float sendMinVal, float sendMaxVal){
    if(!bRegisteredEvents) {
        ofRegisterMouseEvents(this);
        ofRegisterTouchEvents(this);
        ofRegisterKeyEvents(this);
        bRegisteredEvents = true;
    }
	xpos = sendXpos;
	ypos = sendYpos;
	width = sendWidth;
	height = sendHeight;
	minVal = sendMinVal;
	maxVal = sendMaxVal;
	isTouch = false;
	mouseState = false;
	decimalPlace = true;
 	multiply = 1;

 key = 0;
 value = 0;
 previewValue = 0;
 preview = false;
 count = 0;
 now = 0;
 previousMouseY = 0;

}

void number::update(){

}

void number::draw(){
ofPushMatrix();
ofPushStyle();
ofTranslate(xpos, ypos);
ofSetColor(0);
ofDrawRectangle(0, 0, width, height);
ofSetColor(255);
ofDrawRectangle(2, 2, width - 4, height - 4);
if (!mouseState){
ofSetColor(0);
}
else{
ofSetColor(100, 200, 255);
}
if (preview){
ofDrawBitmapString(ofToString(previewValue) + ">", 8, 20);
}
else{
ofDrawBitmapString(ofToString(value), 8, 20);
}
ofPopMatrix();
ofPopStyle();
if (now < ofGetElapsedTimeMillis()){
preview = false;
previewValue = 0;
count = 0;
decimalPlace = false;
multiply = 1;
}
}

void number::clear() {
    if(bRegisteredEvents) {
        ofUnregisterMouseEvents(this);
        ofUnregisterTouchEvents(this);
        ofRegisterKeyEvents(this);
        bRegisteredEvents = false;
    }
}

void number::mouseMoved(ofMouseEventArgs & args){

}

void number::mouseDragged(ofMouseEventArgs & args){
if (mouseState && !isTouch){
if (key == 83){
value = (previousMouseY - args.y) / 1000 + value;
}
else{
value = (previousMouseY - args.y) + value;
}
previousMouseY = args.y;
if (value > maxVal){
value = maxVal;
}
if (value < minVal){
value = minVal;
}
ofNotifyEvent(onMousePressed, value, this); 
}
}

void number::mousePressed(ofMouseEventArgs & args){
if (args.x > xpos && args.x < xpos + width && args.y > ypos && args.y < ypos + height && !isTouch){
count = 0;
previewValue = 0;
preview = false;
decimalPlace = false;
multiply = 1;
mouseState = true;
previousMouseY = args.y;
}
else{
mouseState = 0;
previousMouseY = args.y;
}
}

void number::mouseReleased(ofMouseEventArgs & args){

}

void number::mouseScrolled(ofMouseEventArgs & args){

}

void number::mouseEntered(ofMouseEventArgs & args){

}

void number::mouseExited(ofMouseEventArgs & args){

}

void number::touchDown(ofTouchEventArgs & args){
if (args.x > xpos && args.x < xpos + width && args.y > ypos && args.y < ypos + height){
isTouch = true;
count = 0;
previewValue = 0;
preview = false;
decimalPlace = false;
multiply = 1;
mouseState = true;
previousMouseY = args.y;
}
else{
mouseState = false;
previousMouseY = args.y;
}
}

void number::touchMoved(ofTouchEventArgs & args){
if (mouseState){
if (key == 83){
value = (previousMouseY - args.y) / 1000 + value;
previousMouseY = args.y;
}
else{
value = (previousMouseY - args.y) + value;
previousMouseY = args.y;
}
if (value > maxVal){
value = maxVal;
}
if (value < minVal){
value = minVal;
}
ofNotifyEvent(onMousePressed, value, this); 
}
}

void number::touchUp(ofTouchEventArgs & args){

}

void number::touchDoubleTap(ofTouchEventArgs & args){

}

void number::touchCancelled(ofTouchEventArgs & args){

}

void number::keyPressed(ofKeyEventArgs & args){
	key = args.key;
	if (key == 43 && mouseState && count == 0){
		now = ofGetElapsedTimeMillis() + 3000;
		multiply = 1;
		}
	if (key == 45 && mouseState && count == 0){
		now = ofGetElapsedTimeMillis() + 3000;
		multiply = - 1;
	}
	if (key == 46 && mouseState){
		decimalPlace = true;
		}
if (key == 83 && mouseState){
decimalPlace = true;
}
if (key >= 48 && key <= 57 && mouseState){
now = ofGetElapsedTimeMillis() + 3000;
preview = true;
if (!decimalPlace){
previewValue = previewValue * 10 + (key - 48) * multiply;
}
else{
count = count + 1;
previewValue = previewValue + ((key - 48) * multiply / pow(10, count));
}
}
if (key == 69 && preview){
	value = previewValue;
	if (value > maxVal){
		value = maxVal;
		}
	if (value < minVal){
		value = minVal;
		}
	preview = false;
	mouseState = false;
	ofNotifyEvent(onMousePressed, value, this); 
	}
}

void number::keyReleased(ofKeyEventArgs & args){
if(args.key == 83){
key = 0;
}
}
