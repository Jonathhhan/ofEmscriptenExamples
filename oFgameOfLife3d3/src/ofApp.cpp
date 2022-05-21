#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	videoPlayer.load("Peanuts.mp4");
	videoPlayer.play();
	videoPlayer.setVolume(0.5);
	number_gridNumX.value = 10;
	number_gridNumY.value = 10;
	number_gridNumZ.value = 10;
	auto data = xml.appendChild("data");
	xml.load("points.xml");
	ofSetWindowTitle("Game of Life");
	ofSetBackgroundColor(200);
	width = 800;
	height = 600;
	fboCells.allocate(number_gridNumX.value, number_gridNumY.value);
	fboCellsCopy.allocate(number_gridNumX.value, number_gridNumY.value);
	fboLines.allocate(width, height);
	shaderGameOfLife.load("GameOfLife");
	shaderGameOfLifeSlice.load("Slice3d");
	videoShader.load("video");
	shaderRaster.load("Raster");
	bang_copy.setup(50, 690, 30);
	bang_paste.setup(50, 730, 30);
	bang_save.setup(50, 770, 30);
	bang_reset.setup(50, 810, 30);
	bang_update.setup(280, 810, 30);
	bang_iterate.setup(620, 770, 30);
	toggle_sequence.setup(620, 810, 30);
	toggle_grid.setup(620, 690, 30);
	toggle_grid.value = true;
	number_gridNumX.setup(210, 690, 100, 30, 1, 2000);
	number_gridNumY.setup(210, 730, 100, 30, 1, 2000);
	number_gridNumZ.setup(210, 770, 100, 30, 1, 2000);
	number_interval.setup(550, 730, 100, 30, 1, 1000);
	number_interval.value = 200;
	for (int i = 0; i < NCELLS; i++) {
		groupOfLivingCells[i].setup(i * 20 + 50, 850, 20);
	}
	groupOfLivingCells[2].value = true;
	groupOfLivingCells[3].value = true;
	for (int i = 0; i < NCELLS; i++) {
		groupOfDeadCells[i].setup(i * 20 + 50, 870, 20);
	}
	groupOfDeadCells[3].value = true;
	hRadio_pattern.setup(50, 650, 30, 16);
	hRadio_pattern.value = 0;
	hRadio_pattern.boolean = true;
	groupOfLabels[0].setup(535, 650, 120, 30, "Pattern");
	groupOfLabels[1].setup(85, 690, 120, 30, "Copy");
	groupOfLabels[2].setup(85, 730, 120, 30, "Paste");
	groupOfLabels[3].setup(85, 770, 120, 30, "Save");
	groupOfLabels[4].setup(85, 810, 120, 30, "Reset");
	groupOfLabels[5].setup(315, 690, 120, 30, "X");
	groupOfLabels[6].setup(315, 730, 120, 30, "Y");
	groupOfLabels[7].setup(315, 770, 120, 30, "Z");
	groupOfLabels[8].setup(315, 810, 120, 30, "Update");
	groupOfLabels[10].setup(655, 690, 120, 30, "Grid");
	groupOfLabels[11].setup(655, 730, 120, 30, "Interval");
	groupOfLabels[12].setup(655, 770, 120, 30, "Iterate");
	groupOfLabels[13].setup(655, 810, 120, 30, "Sequence");
	groupOfLabels[9].setup(595, 850, 180, 40, "Cells");
}

