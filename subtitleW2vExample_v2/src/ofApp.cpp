#include "ofApp.h"

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
void ofApp::setup() {
videoPlayer.load("Alphaville.mp4");
videoPlayer.play();
	title = "Montageautomat 2";
	movieTime = 0;
	selectSubtitle = 0;
	fileName = "GoogleNews-vectors-negative300-SLIM.bin";	
	std::cout << "Loading embeddings file: " << fileName << std::endl;
	embed.load_binary(fileName);
	std::cout << "Words in " << fileName << ": " << embed.words << std::endl;
	std::cout << "Dimensions in " << fileName << ": " <<  embed.size << std::endl;
	subParserFactory = new SubtitleParserFactory(ofToDataPath("Alphaville.ENG.srt"));
	parser = subParserFactory->getParser();

	// to get subtitles
	sub = parser -> getSubtitles();
	std::cout << "Subtitles: " << sub.size() << std::endl;
	
	// exclude those words from subtitles
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
	if (sub[selectSubtitle] -> getStartTime() <= movieTime + ofGetElapsedTimeMillis() && sub[selectSubtitle] -> getEndTime() >= movieTime + ofGetElapsedTimeMillis()) {
		ofSetColor(255, 200, 200);
		ofDrawBitmapString(sub[selectSubtitle] -> getDialogue(), 300 - sub[selectSubtitle] -> getDialogue().size() * 4, 400);
	} else if (sub[selectSubtitle] -> getEndTime() + 1000 <= movieTime + ofGetElapsedTimeMillis() && subIndex.size() > 0) {
		std::map<int, float> m;
		std::map<std::string, float> m3;
		std::multimap<float, int> m2;
		std::vector<int> used_indices;
		std::vector<string> v1;
		std::vector<int> choosenSubs;
		currentSubtitle = sub[selectSubtitle] -> getDialogue();

		// process current subtitle
		ofToLower(currentSubtitle);
		char chars[] = ".,-!:?0123456789;'";
		for (int i = 0; i < strlen(chars); ++i) {
			ofStringReplace(currentSubtitle, chars[i], "");
		}
		currentSubtitle.insert(0, " ");
		currentSubtitle += " ";
		for (int i = 0; i < stopWords.size(); i++) {
			ofStringReplace(currentSubtitle, " " + stopWords[i] + " ", " ");
            	}
            	if (currentSubtitle.length() > 1) {
            		currentSubtitle.erase(0, 1);
            		currentSubtitle.pop_back();
            	}
				
		// create vector from words
		Vec = embed.words_to_vec(currentSubtitle, &used_indices);
		if (!Vec.empty()) {
			int count = 50; // number of vector words
			match = embed.match_cos(Vec, count, used_indices);
			for (int i = 0; i < match.size(); i++) {
				m3[match[i].word] = match[i].value;
			}	
		}
		
		// find subtitle with most common w2v words
		for (float element : subIndex) {
		float weight = 0;
		vector<string> target_list;
		v1 = sub[element] -> getIndividualWords();
		std::sort(v1.begin(), v1.end());
		std::set_intersection(v1.begin(), v1.end(), m3.begin(),
		m3.end(), back_inserter(target_list), mycomparer());
		for(const auto & c : target_list) { 
			weight += m3[c];
		}
		m[sub[element] -> getSubNo()] = weight / sub[element] -> getWordCount();
		}
		for (auto&& i : m) {
    			m2.insert(std::make_pair(i.second, i.first));
    		}
		auto it = m2.rbegin(); // get the elem with the highest key
		auto range = m2.equal_range(it -> first);
		for (auto it = range.first; it != range.second; ++it) {
    			// std::cout << "Weight: " << it -> first << ", Subtitle: " << it -> second - 1 << ", Dialogue: " << sub[it -> second - 1] -> getDialogue() << std::endl; 
    			choosenSubs.push_back(it -> second);
    		}
    		
    		// choose a random subtitle with highest key
    		int random = rand() % choosenSubs.size();
		selectSubtitle = choosenSubs[random] - 1;
		
		// set new time
		videoPlayer.setPosition((sub[selectSubtitle - 1] -> getEndTime() / 1000) / videoPlayer.getDuration());
		movieTime = sub[selectSubtitle - 1] -> getEndTime();
		ofResetElapsedTimeCounter();
		
		// exclude choosen subtitle
		for(auto it = subIndex.begin(); it != subIndex.end();) {
			if (*it == selectSubtitle) {
				it = subIndex.erase(it);
			} else {
				++it;
			}
		}
	}
}
