#include "ofApp.h"

#include "emscripten.h"
#include "emscripten/bind.h"

//--------------------------------------------------------------
ofEvent<std::string &> loadBinEvent;
ofEvent<std::string &> loadSubtitleEvent;
ofEvent<std::string &> loadVideoEvent;
ofEvent<std::string &> loadCustomWordsEvent;

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e) {
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.bin';
	input.onchange = function(e) {

		// getting a hold of the file reference
		var file = e.target.files[0]; 

		// setting up the reader
		var reader = new FileReader();
		reader.readAsArrayBuffer(file);

		// here we tell the reader what to do when it's done reading...
		reader.onload = function() {
			var arrayBuffer = reader.result;
			var uint8View = new Uint8Array(arrayBuffer);	
			FS.createDataFile("/data/", file.name, uint8View, true, true);
			FS.syncfs(true, function (err) {
				Module.loadBin(file.name);
				assert(!err);
        		});	
		}
	};
	input.click();
	);
}

void loadBin(std::string file) {        
	loadBinEvent.notify(file); 
}

void ofApp::loadBinX(std::string & file) {
	std::cout << "Loading embeddings file: " << file << std::endl;
	embed.load_binary(file, false);
	std::cout << "Words in " << file << ": " << embed.words << std::endl;
	std::cout << "Dimensions in " << file << ": " <<  embed.size << std::endl;
	EM_ASM(FS.unlink("/data/" + UTF8ToString($0)), file.c_str());
}

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e) {
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.srt';
	input.onchange = function(e) {

		// getting a hold of the file reference
		var file = e.target.files[0]; 

		// setting up the reader
		var reader = new FileReader();
		reader.readAsArrayBuffer(file);

		// here we tell the reader what to do when it's done reading...
		reader.onload = function() {
			var arrayBuffer = reader.result;
			var uint8View = new Uint8Array(arrayBuffer);	
			FS.createDataFile("/data/", file.name, uint8View, true, true);
			FS.syncfs(true, function (err) {
				Module.loadSubtitle(file.name);
				assert(!err);
        		});	
		}
	};
	input.click();
	);
}

void loadSubtitle(std::string file) {        
	loadSubtitleEvent.notify(file); 
}

void ofApp::loadSubtitleX(std::string & file) {
	videoPlayer.setPaused(true);
	selectSubtitle = 0;
	mapSubVector.clear();
	mapSubVectorCopy.clear();
	subParserFactory = new SubtitleParserFactory(ofToDataPath(file));
	parser = subParserFactory->getParser();

	// to get subtitles
	sub = parser -> getSubtitles();
	EM_ASM(FS.unlink("/data/" + UTF8ToString($0)), file.c_str());
	for (auto sub_element : sub) {
		
		// process subtitles
		counter++;
		for (std::string element : sub_element -> getIndividualWords()) {
			ofToLower(element);	
			ofStringReplace(element, "'", " ");
			ofStringReplace(element, "-", " ");
			char chars[] = "0123456789.,!:?;()\"";
			for (int i = 0; i < strlen(chars); ++i) {
				ofStringReplace(element, ofToString(chars[i]), "");
			}
			splitWords = ofSplitString(element, " ");
			for (std::string element : splitWords) {
				if (embed.find_case_sensitive(element) != -1) { 	
					currentWords.push_back(element);
				}
			}
			for (std::string element : stopWords) {
				currentWords.erase(std::remove(currentWords.begin(), currentWords.end(), element), currentWords.end());
			}	
		}
		if (sub_element -> getDialogue().back() == '.' || sub_element -> getDialogue().back() == '?' || sub_element -> getDialogue().back() == '!' || sub_element -> getDialogue().back() == '"') {
			currentDialogue = ofJoinString(currentWords, " ");
			currentWords.clear();
			if (!currentDialogue.empty()) {
				mapSubVector[sub_element -> getSubNo() - counter - 1] = std::make_tuple(embed.words_to_vec(currentDialogue, &used_indices), counter);
			}
			counter = - 1;
		}
	}
	std::cout << "Subtitles: " << sub.size() << std::endl;
}

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool & e) {
	EM_ASM(
	var input = document.createElement('input');
	input.type = 'file';
	input.accept = '.3gp, .h264, .mov, .mp4, .mpeg, .ogg, .webm';
	input.onchange = function(e) {
		var file = e.target.files[0]; 
		var url = URL.createObjectURL(file);
		Module.loadVideo(url);
	};
	input.click();
	);
}

void loadVideo(std::string url) {
	loadVideoEvent.notify(url); 
}

void ofApp::loadVideoX(std::string & url) {
	videoPlayer.load(url);
	
}

//--------------------------------------------------------------
void ofApp::bang_5onMousePressed(bool & e) {
	EM_ASM(
	var customWords = prompt("Please enter custom words:");
	if (customWords != null) {
		Module.loadCustomWords(customWords);
	}
	);
}

void loadCustomWords(std::string string) {
	loadCustomWordsEvent.notify(string); 
}

