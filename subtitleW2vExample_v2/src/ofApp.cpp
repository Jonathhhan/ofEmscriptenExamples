#include "ofApp.h"

#include "emscripten.h"
#include "emscripten/bind.h"

struct SomeCustomId { 
    int id;
    operator int() const { return id; }
    SomeCustomId() : id(0) {}
    SomeCustomId(int c)  : id(c) { }
};

struct mycomparer {
    bool operator()(string const& lhs, pair<string const, SomeCustomId> const& rhs) {
        return lhs < rhs.first;
    }
    bool operator()(pair<string const, SomeCustomId> const& lhs, string const& rhs) {
        return lhs.first < rhs;
    }
};

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
	std::cout << "Subtitles: " << sub.size() << std::endl;
	for (int i = 1; i < sub.size(); ++i) {
		subIndex.push_back(i);
	}
	EM_ASM(FS.unlink("/data/" + UTF8ToString($0)), file.c_str());
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
	subIndex.clear();
	if (videoPlayer.isLoaded()) {
		for (int i = 1; i < sub.size(); ++i) {
			subIndex.push_back(i);
		}
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
void ofApp::number_1onMousePressed(float & e) {
	vectorWords = e;
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
	ofAddListener(number_1.onMousePressed, this, &ofApp::number_1onMousePressed);
	ofAddListener(loadBinEvent, this, & ofApp::loadBinX);
	ofAddListener(loadSubtitleEvent, this, & ofApp::loadSubtitleX);
	ofAddListener(loadVideoEvent, this, & ofApp::loadVideoX);
	
	videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
	videoPlayer.setUsePixels(false);
	ofSetBackgroundColor(200);
	title = "Montageautomat 2";
	vectorWords = 50;
	
	// exclude those words from subtitles
	stopWords = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "this", "that", "these", "those", "there's" "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "haven't", "do", "done", "does", "did", "doing", "don't", "did'nt", "a", "an", "the", "and", "as", "of", "at", "by", "for", "with", "to", "from", "in", "out", "on", "off", "then", "so", "s", "t", "don", "i'm", "it's", "i'll", "i've", "alphaville", "we'll", "let's", "will", "can", "your're", "i'm", "i'd", "would", "wouldn't", "will", "won’t", "seem", "should", "should'nt", "could", "couldn't", "isn't", "we're"};

	groupOfBangs[0].setup(190, 80 + 5, 20);
	groupOfBangs[1].setup(190, 110 + 5, 20);
	groupOfBangs[2].setup(190, 140 + 5, 20);
	groupOfBangs[3].setup(190, 170 + 5, 20);
	groupOfToggles[0].setup(190, 200 + 5, 20);
	groupOfToggles[1].setup(190, 230 + 5, 20);
	number_1.setup(190, 260 + 5, 80, 20, 1, 1000);
	number_1.value = 50;
	hSlider_1.setup(190, 290 + 5, 80, 20, 0, 1);
	hSlider_1.value = 0.5;
	hSlider_1.slider = 0.5;
}

//--------------------------------------------------------------
void ofApp::update() {
	videoPlayer.update();
	if (sub[selectSubtitle] -> getEndTime() + 1000 <= videoPlayer.getPosition() * 1000 && subIndex.size() > 0) {
		mapWordWeight.clear();
		multimapSubWeight.clear();
		choosenSubs.clear();

		// process current subtitle
		currentDialogue = sub[selectSubtitle]->getDialogue();
		lowerCurrentDialogue = ofToLower(currentDialogue);
		char chars[] = ".,-!:?0123456789;";
		for (int i = 0; i < strlen(chars); ++i) {
			ofStringReplace(lowerCurrentDialogue, ofToString(chars[i]), "");
		}
		lowerCurrentDialogue.insert(0, " ");
		lowerCurrentDialogue += " ";
		for (int i = 0; i < stopWords.size(); i++) {
			ofStringReplace(lowerCurrentDialogue, " " + stopWords[i] + " ", " ");
		}
		if (lowerCurrentDialogue.length() > 1) {
			lowerCurrentDialogue.erase(0, 1);
			lowerCurrentDialogue.pop_back();
		}

		// create vector from words
		Vec = embed.words_to_vec(lowerCurrentDialogue, &used_indices);
		if (!Vec.empty()) {
			match = embed.match_cos(Vec, vectorWords, used_indices);
			for (int i = 0; i < match.size(); i++) {
				mapWordWeight[match[i].word] = match[i].value;
			}	
		}
		
		// find subtitle with most common w2v words
		for (int element : subIndex) {
			weight = 0;
			vector<string> target_list;
			v1 = sub[element] -> getIndividualWords();
			std::sort(v1.begin(), v1.end());
			std::set_intersection(v1.begin(), v1.end(), mapWordWeight.begin(),
			mapWordWeight.end(), back_inserter(target_list), mycomparer());
			for(const auto & c : target_list) { 
				weight += mapWordWeight[c];
			}
			multimapSubWeight.insert(std::make_pair(weight / sub[element]->getWordCount(), sub[element]->getSubNo()));
		}
		
		// choose a random subtitle with highest key
		auto it = multimapSubWeight.rbegin();
		if (it -> first > 0) {
		auto range = multimapSubWeight.equal_range(it -> first);
		for (auto it = range.first; it != range.second; ++it) {
			choosenSubs.push_back(it -> second);
			std::cout << "Weight: " << it -> first << ", Subtitle: " << it -> second - 1 << ", Dialogue: " << sub[it -> second - 1] -> getDialogue() << std::endl; 
		}
		random = rand() % choosenSubs.size();
		selectSubtitle = choosenSubs[random] - 1;
		} else {
		random = rand() % subIndex.size();
		selectSubtitle = subIndex[random] - 1;
		}
		
		// set new time
		videoPlayer.setPosition((sub[selectSubtitle - 1] -> getEndTime() / 1000) / videoPlayer.getDuration());
		drawSubtitleDialogue = sub[selectSubtitle] -> getDialogue();
		
		// exclude choosen subtitle
		std::remove(subIndex.begin(), subIndex.end(), selectSubtitle);
		subIndex.pop_back();
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
	ofDrawBitmapString("Load bin", 30, 100);
	ofDrawBitmapString("Load subtitle", 30, 130);
	ofDrawBitmapString("Load movie", 30, 160);
	ofDrawBitmapString("Play", 30, 190);
	ofDrawBitmapString("Pause", 30, 220);
	ofDrawBitmapString("Random start", 30, 250);
	ofDrawBitmapString("Word2Vec words", 30, 280);
	ofDrawBitmapString("Volume", 30, 310);
	for(int i=0; i<NBANGS; i++) {
		groupOfBangs[i].draw();
	}
	for(int i=0; i<NTOGGLES; i++) {
		groupOfToggles[i].draw();
	}
	number_1.draw();
	hSlider_1.draw();
	if (sub[selectSubtitle] -> getStartTime() <= videoPlayer.getPosition() * 1000 && sub[selectSubtitle] -> getEndTime() >= videoPlayer.getPosition() * 1000) {
		ofDrawBitmapString(drawSubtitleDialogue, 600 - drawSubtitleDialogue.size() * 4, 400);
	} 
	ofSetColor(255);
	if (videoPlayer.getTexture() -> isAllocated()) {
		videoPlayer.getTexture() -> draw(400, 60 + 150 - 200 * (videoPlayer.getHeight() / videoPlayer.getWidth()), 400, 400 * (videoPlayer.getHeight() / videoPlayer.getWidth()));
	}
}
