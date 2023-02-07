#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	auto data = xml.appendChild("data");
	xml.load("points.xml");
	ofSetWindowTitle("Game of Life");
	ofSetBackgroundColor(200);
	width = 800;
	height = 600;
	number_gridNumX.value = 10;
	number_gridNumY.value = 10;
	fboCells.allocate(number_gridNumX.value, number_gridNumY.value, GL_RGBA);
	fboCells.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	fboCells.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fboCellsCopy.allocate(number_gridNumX.value, number_gridNumY.value, GL_RGBA);
	fboCellsCopy.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	fboCellsCopy.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	fboLines.allocate(width, height);
	shaderGameOfLife.load("GameOfLife");
	shaderRaster.load("Raster");
	bang_copy.setup(50, 700, 30);
	bang_paste.setup(50, 740, 30);
	bang_save.setup(50, 780, 30);
	bang_reset.setup(50, 820, 30);
	bang_iterate.setup(620, 780, 30);
	toggle_grid.setup(620, 700, 30);
	toggle_grid.value = true;
	toggle_sequence.setup(620, 820, 30);
	number_gridNumX.setup(210, 700, 100, 30, 1, 100);
	number_gridNumY.setup(210, 740, 100, 30, 1, 100);
	number_interval.setup(550, 740, 100, 30, 1, 1000);
	number_interval.value = 200;
	for (int i = 0; i < NCELLS; i++) {
		groupOfLivingCells[i].setup(i * 30 + 210, 790, 30);
	}
	groupOfLivingCells[2].value = true;
	groupOfLivingCells[3].value = true;
	for (int i = 0; i < NCELLS; i++) {
		groupOfDeadCells[i].setup(i * 30 + 210, 820, 30);
	}
	groupOfDeadCells[3].value = true;
	hRadio_pattern.setup(50, 660, 30, 16);
	hRadio_pattern.value = 0;
	hRadio_pattern.boolean = true;
	groupOfLabels[0].setup(535, 660, 120, 30, "Pattern");
	groupOfLabels[1].setup(85, 700, 120, 30, "Copy");
	groupOfLabels[2].setup(85, 740, 120, 30, "Paste");
	groupOfLabels[3].setup(85, 780, 120, 30, "Save");
	groupOfLabels[4].setup(85, 820, 120, 30, "Reset");
	groupOfLabels[5].setup(315, 700, 120, 30, "X");
	groupOfLabels[6].setup(315, 740, 120, 30, "Y");
	groupOfLabels[7].setup(485, 790, 100, 60, "Cells");
	groupOfLabels[8].setup(655, 700, 120, 30, "Grid");
	groupOfLabels[9].setup(655, 740, 120, 30, "Interval");
	groupOfLabels[10].setup(655, 780, 120, 30, "Iterate");
	groupOfLabels[11].setup(655, 820, 120, 30, "Sequence");
}

