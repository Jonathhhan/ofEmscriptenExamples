#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool& e) {
	ofDirectory dir("embedding file");
	dir.allowExt("bin");
	dir.listDir();
	dir.sort();
	std::cout << "Loading embeddings file: " << dir.getPath(0) << std::endl;
	thread.embed.load_binary(dir.getPath(0), false);
	std::cout << "Words in " << dir.getPath(0) << ": " << thread.embed.words << std::endl;
	std::cout << "Dimensions in " << dir.getPath(0) << ": " << thread.embed.size << std::endl;
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool& e) {
	if (thread.embed.words > 0) {
		int counter = 0;
		std::vector<std::string> currentWords;
		ofDirectory dir("subtitles");
		dir.allowExt("srt");
		dir.listDir();
		dir.sort();
		subVector.clear();
		thread.mapSubVector.clear();
		thread.mapSubVectorCopy.clear();
		videoPlayerVector[numberOfVideoPlayer] -> stop();
		for (int i = 0; i < dir.size(); i++) {
			SubtitleParserFactory* subParserFactory = new SubtitleParserFactory("data/" + dir.getPath(i));
			SubtitleParser* parser = subParserFactory -> getParser();
			sub = parser -> getSubtitles();
			subVector.push_back(sub);
			for (auto element : sub) {
				std::string lowerString = ofToLower(element -> getDialogue());
				ofStringReplace(lowerString, "'", " ");
				ofStringReplace(lowerString, "-", " ");
				char chars[] = "0123456789.,!:?;()\"";
				for (auto element : chars) {
					ofStringReplace(lowerString, ofToString(element), "");
				}
				std::vector<std::string> splitWords = ofSplitString(lowerString, " ");
				for (auto& element : splitWords) {
					if (thread.embed.find_case_sensitive(element) != - 1) {
						currentWords.push_back(element);
					}
				}
				for (auto &element : stopWords) {
					currentWords.erase(std::remove(currentWords.begin(), currentWords.end(), element), currentWords.end());
				}
				if (element -> getDialogue().back() == '.' || element -> getDialogue().back() == '?' || element -> getDialogue().back() == '!' || element -> getDialogue().back() == '"' || element -> getDialogue().back() == '\'' || element -> getDialogue().back() == ';') {
					std::string currentDialogue = ofJoinString(currentWords, " ");
					currentWords.clear();
					if (!currentDialogue.empty()) {
						thread.mapSubVector.emplace(std::make_pair(i, element->getSubNo() - counter - 1), std::make_pair(thread.embed.words_to_vec(currentDialogue), counter));
					}
					counter = - 1;
				}
				counter ++;
			}		
			std::cout << "Subtitles: " << dir.getPath(i) << ", Subtitle size: " << sub.size() << ", Subtitle number: " << i << std::endl;
		}
	} else {
		std::cout << "Please load an embedding file first!" << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool& e) {
	for (auto element : videoPlayerVector) {
		element -> close();
	}
	videoPlayerVector.clear();
	ofDirectory dir("videos");
	dir.allowExt("3gp");
	dir.allowExt("avi");
	dir.allowExt("divx");
	dir.allowExt("dv");
	dir.allowExt("h264");
	dir.allowExt("mkv");
	dir.allowExt("mp4");
	dir.allowExt("mpeg");
	dir.allowExt("mpg");
	dir.allowExt("mov");
	dir.allowExt("mxf");
	dir.allowExt("ogg");
	dir.allowExt("ogm");
	dir.allowExt("ogv");
	dir.allowExt("omf");
	dir.allowExt("qt");
	dir.allowExt("webm");
	dir.allowExt("wmv");
	dir.listDir();
	dir.sort();
	ofVideoPlayer* movie;
	for (int i = 0; i < dir.size(); i++) {
		movie = new ofVideoPlayer();
		movie -> load(dir.getPath(i));
		movie -> setLoopState(OF_LOOP_NONE);
		videoPlayerVector.push_back(movie);
		std::cout << "Video: " << dir.getPath(i) << ", Video number: " << i << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::bang_5onMousePressed(bool& e) {
	std::string string = ofSystemTextBoxDialog("Custom words:");
	std::vector<std::string> joinedWords;
	std::string lowerString = ofToLower(string);
	ofStringReplace(lowerString, ",", "");
	std::vector<std::string> splitWords = ofSplitString(lowerString, " ");
	for (auto &element : splitWords) {
		if (thread.embed.find_case_sensitive(element) == - 1 && !element.empty()) {
			std::cout << "Word \"" << element << "\" does not exist! Choose another word or load an embedding file." << std::endl;
		} else {
			joinedWords.push_back(element);
		}
	}
	if (!joinedWords.empty()) {
		thread.customWords = ofJoinString(joinedWords, " ");
		std::cout << "Custom words: " << thread.customWords << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::bang_4onMousePressed(bool& e) {
	if (videoPlayerVector[numberOfVideoPlayer] -> isLoaded()) {
		videoPlayerVector[numberOfVideoPlayer] -> stop();
		thread.mapSubVectorCopy = thread.mapSubVector;
		if ((subVector.size() > 0 && !thread.bCustomWords) || (subVector.size() > 0 && thread.customWords.empty())) {
			if (bRandomStart) {
				thread.possibilities = thread.mapSubVectorCopy.size();
				auto it = thread.mapSubVectorCopy.begin();
				ofSeedRandom();
				std::advance(it, ofRandom(thread.possibilities));
				thread.numberOfVideoPlayer = it -> first.first;
				thread.selectedSubtitle = it -> first.second;
				thread.numberOfSubtitles = it -> second.second;
			}
			else {
				thread.possibilities = 1;
				thread.numberOfVideoPlayer = 0;
				thread.selectedSubtitle = 0;
				thread.numberOfSubtitles = thread.mapSubVectorCopy[{0, 0}].second;
			}
			thread.weight = 0;
		}
		else if (subVector.size() > 0) {
			thread.multimapWeightSub.clear();
			for (auto &element : thread.mapSubVectorCopy) {
				thread.multimapWeightSub.emplace(thread.embed.words_to_vec(thread.customWords).dist_cosine_optimized(get<0>(element.second)), std::make_tuple(element.first.first, element.first.second, element.second.second));
			}
			auto it = thread.multimapWeightSub.rbegin();
			thread.weight = it -> first;
			if (it -> first != 0) {
				thread.choosenSubs.clear();
				auto range = thread.multimapWeightSub.equal_range(it -> first);
				for (auto it = range.first; it != range.second; ++it) {
					thread.choosenSubs.push_back(std::make_tuple(get<0>(it -> second), get<1>(it -> second), get<2>(it -> second)));
				}
				thread.possibilities = thread.choosenSubs.size();
				ofSeedRandom();
				int random = ofRandom(thread.possibilities);
				thread.numberOfVideoPlayer = get<0>(thread.choosenSubs[random]);
				thread.selectedSubtitle = get<1>(thread.choosenSubs[random]);
				thread.numberOfSubtitles = get<2>(thread.choosenSubs[random]);
			}
			else {
				thread.possibilities = thread.mapSubVectorCopy.size();
				auto it = thread.mapSubVectorCopy.begin();
				ofSeedRandom();
				std::advance(it, ofRandom(thread.possibilities));
				thread.numberOfVideoPlayer = it -> first.first;
				thread.selectedSubtitle = it -> first.second;
				thread.numberOfSubtitles = it -> second.second;
			}
		}
		if (thread.selectedSubtitle > 0) {
			if (thread.selectedSubtitle > 0) {
				float diff = (static_cast<float>(subVector[thread.numberOfVideoPlayer][thread.selectedSubtitle] -> getStartTime()) - subVector[thread.numberOfVideoPlayer][(size_t)thread.selectedSubtitle - 1] -> getEndTime()) / 2;
				videoPlayerVector[thread.numberOfVideoPlayer] -> setPosition((subVector[thread.numberOfVideoPlayer][(size_t)thread.selectedSubtitle - 1] -> getEndTime() + diff) / videoPlayerVector[thread.numberOfVideoPlayer] -> getDuration() / 1000);
			}
			else {
				videoPlayerVector[thread.numberOfVideoPlayer] -> setPosition(0);
			}
			videoPlayerVector[thread.numberOfVideoPlayer] -> play();
			videoPlayerVector[thread.numberOfVideoPlayer] -> update();
		}
		std::vector<std::string> words;
		for (int i = 0; i <= thread.numberOfSubtitles; i++) {
			if (!subVector[thread.numberOfVideoPlayer][(size_t)thread.selectedSubtitle + i] -> getDialogue().empty()) {
				words.push_back(subVector[thread.numberOfVideoPlayer][(size_t)thread.selectedSubtitle + i] -> getDialogue());
			}
		}
		std::string joinedString = ofJoinString(words, " ");
		ofStringReplace(joinedString, "\n", " ");
		std::cout << "Subtitles left: " << thread.mapSubVectorCopy.size() - 1 << ", Weight: " << thread.weight << ", Possibilities: " << thread.possibilities << ", Subtitles: " << thread.selectedSubtitle << " - " << thread.selectedSubtitle + thread.numberOfSubtitles << ", Video player: " << thread.numberOfVideoPlayer << ", Dialogue: " << joinedString << std::endl;
		groupOfToggles[0].value = 0;
		numberOfVideoPlayer = thread.numberOfVideoPlayer;
		selectedSubtitle = thread.selectedSubtitle;
		numberOfSubtitles = thread.numberOfSubtitles;
		thread.startThread();
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
	bRandomStart = e;
}

//--------------------------------------------------------------
void ofApp::toggle_3onMousePressed(bool& e) {
	thread.bCustomWords = e;
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
	if ((!thread.isThreadRunning() && !thread.mapSubVectorCopy.empty() && subVector[numberOfVideoPlayer][(size_t)selectedSubtitle + numberOfSubtitles] -> getEndTime() + (static_cast<float>(subVector[numberOfVideoPlayer][(size_t)selectedSubtitle + numberOfSubtitles + 1] -> getStartTime()) - subVector[numberOfVideoPlayer][(size_t)selectedSubtitle + numberOfSubtitles] -> getEndTime()) / 2 < videoPlayerVector[numberOfVideoPlayer] -> getPosition() * videoPlayerVector[numberOfVideoPlayer] -> getDuration() * 1000)
	|| (!thread.isThreadRunning() && !thread.mapSubVectorCopy.empty() && videoPlayerVector[numberOfVideoPlayer] -> getIsMovieDone())) {
		videoPlayerVector[numberOfVideoPlayer]->stop();
		if (thread.selectedSubtitle > 0) {
			float diff = (static_cast<float>(subVector[thread.numberOfVideoPlayer][thread.selectedSubtitle] -> getStartTime()) - subVector[thread.numberOfVideoPlayer][(size_t)thread.selectedSubtitle - 1] -> getEndTime()) / 2;
			videoPlayerVector[thread.numberOfVideoPlayer] -> setPosition((subVector[thread.numberOfVideoPlayer][(size_t)thread.selectedSubtitle - 1] -> getEndTime() + diff) / videoPlayerVector[thread.numberOfVideoPlayer] -> getDuration() / 1000);
		}
		else {
			videoPlayerVector[thread.numberOfVideoPlayer] -> setPosition(0);
		}
		videoPlayerVector[thread.numberOfVideoPlayer] -> play();
		videoPlayerVector[thread.numberOfVideoPlayer] -> update();
		std::vector<std::string> words;
		for (int i = 0; i <= thread.numberOfSubtitles; i++) {
			if (!subVector[thread.numberOfVideoPlayer][(size_t)thread.selectedSubtitle + i] -> getDialogue().empty()) {
				words.push_back(subVector[thread.numberOfVideoPlayer][(size_t)thread.selectedSubtitle + i] -> getDialogue());
			}
		}
		std::string joinedString = ofJoinString(words, " ");
		ofStringReplace(joinedString, "\n", " ");
		std::cout << "Subtitles left: " << thread.mapSubVectorCopy.size() - 1 << ", Weight: " << thread.weight << ", Possibilities: " << thread.possibilities << ", Subtitles: " << thread.selectedSubtitle << " - " << thread.selectedSubtitle + thread.numberOfSubtitles << ", Video player: " << thread.numberOfVideoPlayer << ", Dialogue: " << joinedString << std::endl;
		numberOfVideoPlayer = thread.numberOfVideoPlayer;
		selectedSubtitle = thread.selectedSubtitle;
		numberOfSubtitles = thread.numberOfSubtitles;
		thread.startThread();
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
	if (!sub.empty()) {
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