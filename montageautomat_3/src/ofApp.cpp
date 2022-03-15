#include "ofApp.h"

#include "emscripten.h"
#include "emscripten/bind.h"

//--------------------------------------------------------------
ofEvent<std::string &> loadBinEvent;
ofEvent<std::string &> loadSubtitleEvent;
ofEvent<std::string &> loadVideoEvent;

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
	subParserFactory = new SubtitleParserFactory(ofToDataPath(file));
	parser = subParserFactory->getParser();

	// to get subtitles
	sub = parser -> getSubtitles();
	EM_ASM(FS.unlink("/data/" + UTF8ToString($0)), file.c_str());
	mapSubVector.clear();
	for (auto sub_element : sub) {
		
		// process subtitles
		currentWords = sub_element -> getIndividualWords();
		tempCurrentWords.clear();
		for (std::string element : currentWords) {
			currentWord = ofToLower(element);	
			ofStringReplace(currentWord, "'", " ");
			ofStringReplace(currentWord, "-", " ");
			char chars[] = "0123456789.,!:?;";
			for (int i = 0; i < strlen(chars); ++i) {
				ofStringReplace(currentWord, ofToString(chars[i]), "");
			}
			splitWords = ofSplitString(currentWord, " ");
			for (std::string element : splitWords) {
				if (element.length() > 0 && embed.find_case_sensitive(element) != -1) { 	
					tempCurrentWords.push_back(element);
				}
			}
			for (std::string element : stopWords) {
				tempCurrentWords.erase(std::remove(tempCurrentWords.begin(), tempCurrentWords.end(), element), tempCurrentWords.end());
			}	
		}
		currentDialogue = ofJoinString(tempCurrentWords, " ");
		if (currentDialogue.length() > 0){
			mapSubVector[sub_element -> getSubNo()] = embed.words_to_vec(currentDialogue, &used_indices);
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
EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("loadBin", &loadBin);
	emscripten::function("loadSubtitle", &loadSubtitle);
	emscripten::function("loadVideo", &loadVideo);
}

//--------------------------------------------------------------
void ofApp::bang_4onMousePressed(bool & e) {
	if (videoPlayer.isLoaded()) {
		mapSubVectorCopy = mapSubVector;
		if (randomStart && sub.size() > 0) {
			selectSubtitle = rand() % sub.size();
			videoPlayer.setPosition(sub[selectSubtitle] -> getStartTime() / 1000 / videoPlayer.getDuration());
			drawSubtitleDialogue = sub[selectSubtitle] -> getDialogue();
		} else {
			selectSubtitle = 0;
			videoPlayer.setPosition(0);
			drawSubtitleDialogue = sub[0] -> getDialogue();
		}
		videoPlayer.play();
		groupOfToggles[0].value = 0;
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
void ofApp::hSlider_1onMousePressed(float & e) {
	videoPlayer.setVolume(e);
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofAddListener(groupOfBangs[0].onMousePressed, this, &ofApp::bang_1onMousePressed);
	ofAddListener(groupOfBangs[1].onMousePressed, this, &ofApp::bang_2onMousePressed);
	ofAddListener(groupOfBangs[2].onMousePressed, this, &ofApp::bang_3onMousePressed);
	ofAddListener(groupOfBangs[3].onMousePressed, this, &ofApp::bang_4onMousePressed);
	ofAddListener(groupOfToggles[0].onMousePressed, this, &ofApp::toggle_1onMousePressed);
	ofAddListener(groupOfToggles[1].onMousePressed, this, &ofApp::toggle_2onMousePressed);
	ofAddListener(hSlider_1.onMousePressed, this, &ofApp::hSlider_1onMousePressed);
	ofAddListener(loadBinEvent, this, & ofApp::loadBinX);
	ofAddListener(loadSubtitleEvent, this, & ofApp::loadSubtitleX);
	ofAddListener(loadVideoEvent, this, & ofApp::loadVideoX);
	
	videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
	videoPlayer.setUsePixels(false);
	ofSetBackgroundColor(200);
	title = "Montageautomat 3";

	groupOfBangs[0].setup(190, 80 + 5, 20);
	groupOfBangs[1].setup(190, 110 + 5, 20);
	groupOfBangs[2].setup(190, 140 + 5, 20);
	groupOfBangs[3].setup(190, 200 + 5, 20);
	groupOfToggles[0].setup(190, 230 + 5, 20);
	groupOfToggles[1].setup(190, 260 + 5, 20);
	hSlider_1.setup(190, 290 + 5, 80, 20, 0, 1);
	hSlider_1.value = 0.5;
	hSlider_1.slider = 0.5;
	
	// exclude those words
	stopWords = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now", "d", "do", "m", "re", "ll", "didn", "doesn", "hasn", "hadn", "cannot", "mustn", "isn", "wasn", "couldn", "wouldn", "mr", "ve", "l", "y", "to", "o", "m", "lsn", "from", "out"};
}

//--------------------------------------------------------------
void ofApp::update() {
	videoPlayer.update();
	if (sub[selectSubtitle] -> getEndTime() + 1000 <= videoPlayer.getPosition() * 1000 && mapSubVectorCopy.size() > 0) {

		// get vector similarities	
		for (auto element : mapSubVectorCopy) {
			if (element.first != selectSubtitle + 1) {
				multimapWeightSub.insert(std::make_pair(mapSubVector[selectSubtitle + 1].dist_cosine_optimized(element.second), element.first));
			}
		}
		mapSubVectorCopy.erase(selectSubtitle + 1);
		
		// choose a random subtitle with highest key
		auto it = multimapWeightSub.rbegin();
		if (it -> first != 0) {
			auto range = multimapWeightSub.equal_range(it -> first);
			for (auto it = range.first; it != range.second; ++it) {
				choosenSubs.push_back(it -> second);
			}
			selectSubtitle = choosenSubs[rand() % choosenSubs.size()] - 1;
			std::cout << "Weight: " << it -> first << ", Subtitle: " << selectSubtitle  << ", Dialogue: " << sub[selectSubtitle] -> getDialogue() << std::endl; 
		} else {
			selectSubtitle = rand() % sub.size();
			std::cout << "Weight: " << it -> first << ", Subtitle: " << selectSubtitle << ", Dialogue: " << sub[selectSubtitle] -> getDialogue() << std::endl; 
		}
		if (mapSubVectorCopy.size() == 1) {		
			mapSubVectorCopy = mapSubVector;
			selectSubtitle = rand() % sub.size();
		}
		multimapWeightSub.clear();
		choosenSubs.clear();

		// set new video position and subtitle
		videoPlayer.setPosition((sub[selectSubtitle - 1] -> getEndTime() / 1000 + 1) / videoPlayer.getDuration());
		drawSubtitleDialogue = sub[selectSubtitle] -> getDialogue(1);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(100);
	ofDrawRectangle(10, 10, 880, 430);
	ofSetColor(200);
	ofDrawRectangle(290, 10, 10, 430);
	ofDrawRectangle(390, 50, 420, 320);
	ofSetColor(255, 200, 200);
	ofDrawBitmapString(title, 600 - title.size() * 4, 30);
	ofDrawBitmapString("Load embedding file", 30, 100);
	ofDrawBitmapString("Load subtitle", 30, 130);
	ofDrawBitmapString("Load movie", 30, 160);
	ofDrawBitmapString("Play", 30, 220);
	ofDrawBitmapString("Pause", 30, 250);
	ofDrawBitmapString("Random start", 30, 280);
	ofDrawBitmapString("Volume", 30, 310);
	for(int i=0; i<NBANGS; i++) {
		groupOfBangs[i].draw();
	}
	for(int i=0; i<NTOGGLES; i++) {
		groupOfToggles[i].draw();
	}
	hSlider_1.draw();
	if (sub[selectSubtitle] -> getStartTime() <= videoPlayer.getPosition() * 1000 && sub[selectSubtitle] -> getEndTime() >= videoPlayer.getPosition() * 1000) {
		ofDrawBitmapString(drawSubtitleDialogue, 600 - drawSubtitleDialogue.size() * 4, 400);
	} 
	ofSetColor(255);
	if (videoPlayer.getTexture() -> isAllocated()) {
		videoPlayer.getTexture() -> draw(400, 60 + 150 - 200 * (videoPlayer.getHeight() / videoPlayer.getWidth()), 400, 400 * (videoPlayer.getHeight() / videoPlayer.getWidth()));
	}
}