//--------------------------------------------------------------
void ofApp::update() {
	videoPlayer.update();
	if (videoPlayer.isFrameNew()) {
		ofTexture tex;
		tex.allocate(videoPlayer.getTexture().getWidth(), videoPlayer.getTexture().getHeight(), GL_RGBA);
		std::swap(tex, videoPlayer.getTexture());
		frames.push_front(tex);
		if (frames.size() > texture3d.getVolumeDepth() && !bang_iterate.value) {
			frames.pop_back();
			fboCellsCopy.begin();
			for (int x = 0; x < texture3d.getVolumeDepth(); x++) {
				ofClear(255);
				frames[x].draw(0,0,fboCells.getWidth(), fboCells.getHeight());
				texture3dVideo.updateTexture(0, 0, x, 0, 0, fboCells.getWidth(), fboCells.getHeight());
				videoShader.begin();
				videoShader.setUniformTexture("Tex0", GL_TEXTURE_3D, texture3d.getTextureData().textureID, 0);
				videoShader.setUniformTexture("Tex1", GL_TEXTURE_3D, texture3dVideo.getTextureData().textureID, 1);
				videoShader.setUniform3f("resolution", texture3d.getVolumeWidth(), texture3d.getVolumeHeight(), texture3d.getVolumeDepth());
				videoShader.setUniform1i("zCount", x);
				ofClear(255);
				ofDrawRectangle(0, 0, fboCells.getWidth(), fboCells.getHeight());
				texture3d.updateTexture(0, 0, x, 0, 0, fboCells.getWidth(), fboCells.getHeight());
				videoShader.end();
			}
			fboCellsCopy.end();
		}
	}
	if (now < ofGetElapsedTimeMillis() && toggle_sequence.value) {
		now = ofGetElapsedTimeMillis() + number_interval.value;
		bang_iterate.value = true;
	}

	if (bang_update.value) {
		texture3d.setup(number_gridNumX.value, number_gridNumY.value, number_gridNumZ.value, ofVec3f(1, 0.5, 1), false);
		texture3d.setRenderSettings(1.0, 1.0, 1, 0.1);
		fboCells.allocate(number_gridNumX.value, number_gridNumY.value);
		fboCellsCopy.allocate(number_gridNumX.value, number_gridNumY.value);
		shaderRaster.begin();
		shaderRaster.setUniform2f("resolution", width, height);
		shaderRaster.setUniform1f("gridNumX", number_gridNumX.value);
		shaderRaster.setUniform1f("gridNumY", number_gridNumY.value);
		fboLines.begin();
		ofClear(0);
		ofDrawRectangle(0, 0, width, height);
		fboLines.end();
		shaderRaster.end();
		oldGridNumX = number_gridNumX.value;
		oldGridNumY = number_gridNumY.value;
		bang_update.value = false;
	}

	if (bang_copy.value) {
		fboCells.readToPixels(pixels);
		auto data = xml.getChild("data");
		long long counter = 0;
		data.removeChild("copy");
		auto copy = data.appendChild("copy");
		copy.appendChild("a1").set(number_gridNumX.value);
		copy.appendChild("a2").set(number_gridNumY.value);
		copy.appendChild("a2").set(number_gridNumZ.value);
		for (int x = 0; x < NCELLS; x++) {
			copy.appendChild("a" + ofToString(x + 4)).set(groupOfLivingCells[x].value);
		}
		for (int x = 0; x < NCELLS; x++) {
			copy.appendChild("a" + ofToString(x + 4 + NCELLS)).set(groupOfDeadCells[x].value);
		}
		for (int x = 0; x < number_gridNumX.value; x++) {
			for (int y = 0; y < number_gridNumY.value; y++) {
				if (pixels.getColor(x, y).a == 255) {
					copy.appendChild("a" + ofToString(counter + 4 + NCELLS * 2.)).set((long long)number_gridNumY.value * x + y - 1);
					counter++;
				}
			}
		}
		copy.appendChild("a0").set(counter);
		xml.save("points.xml");
		bang_copy.value = false;
	}

	if (bang_paste.value) {
		auto data = xml.getChild("data");
		auto copy = data.getChild("copy");
		number_gridNumX.value = copy.getChild("a1").getIntValue();
		number_gridNumY.value = copy.getChild("a2").getIntValue();
		number_gridNumZ.value = copy.getChild("a3").getIntValue();
		if (number_gridNumX.value != oldGridNumX || number_gridNumY.value != oldGridNumY || number_gridNumZ.value != oldGridNumZ) {
			fboCells.allocate(number_gridNumX.value, number_gridNumY.value);
			fboCellsCopy.allocate(number_gridNumX.value, number_gridNumY.value);
			texture3d.setup(number_gridNumX.value, number_gridNumY.value, number_gridNumZ.value, ofVec3f(1, 0.5, 1), false);
			texture3d.setRenderSettings(1.0, 5.0, 1, 0.1);
			texture3dCopy.setup(number_gridNumX.value, number_gridNumY.value, number_gridNumZ.value, ofVec3f(1, 0.5, 1), false);
			shaderRaster.begin();
			shaderRaster.setUniform2f("resolution", width, height);
			shaderRaster.setUniform1f("gridNumX", number_gridNumX.value);
			shaderRaster.setUniform1f("gridNumY", number_gridNumY.value);
			fboLines.begin();
			ofClear(0);
			ofDrawRectangle(0, 0, width, height);
			fboLines.end();
			shaderRaster.end();
			oldGridNumX = number_gridNumX.value;
			oldGridNumY = number_gridNumY.value;
			oldGridNumZ = number_gridNumZ.value;
		}
		for (int x = 0; x < NCELLS; x++) {
			groupOfLivingCells[x].value = copy.getChild("a" + ofToString(x + 4)).getIntValue();
		}
		for (int x = 0; x < NCELLS; x++) {
			groupOfDeadCells[x].value = copy.getChild("a" + ofToString(x + 4 + NCELLS)).getIntValue();
		}
		fboCells.begin();
		ofClear(0);
		ofSetColor(0, 0, 0, 255);
		for (int i = 0; i < copy.getChild("a0").getIntValue(); i++) {
			int x = copy.getChild("a" + ofToString(i + 4 + NCELLS * 2)).getIntValue() / number_gridNumY.value + 1;
			int y = copy.getChild("a" + ofToString(i + 4 + NCELLS * 2)).getIntValue() % (int)number_gridNumY.value + 1;
			ofDrawRectangle(x * 1 - 1, y * 1, 1, 1);
		}
		fboCells.end();
		bang_paste.value = false;
	}

	if (bang_save.value) {
		long long counter = 0;
		fboCells.readToPixels(pixels);
		auto data = xml.getChild("data");
		data.removeChild("pattern" + ofToString(hRadio_pattern.value));
		auto pattern = data.appendChild("pattern" + ofToString(hRadio_pattern.value));
		pattern.appendChild("a1").set(number_gridNumX.value);
		pattern.appendChild("a2").set(number_gridNumY.value);
		pattern.appendChild("a3").set(number_gridNumZ.value);
		for (int x = 0; x < NCELLS; x++) {
			pattern.appendChild("a" + ofToString(x + 4)).set(groupOfLivingCells[x].value);
		}
		for (int x = 0; x < NCELLS; x++) {
			pattern.appendChild("a" + ofToString(x + 4 + NCELLS)).set(groupOfDeadCells[x].value);
		}
		for (int x = 0; x < number_gridNumX.value; x++) {
			for (int y = 0; y < number_gridNumY.value; y++) {
				if (pixels.getColor(x, y).a == 255) {
					pattern.appendChild("a" + ofToString(counter + 4 + NCELLS * 2.)).set((long long)number_gridNumY.value * x + y - 1);
					counter++;
				}
			}
		}
		pattern.appendChild("a0").set(counter);
		xml.save("points.xml");
		bang_save.value = false;
	}

	if (bang_reset.value) {
		fboCells.begin();
		ofClear(0);
		fboCells.end();
		texture3d.destroy();
		texture3d.setup(number_gridNumX.value, number_gridNumY.value, number_gridNumZ.value, ofVec3f(1, 0.5, 1), false);
		texture3d.setRenderSettings(1.0, 5.0, 1, 0.1);
		bang_reset.value = false;
	}

	if (bang_iterate.value) {
		float cells[54] = {
		groupOfLivingCells[0].value,
		groupOfDeadCells[0].value,
		groupOfLivingCells[1].value,
		groupOfDeadCells[1].value,
		groupOfLivingCells[2].value,
		groupOfDeadCells[2].value,
		groupOfLivingCells[3].value,
		groupOfDeadCells[3].value,
		groupOfLivingCells[4].value,
		groupOfDeadCells[4].value,
		groupOfLivingCells[5].value,
		groupOfDeadCells[5].value,
		groupOfLivingCells[6].value,
		groupOfDeadCells[6].value,
		groupOfLivingCells[7].value,
		groupOfDeadCells[7].value,
		groupOfLivingCells[8].value,
		groupOfDeadCells[8].value,
		groupOfLivingCells[9].value,
		groupOfDeadCells[9].value,
		groupOfLivingCells[10].value,
		groupOfDeadCells[10].value,
		groupOfLivingCells[11].value,
		groupOfDeadCells[11].value,
		groupOfLivingCells[12].value,
		groupOfDeadCells[12].value,
		groupOfLivingCells[13].value,
		groupOfDeadCells[13].value,
		groupOfLivingCells[14].value,
		groupOfDeadCells[14].value,
		groupOfLivingCells[15].value,
		groupOfDeadCells[15].value,
		groupOfLivingCells[16].value,
		groupOfDeadCells[16].value,
		groupOfLivingCells[17].value,
		groupOfDeadCells[17].value,
		groupOfLivingCells[18].value,
		groupOfDeadCells[18].value,
		groupOfLivingCells[19].value,
		groupOfDeadCells[19].value,
		groupOfLivingCells[20].value,
		groupOfDeadCells[20].value,
		groupOfLivingCells[21].value,
		groupOfDeadCells[21].value,
		groupOfLivingCells[22].value,
		groupOfDeadCells[22].value,
		groupOfLivingCells[23].value,
		groupOfDeadCells[23].value,
		groupOfLivingCells[24].value,
		groupOfDeadCells[24].value,
		groupOfLivingCells[25].value,
		groupOfDeadCells[25].value,
		groupOfLivingCells[26].value,
		groupOfDeadCells[26].value
		};
		fboCellsCopy.begin();
		for (int x = 0; x < texture3d.getVolumeDepth(); x++) {
			shaderGameOfLife.begin(); 
			shaderGameOfLife.setUniformTexture("Tex0", GL_TEXTURE_3D, texture3d.getTextureData().textureID, 0);
			shaderGameOfLife.setUniformTexture("Tex1", GL_TEXTURE_3D, texture3dVideo.getTextureData().textureID, 1);
			videoShader.setUniform2f("res", videoPlayer.getTexture().getWidth(), videoPlayer.getTexture().getHeight());
			shaderGameOfLife.setUniform3f("resolution", texture3d.getVolumeWidth(), texture3d.getVolumeHeight(), texture3d.getVolumeDepth());
			shaderGameOfLife.setUniform2fv("cells", cells, 54);
			shaderGameOfLife.setUniform1i("zCount", x);
			ofClear(255);
			ofDrawRectangle(0, 0, fboCells.getWidth(), fboCells.getHeight());
			texture3dCopy.updateTexture(0, 0, x, 0, 0, fboCells.getWidth(), fboCells.getHeight());
			shaderGameOfLife.end();
		}
		for (int x = 0; x < texture3d.getVolumeDepth(); x++) {
			shaderGameOfLifeSlice.begin();
			shaderGameOfLifeSlice.setUniformTexture("Tex0", GL_TEXTURE_3D, texture3dCopy.getTextureData().textureID, 0);
			shaderGameOfLifeSlice.setUniform3f("resolution", texture3d.getVolumeWidth(), texture3d.getVolumeHeight(), texture3d.getVolumeDepth());
			shaderGameOfLifeSlice.setUniform1i("zCount", x);
			ofClear(255);
			ofDrawRectangle(0, 0, fboCells.getWidth(), fboCells.getHeight());
			texture3d.updateTexture(0, 0, x, 0, 0, fboCells.getWidth(), fboCells.getHeight());
			shaderGameOfLifeSlice.end();
		}
		fboCellsCopy.end();
		bang_iterate.value = false;
	}

	if (hRadio_pattern.boolean) {
		auto data = xml.getChild("data");
		auto pattern = data.getChild("pattern" + ofToString(hRadio_pattern.value));
		if (pattern.getChild("a1").getIntValue() > 0 && pattern.getChild("a2").getIntValue() > 0) {
			number_gridNumX.value = pattern.getChild("a1").getIntValue();
			number_gridNumY.value = pattern.getChild("a2").getIntValue();
			number_gridNumZ.value = pattern.getChild("a3").getIntValue();
		}
		if (number_gridNumX.value != oldGridNumX || number_gridNumY.value != oldGridNumY || number_gridNumZ.value != oldGridNumZ) {
			texture3d.setup(number_gridNumX.value, number_gridNumY.value, number_gridNumZ.value, ofVec3f(1, 0.5, 1), false);
			texture3d.setRenderSettings(1, 5, 0.5, 0.75);
			texture3dCopy.setup(number_gridNumX.value, number_gridNumY.value, number_gridNumZ.value, ofVec3f(1, 0.5, 1), false);
			texture3d.setRenderSettings(1, 5, 0.5, 0.75);
			texture3dVideo.setup(number_gridNumX.value, number_gridNumY.value, number_gridNumZ.value, ofVec3f(1, 0.5, 1), false);
			texture3d.setRenderSettings(1, 5, 0.5, 0.75);
			fboCells.allocate(number_gridNumX.value, number_gridNumY.value);
			fboCellsCopy.allocate(number_gridNumX.value, number_gridNumY.value);
			shaderRaster.begin();
			shaderRaster.setUniform2f("resolution", width, height);
			shaderRaster.setUniform1f("gridNumX", number_gridNumX.value);
			shaderRaster.setUniform1f("gridNumY", number_gridNumY.value);
			fboLines.begin();
			ofClear(0);
			ofDrawRectangle(0, 0, width, height);
			fboLines.end();
			shaderRaster.end();
			oldGridNumX = number_gridNumX.value;
			oldGridNumY = number_gridNumY.value;
			oldGridNumZ = number_gridNumZ.value;
		}
		for (int x = 0; x < NCELLS; x++) {
			groupOfLivingCells[x].value = pattern.getChild("a" + ofToString(x + 4)).getIntValue();
		}
		for (int x = 0; x < NCELLS; x++) {
			groupOfDeadCells[x].value = pattern.getChild("a" + ofToString(x + 4 + NCELLS)).getIntValue();
		}
		fboCells.begin();
		ofClear(0);
		ofSetColor(0, 0, 0, 255);
		for (int i = 0; i < pattern.getChild("a0").getIntValue(); i++) {
			int x = pattern.getChild("a" + ofToString(i + 4 + NCELLS * 2)).getIntValue() / number_gridNumY.value + 1;
			int y = pattern.getChild("a" + ofToString(i + 4 + NCELLS * 2)).getIntValue() % (int)number_gridNumY.value + 1;
			ofDrawRectangle(x * 1 - 1, y * 1, 1, 1);
		}
		texture3d.updateTexture(0, 0, number_gridNumZ.value/2, 0, 0, fboCells.getWidth(), fboCells.getHeight());
		fboCells.end();
		hRadio_pattern.boolean = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	// texture3d.setDrawDebugVolume(true);
	ofSetColor(255,100,100);
	ofDrawRectangle(20, 20, 800, 600);
	ofSetColor(20, 170, 150);
	ofDrawRectangle(20, 640, 800, 260);
	ofSetColor(255);
	ofRectangle viewport(20, 20, 800, 600);
	easyCam.begin(viewport);
	ofRotateY(ofGetElapsedTimeMicros() / 400000.);
	ofRotateX(10);
	ofSetRectMode(OF_RECTMODE_CORNER);
	texture3d.drawVolume(0, 75, 0, 300, 0);
	easyCam.end();
	if (toggle_grid.value) {
		fboCells.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		fboCells.draw(20, 20, width, height);
		fboLines.draw(20, 20);
	}
	bang_copy.draw();
	bang_paste.draw();
	bang_save.draw();
	bang_reset.draw();
	bang_update.draw();
	bang_iterate.draw();
	toggle_sequence.draw();
	toggle_grid.draw();
	for (auto& element : groupOfLivingCells) {
		element.draw();
	}
	for (auto& element : groupOfDeadCells) {
		element.draw();
	}
	number_gridNumX.draw();
	number_gridNumY.draw();
	number_gridNumZ.draw();
	number_interval.draw();
	hRadio_pattern.draw();
	for (auto& element : groupOfLabels) {
		element.draw();
	}
	//if(frames[0].isAllocated())
	//frames[0].draw(0,0,200,200);
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs& args) {
	if (args.x > 20 && args.x < width + 20 && args.y > 20 && args.y < height + 20 && toggle_grid.value) {
		fboCells.readToPixels(pixels);
		int x = (args.x - 20 + (width / number_gridNumX.value)) / (width / number_gridNumX.value);
		int y = (args.y - 20 + (height / number_gridNumY.value)) / (height / number_gridNumY.value);
		fboCells.begin();
		ofDisableAlphaBlending();
		if (pixels.getColor(x - 1., y - 1.).a == 255) {
			ofSetColor(255, 255, 255, 0);
			ofDrawRectangle(x * 1 - 1, y * 1 - 1, 1, 1);
		}
		else {
			ofSetColor(0, 0, 0, 255);
			ofDrawRectangle(x * 1 - 1, y * 1 - 1, 1, 1);
		}
		texture3d.updateTexture(0, 0, 0, 0, 0, fboCells.getWidth(), fboCells.getHeight());
		fboCells.end();
	}
}