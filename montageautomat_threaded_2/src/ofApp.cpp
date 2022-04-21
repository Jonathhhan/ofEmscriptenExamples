#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool& e) {
	embedLoaderThread.startThread();
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool& e) {
	if (vec2WordThread.embed.words > 0) {
		videoPlayerVector[numberOfVideoPlayer] -> stop();
		subtitleLoaderThread.startThread();
	} else {
		std::cout << "Please load an embedding file first!" << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool& e) {
	videoLoaderThread.startThread();
}

//--------------------------------------------------------------
void ofApp::bang_5onMousePressed(bool& e) {
	std::string string = ofSystemTextBoxDialog("Custom words:");
	std::vector<std::string> joinedWords;
	std::string lowerString = ofToLower(string);
	ofStringReplace(lowerString, ",", "");
	std::vector<std::string> splitWords = ofSplitString(lowerString, " ");
	for (auto &element : splitWords) {
		if (vec2WordThread.embed.find_case_sensitive(element) == - 1 && !element.empty()) {
			std::cout << "Word \"" << element << "\" does not exist! Choose another word or load an embedding file." << std::endl;
		} else {
			joinedWords.push_back(element);
		}
	}
	if (!joinedWords.empty()) {
		vec2WordThread.customWords = ofJoinString(joinedWords, " ");
		std::cout << "Custom words: " << vec2WordThread.customWords << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::bang_4onMousePressed(bool& e) {
	if (videoPlayerVector[numberOfVideoPlayer] -> isLoaded()) {
		groupOfToggles[0].value = 0;
		vec2WordThread.start = true;
		vec2WordThread.startThread();
	}
}

//--------------------------------------------------------------
void ofApp::toggle_1onMousePressed(bool& e) {
	if (e && videoPlayerVector[numberOfVideoPlayer] -> isLoaded()) {
		videoPlayerVector[numberOfVideoPlayer] -> setPaused(true);
	}
	else {
		videoPlayerVector[numberOfVideoPlayer] -> setPaused(false);
	}
}

//--------------------------------------------------------------
void ofApp::toggle_2onMousePressed(bool& e) {
	vec2WordThread.bRandomStart = e;
}

//--------------------------------------------------------------
void ofApp::toggle_3onMousePressed(bool& e) {
	vec2WordThread.bCustomWords = e;
}

//--------------------------------------------------------------
void ofApp::toggle_4onMousePressed(bool& e) {
	bReadMe = e;
}

//--------------------------------------------------------------
void ofApp::hSlider_1onMousePressed(float& e) {
	for (auto element : videoPlayerVector) {
		element -> setVolume(e);
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofAddListener(groupOfBangs[0].onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(groupOfBangs[1].onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(groupOfBangs[2].onMousePressed, this, &ofApp::bang_3onMousePressed);
	ofAddListener(groupOfBangs[3].onMousePressed, this, &ofApp::bang_4onMousePressed);
	ofAddListener(groupOfBangs[4].onMousePressed, this, &ofApp::bang_5onMousePressed);
	ofAddListener(groupOfToggles[0].onMousePressed, this, &ofApp::toggle_1onMousePressed);
	ofAddListener(groupOfToggles[1].onMousePressed, this, &ofApp::toggle_2onMousePressed);
	ofAddListener(groupOfToggles[2].onMousePressed, this, &ofApp::toggle_3onMousePressed);
	ofAddListener(groupOfToggles[3].onMousePressed, this, &ofApp::toggle_4onMousePressed);
	ofAddListener(hSlider_1.onMousePressed, this, &ofApp::hSlider_1onMousePressed);

	ofSetWindowTitle("Montageautomat 3");
	ofSetBackgroundColor(200);
	ofVideoPlayer* movie;
	movie = new ofVideoPlayer();
	videoPlayerVector.push_back(movie);

	groupOfBangs[0].setup(190, 40 + 5, 20);
	groupOfBangs[1].setup(190, 70 + 5, 20);
	groupOfBangs[2].setup(190, 100 + 5, 20);
	groupOfBangs[3].setup(190, 160 + 5, 20);
	groupOfToggles[0].setup(190, 190 + 5, 20);
	groupOfToggles[1].setup(190, 220 + 5, 20);
	hSlider_1.setup(190, 250 + 5, 80, 20, 0, 1);
	hSlider_1.value = 0.5;
	hSlider_1.slider = 0.5;
	groupOfToggles[2].setup(190, 300 + 5, 20);
	groupOfBangs[4].setup(190, 330 + 5, 20);
	groupOfToggles[3].setup(190, 380 + 5, 20);

	stopWords = {"ll", "re", "d", "m", "t", "s", "re"};
}

//--------------------------------------------------------------
void ofApp::update() {
	videoPlayerVector[numberOfVideoPlayer] -> update();
	if (embedLoaderThread.loaded) {
		subtitleLoaderThread.embed = embedLoaderThread.embed;
		vec2WordThread.embed = embedLoaderThread.embed;
		embedLoaderThread.loaded = false;
	}
	if (subtitleLoaderThread.loaded) {
		vec2WordThread.mapSubVector = subtitleLoaderThread.mapSubVector;
		subVector = subtitleLoaderThread.subVector;
		subtitleLoaderThread.loaded = false;
	}
	if (videoLoaderThread.loaded) {
		videoPlayerVector = videoLoaderThread.videoPlayerVector;
		videoLoaderThread.loaded = false;
	}
	if (!vec2WordThread.isThreadRunning() && vec2WordThread.loaded && vec2WordThread.start
	|| !vec2WordThread.isThreadRunning() && vec2WordThread.loaded && !subVector.empty() && subVector[numberOfVideoPlayer][(size_t)selectedSubtitle + numberOfSubtitles] -> getEndTime() + (static_cast<float>(subVector[numberOfVideoPlayer][(size_t)selectedSubtitle + numberOfSubtitles + 1] -> getStartTime()) - subVector[numberOfVideoPlayer][(size_t)selectedSubtitle + numberOfSubtitles] -> getEndTime()) / 2 < videoPlayerVector[numberOfVideoPlayer] -> getPosition() * videoPlayerVector[numberOfVideoPlayer] -> getDuration() * 1000
	|| !vec2WordThread.isThreadRunning() && vec2WordThread.loaded && !subVector.empty() && videoPlayerVector[numberOfVideoPlayer] -> getIsMovieDone()) {
		videoPlayerVector[numberOfVideoPlayer] -> stop();
		numberOfVideoPlayer = vec2WordThread.numberOfVideoPlayer;
		selectedSubtitle = vec2WordThread.selectedSubtitle;
		numberOfSubtitles = vec2WordThread.numberOfSubtitles;
		vec2WordThread.loaded = false;
		vec2WordThread.start = false;
		if (selectedSubtitle > 0) {
			float diff = (static_cast<float>(subVector[numberOfVideoPlayer][selectedSubtitle] -> getStartTime()) - subVector[numberOfVideoPlayer][(size_t)selectedSubtitle - 1] -> getEndTime()) / 2;
			videoPlayerVector[numberOfVideoPlayer] -> setPosition((subVector[numberOfVideoPlayer][(size_t)selectedSubtitle - 1] -> getEndTime() + diff) / videoPlayerVector[numberOfVideoPlayer] -> getDuration() / 1000);
		}
		else {
			videoPlayerVector[numberOfVideoPlayer] -> setPosition(0);
		}
		videoPlayerVector[numberOfVideoPlayer] -> play();
		videoPlayerVector[numberOfVideoPlayer] -> update();
		std::vector<std::string> words;
		if (subVector.size() > 0) {
			for (int i = 0; i <= numberOfSubtitles; i++) {
				if (!subVector[numberOfVideoPlayer][(size_t)selectedSubtitle + i]->getDialogue().empty()) {
					words.push_back(subVector[numberOfVideoPlayer][(size_t)selectedSubtitle + i]->getDialogue());
				}
			}
		}
		std::string joinedString = ofJoinString(words, " ");
		ofStringReplace(joinedString, "\n", " ");
		int size;
		if (!vec2WordThread.mapSubVectorCopy.empty()) {
			size = vec2WordThread.mapSubVectorCopy.size() - 1;
		}else{
			size = 0;
		}
		std::cout << "Subtitles left: " << size << ", Weight: " << vec2WordThread.weight << ", Possibilities: " << vec2WordThread.possibilities << ", Subtitles: " << selectedSubtitle << " - " << selectedSubtitle + numberOfSubtitles << ", Video player: " << numberOfVideoPlayer << ", Dialogue: " << joinedString << std::endl;
		vec2WordThread.startThread();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(100);
	ofDrawRectangle(10, 10, 290, 430);
	ofDrawRectangle(310, 10, 580, 430);
	ofSetColor(200);
	ofDrawRectangle(390, 50, 420, 320);
	ofSetColor(255, 200, 200);
	ofDrawBitmapString("Load embedding file", 30, 60);
	ofDrawBitmapString("Load subtitles", 30, 90);
	ofDrawBitmapString("Load videos", 30, 120);
	ofDrawBitmapString("Play", 30, 180);
	ofDrawBitmapString("Pause", 30, 210);
	ofDrawBitmapString("Random start", 30, 240);
	ofDrawBitmapString("Volume", 30, 270);
	ofDrawBitmapString("Custom words", 30, 320);
	ofDrawBitmapString("Choose custom words", 30, 350);
	ofDrawBitmapString("Read me", 30, 400);
	for (auto &element : groupOfBangs) {
		element.draw();
	}
	for (auto &element : groupOfToggles) {
		element.draw();
	}
	hSlider_1.draw();
	if (!subVector.empty()) {
		for (int i = selectedSubtitle; i <= selectedSubtitle + numberOfSubtitles; i++) {
			if (subVector[numberOfVideoPlayer][i] -> getStartTime() <= videoPlayerVector[numberOfVideoPlayer] -> getPosition() * videoPlayerVector[numberOfVideoPlayer] -> getDuration() * 1000 - 50 && subVector[numberOfVideoPlayer][i] -> getEndTime() >= videoPlayerVector[numberOfVideoPlayer] -> getPosition() * videoPlayerVector[numberOfVideoPlayer] -> getDuration() * 1000 - 50) {
				ofDrawBitmapString(subVector[numberOfVideoPlayer][i] -> getDialogue(), 400, 395);
			}
		}
	}
	ofSetColor(255);
	if (videoPlayerVector[numberOfVideoPlayer] -> getTexture().isAllocated()) {
		videoPlayerVector[numberOfVideoPlayer] -> draw(400, 60 + 150 - 200 * (videoPlayerVector[numberOfVideoPlayer] -> getHeight() / videoPlayerVector[numberOfVideoPlayer] -> getWidth()), 400, 400 * (videoPlayerVector[numberOfVideoPlayer] -> getHeight() / videoPlayerVector[numberOfVideoPlayer] -> getWidth()));
	}
	if (bReadMe) {
		ofSetColor(100);
		ofDrawRectangle(310, 10, 580, 430);
		ofSetColor(255, 200, 200);
		ofDrawBitmapString("1. Load an embedding file, for example:\n\nhttps://github.com/eyaler/word2vec-slim/\n\n2. Load .srt subtitle files\n\n3. Load the corresponding video files\n\n- \"Play\" also reloads the used subtitles\n\n- \"Random start\" only works, if \"Custom words\" is deselected", 370, 90);
	}
}