// Refer to the README.md in the example's root folder for more information on usage

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	auto data = xml.appendChild("data");
	xml.load("points.xml");
	gridNumX = 10;
	gridNumY = 10;
        ofSetWindowTitle("Game of Life");
        ofSetBackgroundColor(200);
        width = ofGetWidth() - 40;
        height = (ofGetHeight() - 60)/1.5;
	fboCells.allocate(gridNumX, gridNumY);
	fboCellsCopy.allocate(gridNumX, gridNumY);
	fboLines.allocate(width, height);
	shaderGameOfLife.load("GameOfLife");
	shaderRaster.load("Raster");
	ofAddListener(toggle_1.onMousePressed, this, &ofApp::toggle_1onMousePressed);
	ofAddListener(toggle_2.onMousePressed, this, &ofApp::toggle_2onMousePressed);
	ofAddListener(livingCell_1.onMousePressed, this, &ofApp::livingCell_1onMousePressed);
	ofAddListener(livingCell_2.onMousePressed, this, &ofApp::livingCell_2onMousePressed);
	ofAddListener(livingCell_3.onMousePressed, this, &ofApp::livingCell_3onMousePressed);
	ofAddListener(livingCell_4.onMousePressed, this, &ofApp::livingCell_4onMousePressed);
	ofAddListener(livingCell_5.onMousePressed, this, &ofApp::livingCell_5onMousePressed);
	ofAddListener(livingCell_6.onMousePressed, this, &ofApp::livingCell_6onMousePressed);
	ofAddListener(livingCell_7.onMousePressed, this, &ofApp::livingCell_7onMousePressed);
	ofAddListener(livingCell_8.onMousePressed, this, &ofApp::livingCell_8onMousePressed);
	ofAddListener(livingCell_9.onMousePressed, this, &ofApp::livingCell_9onMousePressed);
	ofAddListener(deadCell_1.onMousePressed, this, &ofApp::deadCell_1onMousePressed);
	ofAddListener(deadCell_2.onMousePressed, this, &ofApp::deadCell_2onMousePressed);
	ofAddListener(deadCell_3.onMousePressed, this, &ofApp::deadCell_3onMousePressed);
	ofAddListener(deadCell_4.onMousePressed, this, &ofApp::deadCell_4onMousePressed);
	ofAddListener(deadCell_5.onMousePressed, this, &ofApp::deadCell_5onMousePressed);
	ofAddListener(deadCell_6.onMousePressed, this, &ofApp::deadCell_6onMousePressed);
	ofAddListener(deadCell_7.onMousePressed, this, &ofApp::deadCell_7onMousePressed);
	ofAddListener(deadCell_8.onMousePressed, this, &ofApp::deadCell_8onMousePressed);
	ofAddListener(deadCell_9.onMousePressed, this, &ofApp::deadCell_9onMousePressed);
	ofAddListener(bang_1.onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(bang_3.onMousePressed, this, &ofApp::bang_3onMousePressed);
	ofAddListener(bang_4.onMousePressed, this, &ofApp::bang_4onMousePressed);
	ofAddListener(bang_5.onMousePressed, this, &ofApp::bang_5onMousePressed);
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(number_2.onMousePressed, this, &ofApp::number_2onMousePressed);
	ofAddListener(number_3.onMousePressed, this, &ofApp::number_3onMousePressed);
	ofAddListener(hRadio_1.onMousePressed, this, &ofApp::hRadio_1onMousePressed);
	bang_1.setup(50, 640, 30);
	bang_2.setup(50, 680, 30);
	bang_3.setup(50, 720, 30);
	bang_4.setup(50, 760, 30);
	bang_5.setup(620, 720, 30);
	toggle_1.setup(620, 760, 30);
	toggle_2.setup(620, 640, 30);
	toggle_2.value = true;
	raster = true;
	number_1.setup(210, 640, 100, 30, 1, 2000);
	number_1.value = 10;
	number_2.setup(210, 680, 100, 30, 1, 2000);
	number_2.value = 10;
	number_3.setup(550, 680, 100, 30, 1, 1000);
	number_3.value = 200;
	interval = 200;
	livingCell_1.setup(210, 730, 30);
	livingCell_2.setup(240, 730, 30);
	livingCell_3.setup(270, 730, 30);
	livingCell_3.value = true;
	livingCell_4.setup(300, 730, 30);
	livingCell_4.value = true;
	livingCell_5.setup(330, 730, 30);
	livingCell_6.setup(360, 730, 30);
	livingCell_7.setup(390, 730, 30);
	livingCell_8.setup(420, 730, 30);
	livingCell_9.setup(450, 730, 30);
	deadCell_1.setup(210, 760, 30);
	deadCell_2.setup(240, 760, 30);
	deadCell_3.setup(270, 760, 30);
	deadCell_4.setup(300, 760, 30);
	deadCell_4.value = true;
	deadCell_5.setup(330, 760, 30);
	deadCell_6.setup(360, 760, 30);
	deadCell_7.setup(390, 760, 30);
	deadCell_8.setup(420, 760, 30);
	deadCell_9.setup(450, 760, 30);
	hRadio_1.setup(50, 600, 30, 16);
	hRadio_1.value = 0;
	float presetPattern = 0;
	hRadio_1onMousePressed(presetPattern);
	label_1.setup(535, 600, 120, 30, "Pattern");
	label_2.setup(85, 640, 120, 30, "Copy");
	label_3.setup(85, 680, 120, 30, "Paste");
	label_4.setup(85, 720, 120, 30, "Save");
	label_5.setup(85, 760, 120, 30, "Reset");
	label_6.setup(315, 640, 120, 30, "X");
	label_7.setup(315, 680, 120, 30, "Y");
	label_8.setup(485, 730, 100, 60, "Cells");
	label_9.setup(655, 640, 120, 30, "Grid");
	label_10.setup(655, 680, 120, 30, "Interval");
	label_11.setup(655, 720, 120, 30, "Iterate");
	label_12.setup(655, 760, 120, 30, "Sequence");
	cellArray[0][2] = 1;
	cellArray[0][3] = 1;
	cellArray[1][3] = 1;
}