void ofApp::loadCustomWordsX(std::string & string) {
	joinedWords.clear();
	ofToLower(string);
	ofStringReplace(string, ",", "");
	splitWords = ofSplitString(string, " ");
	for (std::string element : splitWords) {
		if (embed.find_case_sensitive(element) == -1 && !element.empty()) { 
			std::cout << "Word \"" << element << "\" does not exist! Choose another word." << std::endl;
		} else if (!element.empty()) {
			joinedWords.push_back(element);
		}
	}
	if (!joinedWords.empty()) {
		customWords = ofJoinString(joinedWords, " ");
	}
}

//--------------------------------------------------------------
EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("loadBin", &loadBin);
	emscripten::function("loadSubtitle", &loadSubtitle);
	emscripten::function("loadVideo", &loadVideo);
	emscripten::function("loadCustomWords", & loadCustomWords);
}

//--------------------------------------------------------------
void ofApp::bang_4onMousePressed(bool & e) {
	if (videoPlayer.isLoaded()) {
		mapSubVectorCopy = mapSubVector;
		if ((sub.size() > 0 && !bCustomWords) || (sub.size() > 0 && customWords.empty())) {
			if (randomStart) {
				auto it = mapSubVectorCopy.begin();
				std::advance(it, rand() % mapSubVectorCopy.size());
				selectSubtitle = it -> first;
				counter2 = get<1>(it -> second);
			} else {
				selectSubtitle = 0;
				counter2 = get<1>(mapSubVectorCopy[0]);
			}
			weight = 0;
		} else if (sub.size() > 0) {
			multimapWeightSub.clear();
		
			// get vector similarities	
			for (auto element : mapSubVectorCopy) {
				multimapWeightSub.insert(std::make_pair(embed.words_to_vec(customWords, &used_indices).dist_cosine_optimized(get<0>(element.second)), std::make_tuple(element.first, get<1>(element.second))));
			}
			
			// choose a random subtitle with highest key
			auto it = multimapWeightSub.rbegin();
			weight = it -> first;
			if (it -> first != 0) {
				choosenSubs.clear();
				choosenLength.clear();
				auto range = multimapWeightSub.equal_range(it -> first);
				for (auto it = range.first; it != range.second; ++it) {
					choosenSubs.push_back(get<0>(it -> second));
					choosenLength.push_back(get<1>(it -> second));
				}
				int random = rand() % choosenSubs.size();
				selectSubtitle = choosenSubs[random];
				counter2 = choosenLength[random];
			} else {
				auto it = mapSubVectorCopy.begin();
				std::advance(it, rand() % mapSubVectorCopy.size());
				selectSubtitle = it -> first;
				counter2 = get<1>(it -> second);
			}
		}
		if (selectSubtitle > 0) {
			videoPlayer.setPosition((sub[selectSubtitle -1] -> getEndTime() + 50) / videoPlayer.getDuration() / 1000);
		} else {
			videoPlayer.setPosition(0);
		}
		videoPlayer.play();
		groupOfToggles[0].value = 0;
		words.clear();
		for (int i = 0; i <= counter2; i++) {
			words.push_back(sub[selectSubtitle + i] -> getDialogue());
		}
		std::cout << "Subtitles left: " << mapSubVectorCopy.size() << ", Weight: " << weight << ", Subtitles: " << selectSubtitle << " - " << selectSubtitle + counter2 << ", Dialogue: " << ofJoinString(words, " ") << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::toggle_1onMousePressed(bool & e) {
	if (e && videoPlayer.isLoaded()) {
		videoPlayer.setPaused(true);
	} else {
		videoPlayer.setPaused(false);	
	}
}

//--------------------------------------------------------------
void ofApp::toggle_2onMousePressed(bool & e) {
	randomStart = e;
}

//--------------------------------------------------------------
void ofApp::toggle_3onMousePressed(bool & e) {
	bCustomWords = e;
}

//--------------------------------------------------------------
void ofApp::toggle_4onMousePressed(bool & e) {
	bReadMe = e;
}

//--------------------------------------------------------------
void ofApp::hSlider_1onMousePressed(float & e) {
	videoPlayer.setVolume(e);
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
	ofAddListener(loadBinEvent, this, & ofApp::loadBinX);
	ofAddListener(loadSubtitleEvent, this, & ofApp::loadSubtitleX);
	ofAddListener(loadVideoEvent, this, & ofApp::loadVideoX);
	ofAddListener(loadCustomWordsEvent, this, & ofApp::loadCustomWordsX);
	
	videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
	videoPlayer.setUsePixels(false);
	ofSetBackgroundColor(200);
	title = "Montageautomat 3";
	counter = - 1;

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
	
	// exclude those words
	stopWords = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now", "d", "do", "m", "re", "ll", "didn", "doesn", "hasn", "hadn", "cannot", "mustn", "isn", "wasn", "couldn", "wouldn", "mr", "ve", "l", "y", "to", "o", "m", "lsn", "from", "out"};
}

//--------------------------------------------------------------
void ofApp::update() {
	videoPlayer.update();
	if (sub[selectSubtitle + counter2] -> getEndTime() + 50 < videoPlayer.getPosition() * 1000 && mapSubVectorCopy.size() > 0) {
		multimapWeightSub.clear();

		// get vector similarities	
		for (auto element : mapSubVectorCopy) {
			if (!get<0>(element.second).empty() && element.first != selectSubtitle) {
				if (!bCustomWords || customWords.empty()) {
					multimapWeightSub.insert(std::make_pair(get<0>(mapSubVectorCopy[selectSubtitle]).dist_cosine_optimized(get<0>(element.second)), std::make_tuple(element.first, get<1>(element.second))));
				} else {
					multimapWeightSub.insert(std::make_pair(embed.words_to_vec(customWords, &used_indices).dist_cosine_optimized(get<0>(element.second)), std::make_tuple(element.first, get<1>(element.second))));
				}
			}
		}
		mapSubVectorCopy.erase(selectSubtitle);
		
		// choose a random subtitle with highest key
		if (mapSubVectorCopy.size() > 0) {
			auto it = multimapWeightSub.rbegin();
			weight = it -> first;
			if (it -> first != 0) {
				choosenSubs.clear();
				choosenLength.clear();
				auto range = multimapWeightSub.equal_range(it -> first);
				for (auto it = range.first; it != range.second; ++it) {
					choosenSubs.push_back(get<0>(it -> second));
					choosenLength.push_back(get<1>(it -> second));
				}
				int random = rand() % choosenSubs.size();
				selectSubtitle = choosenSubs[random];
				counter2 = choosenLength[random];
			} else {
				auto it = mapSubVectorCopy.begin();
				std::advance(it, rand() % mapSubVectorCopy.size());
				selectSubtitle = it -> first;
				counter2 = get<1>(it -> second);
			}
		} else {		
			mapSubVectorCopy = mapSubVector;
			auto it = mapSubVectorCopy.begin();
			std::advance(it, rand() % mapSubVectorCopy.size());
			selectSubtitle = it -> first;
			counter2 = get<1>(it -> second);
			weight = 0;
		}
		
		// set video position and subtitle
		videoPlayer.setPosition((sub[selectSubtitle -1] -> getEndTime() + 50) / videoPlayer.getDuration() / 1000);
		words.clear();
		for (int i = 0; i <= counter2; i++) {
			words.push_back(sub[selectSubtitle + i] -> getDialogue());
		}
		std::cout << "Subtitles left: " << mapSubVectorCopy.size() << ", Weight: " << weight << ", Subtitles: " << selectSubtitle << " - " << selectSubtitle + counter2 << ", Dialogue: " << ofJoinString(words, " ") << std::endl;
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
	ofDrawBitmapString("Load subtitle", 30, 90);
	ofDrawBitmapString("Load movie", 30, 120);
	ofDrawBitmapString("Play", 30, 180);
	ofDrawBitmapString("Pause", 30, 210);
	ofDrawBitmapString("Random start", 30, 240);
	ofDrawBitmapString("Volume", 30, 270);
	ofDrawBitmapString("Custom words", 30, 320);
	ofDrawBitmapString("Choose custom words", 30, 350);
	ofDrawBitmapString("Read me", 30, 400);
	for(int i=0; i<NBANGS; i++) {
		groupOfBangs[i].draw();
	}
	for(int i=0; i<NTOGGLES; i++) {
		groupOfToggles[i].draw();
	}
	hSlider_1.draw();
	for (int i = selectSubtitle; i <= selectSubtitle + counter2; i++) {
		if (sub[i] -> getStartTime() <= videoPlayer.getPosition() * 1000 - 50 && sub[i] -> getEndTime() >= videoPlayer.getPosition() * 1000 - 50) {
			ofDrawBitmapString(sub[i] -> getDialogue(), 600 - (sub[i] -> getDialogue()).size() * 4, 400);
		} 
	}
	ofSetColor(255);
	if (videoPlayer.getTexture() -> isAllocated()) {
		videoPlayer.getTexture() -> draw(400, 60 + 150 - 200 * (videoPlayer.getHeight() / videoPlayer.getWidth()), 400, 400 * (videoPlayer.getHeight() / videoPlayer.getWidth()));
	}
	if (bReadMe) {
		ofSetColor(100);
		ofDrawRectangle(310, 10, 580, 430);
		ofSetColor(255, 200, 200);
		ofDrawBitmapString("1. Load an embedding file, for example:\n\nhttps://github.com/eyaler/word2vec-slim/\n\n2. Load an .srt subtitle file\n\n3. Load the corresponding video file\n\n- \"Play\" also reloads the used subtitles\n\n- \"Random start\" only works, if \"Custom words\" is deselected", 370, 90);
	}
	ofSetColor(255, 200, 200);
	ofDrawBitmapString(title, 600 - title.size() * 4, 30);
}