//--------------------------------------------------------------
void ofApp::update() {
	if (now < ofGetElapsedTimeMillis() && toggle_sequence.value == true) {
		now = ofGetElapsedTimeMillis() + number_interval.value;
		bang_iterate.value = true;
	}
	if (bang_copy.value) {
		fboCells.readToPixels(pixels);
		auto data = xml.getChild("data");
		counter = 0;
		data.removeChild("copy");
		auto copy = data.appendChild("copy");
		copy.appendChild("a1").set(number_gridNumX.value);
		copy.appendChild("a2").set(number_gridNumY.value);
		for (int x = 0; x < NCELLS; x++) {
			copy.appendChild("a" + ofToString(x + 3)).set(groupOfLivingCells[x].value);
		}
		for (int x = 0; x < NCELLS; x++) {
			copy.appendChild("a" + ofToString(x + 3 + NCELLS)).set(groupOfDeadCells[x].value);
		}
		for (int x = 0; x < number_gridNumX.value; x++) {
			for (int y = 0; y < number_gridNumY.value; y++) {
				if (pixels.getColor(x, y).a == 255) {
					copy.appendChild("a" + ofToString(counter + 3 + NCELLS * 2.)).set((long long)number_gridNumY.value * x + y);
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
		if (number_gridNumX.value != oldGridNumX || number_gridNumY.value != oldGridNumY) {
			fboCells.allocate(number_gridNumX.value, number_gridNumY.value, GL_RGBA);
			fboCells.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
			fboCells.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			fboCellsCopy.allocate(number_gridNumX.value, number_gridNumY.value, GL_RGBA);
			fboCellsCopy.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
			fboCellsCopy.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			shaderRaster.begin();
			shaderRaster.setUniformTexture("Tex0", fboLines.getTexture(), 0);
			shaderRaster.setUniform2f("resolution", width, height);
			shaderRaster.setUniform2f("gridNums", number_gridNumX.value, number_gridNumY.value);
			fboLines.begin();
			ofClear(0);
			ofDrawRectangle(0, 0, width, height);
			fboLines.end();
			shaderRaster.end();
			oldGridNumX = number_gridNumX.value;
			oldGridNumY = number_gridNumY.value;
		}
		for (int x = 0; x < NCELLS; x++) {
			groupOfLivingCells[x].value = copy.getChild("a" + ofToString(x + 3)).getIntValue();
		}
		for (int x = 0; x < NCELLS; x++) {
			groupOfDeadCells[x].value = copy.getChild("a" + ofToString(x + 3 + NCELLS)).getIntValue();
		}
		fboCells.begin();
		ofClear(0);
		ofSetColor(0, 0, 0, 255);
		for (int i = 0; i < copy.getChild("a0").getIntValue(); i++) {
			int x = copy.getChild("a" + ofToString(i + 3 + NCELLS * 2)).getIntValue() / number_gridNumY.value;
			int y = copy.getChild("a" + ofToString(i + 3 + NCELLS * 2)).getIntValue() % (int)number_gridNumY.value;
			ofDrawRectangle(x, y, 1, 1);
		}
		fboCells.end();
		bang_paste.value = false;
	}
	if (bang_save.value) {
		fboCells.readToPixels(pixels);
		auto data = xml.getChild("data");
		counter = 0;
		data.removeChild("pattern" + ofToString(hRadio_pattern.value));
		auto pattern = data.appendChild("pattern" + ofToString(hRadio_pattern.value));
		pattern.appendChild("a1").set(number_gridNumX.value);
		pattern.appendChild("a2").set(number_gridNumY.value);
		for (int x = 0; x < NCELLS; x++) {
			pattern.appendChild("a" + ofToString(x + 3)).set(groupOfLivingCells[x].value);
		}
		for (int x = 0; x < NCELLS; x++) {
			pattern.appendChild("a" + ofToString(x + 3 + NCELLS)).set(groupOfDeadCells[x].value);
		}
		for (int x = 0; x < number_gridNumX.value; x++) {
			for (int y = 0; y < number_gridNumY.value; y++) {
				if (pixels.getColor(x, y).a == 255) {
					pattern.appendChild("a" + ofToString(counter + 3 + NCELLS * 2.)).set((long long)number_gridNumY.value * x + y);
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
		bang_reset.value = false;
	}
	if (hRadio_pattern.boolean) {
		auto data = xml.getChild("data");
		auto pattern = data.getChild("pattern" + ofToString(hRadio_pattern.value));
		if (pattern.getChild("a1").getIntValue() > 0 && pattern.getChild("a2").getIntValue() > 0) {
			number_gridNumX.value = pattern.getChild("a1").getIntValue();
			number_gridNumY.value = pattern.getChild("a2").getIntValue();
		}
		if (number_gridNumX.value != oldGridNumX || number_gridNumY.value != oldGridNumY) {
			fboCells.allocate(number_gridNumX.value, number_gridNumY.value, GL_RGBA);
			fboCells.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
			fboCells.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			fboCellsCopy.allocate(number_gridNumX.value, number_gridNumY.value, GL_RGBA);
			fboCellsCopy.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
			fboCellsCopy.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
			shaderRaster.begin();
			shaderRaster.setUniform2f("resolution", width, height);
			shaderRaster.setUniform2f("gridNums", number_gridNumX.value, number_gridNumY.value);
			fboLines.begin();
			ofClear(0);
			ofDrawRectangle(0, 0, width, height);
			fboLines.end();
			shaderRaster.end();
			oldGridNumX = number_gridNumX.value;
			oldGridNumY = number_gridNumY.value;
		}
		for (int x = 0; x < NCELLS; x++) {
			groupOfLivingCells[x].value = pattern.getChild("a" + ofToString(x + 3)).getIntValue();
		}
		for (int x = 0; x < NCELLS; x++) {
			groupOfDeadCells[x].value = pattern.getChild("a" + ofToString(x + 3 + NCELLS)).getIntValue();
		}
		fboCells.begin();
		ofClear(0);
		ofSetColor(0, 0, 0, 255);
		for (int i = 0; i < pattern.getChild("a0").getIntValue(); i++) {
			int x = pattern.getChild("a" + ofToString(i + 3 + NCELLS * 2)).getIntValue() / number_gridNumY.value;
			int y = pattern.getChild("a" + ofToString(i + 3 + NCELLS * 2)).getIntValue() % (int)number_gridNumY.value;
			ofDrawRectangle(x, y, 1, 1);
		}
		fboCells.end();
		hRadio_pattern.boolean = false;
	}
	if (number_gridNumX.value != oldGridNumX || number_gridNumY.value != oldGridNumY) {
		fboCells.allocate(number_gridNumX.value, number_gridNumY.value, GL_RGBA);
		fboCells.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		fboCells.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
		fboCellsCopy.allocate(number_gridNumX.value, number_gridNumY.value, GL_RGBA);
		fboCellsCopy.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		fboCellsCopy.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
		shaderRaster.begin();
		shaderRaster.setUniformTexture("Tex0", fboLines.getTexture(), 0);
		shaderRaster.setUniform2f("resolution", width, height);
		shaderRaster.setUniform2f("gridNums", number_gridNumX.value, number_gridNumY.value);
		fboLines.begin();
		ofClear(0);
		ofDrawRectangle(0, 0, width, height);
		fboLines.end();
		shaderRaster.end();
		oldGridNumX = number_gridNumX.value;
		oldGridNumY = number_gridNumY.value;
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
		};
		shaderGameOfLife.begin();
		shaderGameOfLife.setUniformTexture("Tex0", fboCells.getTexture(), 0);
		shaderGameOfLife.setUniform2f("resolution", fboCells.getWidth(), fboCells.getHeight());
		shaderGameOfLife.setUniform2fv("cells", cells, 18);
		fboCellsCopy.begin();
		ofClear(0);
		ofDrawRectangle(0, 0, fboCells.getWidth(), fboCells.getHeight());
		fboCellsCopy.end();
		shaderGameOfLife.end();
		std::swap(fboCells, fboCellsCopy);
		bang_iterate.value = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(240, 220, 100);
	ofDrawRectangle(20, 20, width, height);
	ofSetColor(20, 170, 150);
	ofDrawRectangle(20, 640, width, 220);
	fboCells.draw(20, 20, width, height);
	if (toggle_grid.value) {
		fboLines.draw(20, 20);
	}
	bang_copy.draw();
	bang_paste.draw();
	bang_save.draw();
	bang_reset.draw();
	bang_iterate.draw();
	toggle_grid.draw();
	toggle_sequence.draw();
	for (auto& element : groupOfLivingCells) {
		element.draw();
	}
	for (auto& element : groupOfDeadCells) {
		element.draw();
	}
	number_gridNumX.draw();
	number_gridNumY.draw();
	number_interval.draw();
	hRadio_pattern.draw();
	for (auto& element : groupOfLabels) {
		element.draw();
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs& args) {
	if (args.x > 20 && args.x < width + 20 && args.y > 20 && args.y < height + 20) {
		fboCells.readToPixels(pixels);
		int x = (args.x - 20) / (width / number_gridNumX.value);
		int y = (args.y - 20) / (height / number_gridNumY.value);
		fboCells.begin();
		ofDisableAlphaBlending();
		if (pixels.getColor(x, y).a == 255) {
			ofSetColor(255, 255, 255, 0);
			ofDrawRectangle(x, y, 1, 1);
		}
		else {
			ofSetColor(0, 0, 0, 255);
			ofDrawRectangle(x, y, 1, 1);
		}
		fboCells.end();
	}
}
