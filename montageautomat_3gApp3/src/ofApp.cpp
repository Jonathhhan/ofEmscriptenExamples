#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool& e) {
	std::string file = "embedding file/GoogleNews-vectors-negative300-SLIM.bin";
	std::cout << "Loading embeddings file: " << file << std::endl;
	embed.load_binary(file, false);
	std::cout << "Words in " << file << ": " << embed.words << std::endl;
	std::cout << "Dimensions in " << file << ": " << embed.size << std::endl;
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool& e) {
	int counter = - 1;
	selectedSubtitle = 0;
	std::vector<std::string> currentWords;
	videoPlayerVector[numberOfVideoPlayer]->setPaused(true);
	subVector.clear();
	mapSubVector.clear();
	mapSubVectorCopy.clear();
	ofDirectory dir("subtitles");
	dir.allowExt("srt");
	dir.listDir();
	dir.sort();
	for (int i = 0; i < dir.size(); i++) {
		SubtitleParserFactory* subParserFactory = new SubtitleParserFactory("data/" + dir.getPath(i));
		SubtitleParser* parser = subParserFactory -> getParser();
		sub = parser -> getSubtitles();
		subVector.push_back(sub);
		for (auto element : sub) {
			counter++;
			std::string lowerString = ofToLower(element -> getDialogue());
			if (!lowerString.empty()) {
				ofStringReplace(lowerString, "'", " ");
				ofStringReplace(lowerString, "-", " ");
				char chars[] = "0123456789.,!:?;()\"";
				for (int j = 0; j < strlen(chars); ++j) {
					ofStringReplace(lowerString, ofToString(chars[j]), "");
				}
				std::vector<std::string> splitWords = ofSplitString(lowerString, " ");
				for (auto &element : splitWords) {
					if (embed.find_case_sensitive(element) != -1 && currentWords.size() <= 20) {
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
						mapSubVector[{i, element -> getSubNo() - counter - 1}] = std::make_pair(embed.words_to_vec(currentDialogue), counter);
					}
					counter = - 1;
				}
			}
		}
		std::cout << "Subtitles: " << dir.getPath(i) << ", Subtitle size: " << sub.size() << ", Subtitle number: " << i << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool& e) {
	videoPlayerVector.clear();
	ofDirectory dir("videos");
	dir.allowExt("mp4");
	dir.listDir();
	dir.sort();
	ofVideoPlayer* movie;
	for (int i = 0; i < dir.size(); i++) {
		movie = new ofVideoPlayer();
		movie -> load(dir.getPath(i));
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
		if (embed.find_case_sensitive(element) == - 1 && !element.empty()) {
			std::cout << "Word \"" << element << "\" does not exist! Choose another word or load an embedding file." << std::endl;
		}
		else if (!element.empty()) {
			joinedWords.push_back(element);
		}
	}
	if (!joinedWords.empty()) {
		customWords = ofJoinString(joinedWords, " ");
	}
}

//--------------------------------------------------------------
void ofApp::bang_4onMousePressed(bool& e) {
	if (videoPlayerVector[numberOfVideoPlayer] -> isLoaded()) {
		videoPlayerVector[numberOfVideoPlayer] -> setPaused(true);
		mapSubVectorCopy = mapSubVector;
		if ((subVector.size() > 0 && !bCustomWords) || (subVector.size() > 0 && customWords.empty())) {
			if (bRandomStart) {
				auto it = mapSubVectorCopy.begin();
				std::advance(it, rand() % mapSubVectorCopy.size());
				numberOfVideoPlayer = it -> first.first;
				selectedSubtitle = it -> first.second;
				numberOfSubtitles = it -> second.second;
			}
			else {
				numberOfVideoPlayer = 0;
				selectedSubtitle = 0;
				numberOfSubtitles = mapSubVectorCopy[{0, 0}].second;
			}
			weight = 0;
		}
		else if (subVector.size() > 0) {
			std::multimap<double, std::tuple<int, int, int>> multimapWeightSub;
			for (auto &element : mapSubVectorCopy) {
				multimapWeightSub.insert(std::make_pair(embed.words_to_vec(customWords).dist_cosine_optimized(get<0>(element.second)), std::make_tuple(element.first.first, element.first.second, element.second.second)));
			}
			auto it = multimapWeightSub.rbegin();
			weight = it -> first;
			if (it -> first != 0) {
				std::vector<std::tuple<int, int, int>> choosenSubs;
				auto range = multimapWeightSub.equal_range(it -> first);
				for (auto it = range.first; it != range.second; ++it) {
					choosenSubs.push_back(std::make_tuple(get<0>(it -> second), get<1>(it -> second), get<2>(it -> second)));
				}
				int random = rand() % choosenSubs.size();
				numberOfVideoPlayer = get<0>(choosenSubs[random]);
				selectedSubtitle = get<1>(choosenSubs[random]);
				numberOfSubtitles = get<2>(choosenSubs[random]);
			}
			else {
				auto it = mapSubVectorCopy.begin();
				std::advance(it, rand() % mapSubVectorCopy.size());
				numberOfVideoPlayer = it -> first.first;
				selectedSubtitle = it -> first.second;
				numberOfSubtitles = it -> second.second;
			}
		}
		if (selectedSubtitle > 0) {
			int subtitle = selectedSubtitle - 1;
			videoPlayerVector[numberOfVideoPlayer] -> setPosition((subVector[numberOfVideoPlayer][subtitle] -> getEndTime() + 50) / videoPlayerVector[numberOfVideoPlayer] -> getDuration() / 1000);
		}
		else {
			videoPlayerVector[numberOfVideoPlayer] -> setPosition(0);
		}
		std::vector<std::string> words;
		for (int i = 0; i <= numberOfSubtitles; i++) {
			int subtitle = selectedSubtitle + i;
			if (!subVector[numberOfVideoPlayer][subtitle] -> getDialogue().empty()) {
				words.push_back(subVector[numberOfVideoPlayer][subtitle] -> getDialogue());
			}
		}
		std::string joinedString = ofJoinString(words, " ");
		ofStringReplace(joinedString, "\n", " ");
		std::cout << "Subtitles left: " << mapSubVectorCopy.size() - 1 << ", Weight: " << weight << ", Subtitles: " << selectedSubtitle << " - " << selectedSubtitle + numberOfSubtitles << ", Video player: " << numberOfVideoPlayer << ", Dialogue: " << joinedString << std::endl;
		groupOfToggles[0].value = 0;
		videoPlayerVector[numberOfVideoPlayer] -> play();
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
	bCustomWords = e;
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
	selectedSubtitle = 0;
	numberOfSubtitles = 0;
	numberOfVideoPlayer = 0;
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

	stopWords = { "i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now", "d", "do", "m", "re", "ll", "didn", "doesn", "hasn", "hadn", "cannot", "mustn", "isn", "wasn", "couldn", "wouldn", "mr", "ve", "l", "y", "to", "o", "m", "lsn", "from", "out" };
}

//--------------------------------------------------------------
void ofApp::update() {
	videoPlayerVector[numberOfVideoPlayer] -> update();
	int subtitle = selectedSubtitle + numberOfSubtitles;
	if (!mapSubVectorCopy.empty() && subVector[numberOfVideoPlayer][subtitle] -> getEndTime() + 50 < videoPlayerVector[numberOfVideoPlayer] -> getPosition() * videoPlayerVector[numberOfVideoPlayer] -> getDuration() * 1000 && mapSubVectorCopy.size() > 0) {
		videoPlayerVector[numberOfVideoPlayer] -> setPaused(true);
		std::multimap<double, std::tuple<int, int, int>> multimapWeightSub;
		for (auto &element : mapSubVectorCopy) {
			if (element.first.first != numberOfVideoPlayer || element.first.second != selectedSubtitle) {
				if (!bCustomWords || customWords.empty()) {
					multimapWeightSub.insert(std::make_pair(mapSubVectorCopy[{numberOfVideoPlayer, selectedSubtitle}].first.dist_cosine_optimized(element.second.first), std::make_tuple(element.first.first, element.first.second, element.second.second)));
				}
				else {
					multimapWeightSub.insert(std::make_pair(embed.words_to_vec(customWords).dist_cosine_optimized(element.second.first), std::make_tuple(element.first.first, element.first.second, element.second.second)));
				}
			}
		}
		mapSubVectorCopy.erase({numberOfVideoPlayer, selectedSubtitle});
		if (mapSubVectorCopy.size() > 0) {
			auto it = multimapWeightSub.rbegin();
			weight = it -> first;
			if (it -> first != 0) {
				std::vector<std::tuple<int, int, int>> choosenSubs;
				auto range = multimapWeightSub.equal_range(it -> first);
				for (auto it = range.first; it != range.second; ++it) {
					choosenSubs.push_back(std::make_tuple(get<0>(it -> second), get<1>(it -> second), get<2>(it -> second)));
				}
				int random = rand() % choosenSubs.size();
				numberOfVideoPlayer = get<0>(choosenSubs[random]);
				selectedSubtitle = get<1>(choosenSubs[random]);
				numberOfSubtitles = get<2>(choosenSubs[random]);
			}
			else {
				auto it = mapSubVectorCopy.begin();
				std::advance(it, rand() % mapSubVectorCopy.size());
				numberOfVideoPlayer = it -> first.first;
				selectedSubtitle = it -> first.second;
				numberOfSubtitles = it -> second.second;
			}
		}
		else {
			mapSubVectorCopy = mapSubVector;
			auto it = mapSubVectorCopy.begin();
			std::advance(it, rand() % mapSubVectorCopy.size());
			numberOfVideoPlayer = it -> first.first;
			selectedSubtitle = it -> first.second;
			numberOfSubtitles = it -> second.second;
			weight = 0;
		}
		if (selectedSubtitle > 0) {
			int subtitle = selectedSubtitle - 1;
			videoPlayerVector[numberOfVideoPlayer] -> setPosition((subVector[numberOfVideoPlayer][subtitle] -> getEndTime() + 50) / videoPlayerVector[numberOfVideoPlayer] -> getDuration() / 1000);
		}
		else {
			videoPlayerVector[numberOfVideoPlayer] -> setPosition(0);
		}
		videoPlayerVector[numberOfVideoPlayer] -> play();
		std::vector<std::string> words;
		for (int i = 0; i <= numberOfSubtitles; i++) {
			int subtitle = selectedSubtitle + i;
			if (!subVector[numberOfVideoPlayer][subtitle] -> getDialogue().empty()) {
				words.push_back(subVector[numberOfVideoPlayer][subtitle] -> getDialogue());
			}
		}
		std::string joinedString = ofJoinString(words, " ");
		ofStringReplace(joinedString, "\n", " ");
		std::cout << "Subtitles left: " << mapSubVectorCopy.size() - 1 << ", Weight: " << weight << ", Subtitles: " << selectedSubtitle << " - " << selectedSubtitle + numberOfSubtitles << ", Video player: " << numberOfVideoPlayer << ", Dialogue: " << joinedString << std::endl;
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