//--------------------------------------------------------------
void ofApp::update(){
	if (now < ofGetElapsedTimeMillis() && sequence == true){
		now = ofGetElapsedTimeMillis() + interval;
		bool update = true;
		bang_5onMousePressed(update);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(240, 220, 100);
	ofDrawRectangle(20, 20, 800, 800 / 1.5);
	ofSetColor(20, 170, 150);
	ofDrawRectangle(20, (ofGetHeight() - 60)/ 3 * 2 + 40, 800, ((ofGetHeight()- 60)/ 3 ));
	fboCells.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	fboCells.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fboCells.draw(20, 20, width, height);
	if (raster) {
		fboLines.draw(20, 20);
		}
	bang_1.draw();
	bang_2.draw();
	bang_3.draw();
	bang_4.draw();
	bang_5.draw();
	toggle_1.draw();
	toggle_2.draw();
	livingCell_1.draw();
	livingCell_2.draw();
	livingCell_3.draw();
	livingCell_4.draw();
	livingCell_5.draw();
	livingCell_6.draw();
	livingCell_7.draw();
	livingCell_8.draw();
	livingCell_9.draw();
	deadCell_1.draw();
	deadCell_2.draw();
	deadCell_3.draw();
	deadCell_4.draw();
	deadCell_5.draw();
	deadCell_6.draw();
	deadCell_7.draw();
	deadCell_8.draw();
	deadCell_9.draw();
	number_1.draw();
	number_2.draw();
	number_3.draw();
	hRadio_1.draw();
	label_1.draw();
	label_2.draw();
	label_3.draw();
	label_4.draw();
	label_5.draw();
	label_6.draw();
	label_7.draw();
	label_8.draw();
	label_9.draw();
	label_10.draw();
	label_11.draw();
	label_12.draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed  (ofKeyEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::keyReleased  (ofKeyEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::toggle_1onMousePressed(bool & e){
	sequence = e;
}

//--------------------------------------------------------------
void ofApp::toggle_2onMousePressed(bool & e){
	raster = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_1onMousePressed(bool & e){
	cellArray[0][0] = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_2onMousePressed(bool & e){
	cellArray[0][1] = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_3onMousePressed(bool & e){
	cellArray[0][2] = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_4onMousePressed(bool & e){
	cellArray[0][3] = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_5onMousePressed(bool & e){
	cellArray[0][4] = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_6onMousePressed(bool & e){
	cellArray[0][5] = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_7onMousePressed(bool & e){
	cellArray[0][6] = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_8onMousePressed(bool & e){
	cellArray[0][7] = e;
}

//--------------------------------------------------------------
void ofApp::livingCell_9onMousePressed(bool & e){
	cellArray[0][8] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_1onMousePressed(bool & e){
	cellArray[1][0] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_2onMousePressed(bool & e){
	cellArray[1][1] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_3onMousePressed(bool & e){
	cellArray[1][2] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_4onMousePressed(bool & e){
	cellArray[1][3] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_5onMousePressed(bool & e){
	cellArray[1][4] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_6onMousePressed(bool & e){
	cellArray[1][5] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_7onMousePressed(bool & e){
	cellArray[1][6] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_8onMousePressed(bool & e){
	cellArray[1][7] = e;
}

//--------------------------------------------------------------
void ofApp::deadCell_9onMousePressed(bool & e){
	cellArray[1][8] = e;
}

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e){
fboCells.readToPixels(pixels);
auto data = xml.getChild("data");
counter = 3;
data.removeChild("copy");
auto copy = data.appendChild("copy");
copy.appendChild("a1").set(gridNumX);
copy.appendChild("a2").set(gridNumY);
for (x = 0; x < gridNumX; x++) {
for (y = 0; y < gridNumY; y++) {
	if (pixels.getColor(x, y).a == 255) {
	copy.appendChild("a" + ofToString(counter)).set(gridNumY * x + y - 1);
		counter = counter + 1;
		}
	}
	}
	copy.appendChild("a0").set(counter);
xml.save("points.xml");
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e){
auto data = xml.getChild("data");
auto copy = data.getChild("copy");
gridNumX = copy.getChild("a1").getIntValue();
gridNumY = copy.getChild("a2").getIntValue();
number_1.value = gridNumX;
number_2.value = gridNumY;

if (gridNumX != oldGridNumX || gridNumY != oldGridNumY){
fboCells.allocate(gridNumX, gridNumY);
fboCellsCopy.allocate(gridNumX, gridNumY);
shaderRaster.begin();
shaderRaster.setUniformTexture("Tex0", fboLines.getTexture(), 0);
shaderRaster.setUniform2f("resolution", width, height);
shaderRaster.setUniform1f("gridNumX", gridNumX);
shaderRaster.setUniform1f("gridNumY", gridNumY);
fboLines.begin();
ofClear(0);
ofDrawRectangle(0, 0, width, height);
fboLines.end();
shaderRaster.end();
oldGridNumX = gridNumX;
oldGridNumY = gridNumY;
}

fboCells.begin();
ofClear(0);
ofSetColor(0, 0, 0, 255);
for(int i = 3; i < copy.getChild("a0").getIntValue(); i++){
x = copy.getChild("a" + ofToString(i)).getIntValue() / gridNumY + 1;
y = int(copy.getChild("a" + ofToString(i)).getIntValue()) % gridNumY + 1;
ofDrawRectangle(x * 1 - 1, y * 1 , 1, 1);
}
fboCells.end();	
}

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool & e){
fboCells.readToPixels(pixels);
auto data = xml.getChild("data");
counter = 3;
data.removeChild("pattern" + ofToString(pattern));
auto pattern2 = data.appendChild("pattern" + ofToString(pattern));
pattern2.appendChild("a1").set(gridNumX);
pattern2.appendChild("a2").set(gridNumY);
for (x = 0; x < gridNumX; x++) {
for (y = 0; y < gridNumY; y++) {
	if (pixels.getColor(x, y).a == 255) {
	pattern2.appendChild("a" + ofToString(counter)).set(gridNumY * x + y - 1);
		counter = counter + 1;
		}
	}
	}
	pattern2.appendChild("a0").set(counter);
xml.save("points.xml");
}

//--------------------------------------------------------------
void ofApp::bang_4onMousePressed(bool & e){
fboCells.begin();
ofClear(0);
fboCells.end();
}

//--------------------------------------------------------------
void ofApp::bang_5onMousePressed(bool & e){
shaderGameOfLife.begin();
shaderGameOfLife.setUniformTexture("Tex0", fboCells.getTexture(), 0);
shaderGameOfLife.setUniform2f("resolution", gridNumX, gridNumY);
shaderGameOfLife.setUniform1f("lCell_0", cellArray[0][0]);
shaderGameOfLife.setUniform1f("lCell_1", cellArray[0][1]);
shaderGameOfLife.setUniform1f("lCell_2", cellArray[0][2]);
shaderGameOfLife.setUniform1f("lCell_3", cellArray[0][3]);
shaderGameOfLife.setUniform1f("lCell_4", cellArray[0][4]);
shaderGameOfLife.setUniform1f("lCell_5", cellArray[0][5]);
shaderGameOfLife.setUniform1f("lCell_6", cellArray[0][6]);
shaderGameOfLife.setUniform1f("lCell_7", cellArray[0][7]);
shaderGameOfLife.setUniform1f("lCell_8", cellArray[0][8]);
shaderGameOfLife.setUniform1f("dCell_0", cellArray[1][0]);
shaderGameOfLife.setUniform1f("dCell_1", cellArray[1][1]);
shaderGameOfLife.setUniform1f("dCell_2", cellArray[1][2]);
shaderGameOfLife.setUniform1f("dCell_3", cellArray[1][3]);
shaderGameOfLife.setUniform1f("dCell_4", cellArray[1][4]);
shaderGameOfLife.setUniform1f("dCell_5", cellArray[1][5]);
shaderGameOfLife.setUniform1f("dCell_6", cellArray[1][6]);
shaderGameOfLife.setUniform1f("dCell_7", cellArray[1][7]);
shaderGameOfLife.setUniform1f("dCell_8", cellArray[1][8]);
fboCellsCopy.begin();
ofClear(0);
fboCells.draw(0, 0);
fboCellsCopy.end();
shaderGameOfLife.end();
fboCells.begin();
ofClear(0);
fboCellsCopy.draw(0, 0);
fboCells.end();
}

//--------------------------------------------------------------
void ofApp::number_1onMousePressed(float & e){
gridNumX = e;
oldGridNumX = gridNumX;
fboCells.allocate(gridNumX, gridNumY);
fboCellsCopy.allocate(gridNumX, gridNumY);
shaderRaster.begin();
shaderRaster.setUniformTexture("Tex0", fboLines.getTexture(), 0);
shaderRaster.setUniform2f("resolution", width, height);
shaderRaster.setUniform1f("gridNumX", gridNumX);
shaderRaster.setUniform1f("gridNumY", gridNumY);
fboLines.begin();
ofClear(0);
ofDrawRectangle(0, 0, width, height);
fboLines.end();
shaderRaster.end();
fboCells.begin();
ofClear(0);
fboCells.end();
}

//--------------------------------------------------------------
void ofApp::number_2onMousePressed(float & e){
gridNumY = e;
oldGridNumY = gridNumY;
fboCells.allocate(gridNumX, gridNumY);
fboCellsCopy.allocate(gridNumX, gridNumY);
shaderRaster.begin();
shaderRaster.setUniformTexture("Tex0", fboLines.getTexture(), 0);
shaderRaster.setUniform2f("resolution", width, height);
shaderRaster.setUniform1f("gridNumX", gridNumX);
shaderRaster.setUniform1f("gridNumY", gridNumY);
fboLines.begin();
ofClear(0);
ofDrawRectangle(0, 0, width, height);
fboLines.end();
shaderRaster.end();
fboCells.begin();
ofClear(0);
fboCells.end();
}

//--------------------------------------------------------------
void ofApp::number_3onMousePressed(float & e){
interval = e;
}

//--------------------------------------------------------------
void ofApp::hRadio_1onMousePressed(float & e){
 pattern = e;
auto data = xml.getChild("data");
auto pattern2 = data.getChild("pattern" + ofToString(pattern));
if (pattern2.getChild("a1").getIntValue() > 0 && pattern2.getChild("a2").getIntValue() > 2){
gridNumX = pattern2.getChild("a1").getIntValue();
gridNumY = pattern2.getChild("a2").getIntValue();
number_1.value = gridNumX;
number_2.value = gridNumY;
}

if (gridNumX != oldGridNumX || gridNumY != oldGridNumY){
fboCells.allocate(gridNumX, gridNumY);
fboCellsCopy.allocate(gridNumX, gridNumY);
shaderRaster.begin();
shaderRaster.setUniformTexture("Tex0", fboLines.getTexture(), 0);
shaderRaster.setUniform2f("resolution", width, height);
shaderRaster.setUniform1f("gridNumX", gridNumX);
shaderRaster.setUniform1f("gridNumY", gridNumY);
fboLines.begin();
ofClear(0);
ofDrawRectangle(0, 0, width, height);
fboLines.end();
shaderRaster.end();
oldGridNumX = gridNumX;
oldGridNumY = gridNumY;
}

fboCells.begin();
ofClear(0);
ofSetColor(0, 0, 0, 255);
for(int i = 3; i < pattern2.getChild("a0").getIntValue(); i++){
x = pattern2.getChild("a" + ofToString(i)).getIntValue() / gridNumY + 1;
y = int(pattern2.getChild("a" + ofToString(i)).getIntValue()) % gridNumY + 1;
ofDrawRectangle(x * 1 - 1, y * 1 , 1, 1);
}
fboCells.end();	
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
if (args.x > 20 && args.x < width + 20 && args.y > 20 && args.y < height + 20) {
fboCells.readToPixels(pixels);
int x = (args.x - 20 + (width / gridNumX)) / (width / gridNumX);
int y = (args.y - 20 + (height / gridNumY)) / (height / gridNumY);
fboCells.begin();
ofDisableAlphaBlending();
if (pixels.getColor(x - 1, y - 1).a == 255){
ofSetColor(255, 255, 255, 0);
ofDrawRectangle(x * 1 - 1, y * 1 - 1, 1, 1);
}
else {
ofSetColor(0, 0, 0, 255);
ofDrawRectangle(x * 1 - 1, y * 1 - 1, 1, 1);
}
fboCells.end();
}
	}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){

	}

//--------------------------------------------------------------
void ofApp::mouseEntered(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseExited(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
