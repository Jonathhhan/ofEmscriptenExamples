#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
videoPlayer.load("Alphaville.mp4");
videoPlayer.play();
	title = "Montageautomat 2";
	movieTime = 0;
	selectSubtitle = 0;
	fileName = "vec_text8.bin";	
	std::cout << "Loading embeddings file: " << fileName << std::endl;
	embed.load_binary(fileName);
	std::cout << "Words in " << fileName << ": " << embed.words << std::endl;
	std::cout << "Dimensions in " << fileName << ": " <<  embed.size << std::endl;
	subParserFactory = new SubtitleParserFactory(ofToDataPath("Alphaville.ENG.srt"));
	parser = subParserFactory->getParser();

	//to get subtitles
	sub = parser->getSubtitles();
	std::cout << "Subtitles: " << sub.size() << std::endl;
	
	// exclude those words
	stopWords = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now"};
	for (int i = 1; i < sub.size(); ++i) {
		subIndex.insert(i);
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	videoPlayer.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(200);
	ofDrawBitmapString(title, 300 - title.size() * 4, 30);
	ofDrawRectangle(90, 50, 420, 320);
	ofSetColor(255);
	videoPlayer.draw(100, 60, 400, 300);
	if (sub[selectSubtitle] -> getStartTime() <= movieTime + ofGetElapsedTimeMillis() && sub[selectSubtitle]->getEndTime() >= movieTime + ofGetElapsedTimeMillis()) {
		ofSetColor(255, 200, 200);
		ofDrawBitmapString(sub[selectSubtitle] -> getDialogue(), 300 - sub[selectSubtitle] -> getDialogue().size() * 4, 400);
	} else if (sub[selectSubtitle] -> getEndTime() + 1000 <= movieTime + ofGetElapsedTimeMillis() && subIndex.size() > 0) {
		std::map<int, int> m;
		std::multimap<int, int> m2;
		std::vector<int> used_indices;
		std::vector<string> v1, v2;
		std::vector<int> choosenSubs;
	    	individualWords = sub[selectSubtitle] -> getIndividualWords();
	    	
	    	// find next subtitle
            	for(std::string individualWord : individualWords) {
            		if (std::find(stopWords.begin(), stopWords.end(), individualWord) != stopWords.end()) {
            			// do nothing
            		} else {
            		
            			// make everything lower case
    				std::for_each(individualWord.begin(), individualWord.end(), [](char & c) {
        				c = std::tolower(c);
    				});
    				
    				// erase those chars
				char chars[] = ".,-!:?0123456789; ";
				for (int i = 0; i < strlen(chars); ++i) {
					individualWord.erase (std::remove(individualWord.begin(), individualWord.end(), chars[i]), individualWord.end());
				}
				individualWord = individualWord.substr(0, individualWord.find("'"));
				
				// create vector from words
				ofxWord2VecVector Vec = embed.words_to_vec(individualWord, &used_indices);
				if (!Vec.empty()) {
					int count = 10; // number of vector words
					auto match = embed.match_cos(Vec, count, used_indices);
					for (int i = 0; i < match.size(); i++) {
						v1.push_back(match[i].word);
					}	
				}
			}
		}
		
		// find subtitle with most common w2v words
		for (float element : subIndex) {
			v2 = sub[element] -> getIndividualWords();
			std::sort(v1.begin(), v1.end());
			std::sort(v2.begin(), v2.end());
			std::vector<string> v_intersection;
			std::set_intersection(v1.begin(), v1.end(),
			v2.begin(), v2.end(),
			std::back_inserter(v_intersection));
			int counter = 0;
			for(std::string n : v_intersection) {
				counter++;
			}
			m[sub[element] -> getSubNo()] = (counter * 100000) / sub[element] -> getWordCount();
		}
		for (auto&& i : m) {
    			m2.insert(std::make_pair(i.second, i.first));
    		}
		auto it = m2.rbegin(); // get the elem with the highest key
		auto range = m2.equal_range(it -> first);
		for (auto it = range.first; it != range.second; ++it) {
    			std::cout << "Weight: " << it -> first << ", Subtitle: " << it -> second << std::endl; 
    			choosenSubs.push_back(it -> second);
    		}
    		
    		// choose a random subtitle with highest key
    		int random = rand() % choosenSubs.size();
		selectSubtitle = choosenSubs[random] - 1;
		
		// set new time
		std::cout << "Selected subtitle: " << selectSubtitle << std::endl;
		std::cout << "Dialog: " << sub[selectSubtitle] -> getDialogue()<<std::endl;
		videoPlayer.setPosition((sub[selectSubtitle - 1] -> getEndTime() / 1000) / videoPlayer.getDuration());
		movieTime = sub[selectSubtitle - 1] -> getEndTime();
		ofResetElapsedTimeCounter();
		
		// exclude choosen subtitle
		for(auto it = subIndex.begin(); it != subIndex.end(); ) {
			if (*it == selectSubtitle) {
				it = subIndex.erase(it);
			} else {
				++it;
			}
		}
	}
}
