#include "ofApp.h"



    #include <algorithm>
    #include <iterator>
    #include <string>
    #include <vector>
    #include <list>
    #include <iostream>



//--------------------------------------------------------------
void ofApp::setup() {

	string file_name = "vec_text8.bin";	
	cout << "Loading embeddings file: " << file_name << endl;
	embed.load_binary(file_name);

	cout << "words: " << embed.words << endl;
	cout << "dimensions: " << embed.size << endl;
	subParserFactory = new SubtitleParserFactory(ofToDataPath("Alphaville.ENG.srt"));
	parser = subParserFactory->getParser();

	//to get subtitles

	sub = parser->getSubtitles();
}

//--------------------------------------------------------------
void ofApp::draw() {
	vector<int> used_indices;
	for (SubtitleItem* element : sub) {
		std::map<int, int> m;
		if (element->getStartTime() <= (ofGetElapsedTimeMillis() + 35000) && element->getEndTime() >= (ofGetElapsedTimeMillis() + 35000)) {
			ofSetColor(255, 200, 200);
			ofDrawBitmapString(element->getDialogue(), 50, 30);
			ofSetColor(200, 255, 200);
			ofDrawBitmapString(s3, 50, 80);
			if (subNo != element->getSubNo()) {
				s = "";
				subNo = element->getSubNo();
	    			word = element->getIndividualWords();
            			for(std::string display : word){
    					std::for_each(display.begin(), display.end(), [](char & c) {
        					c = ::tolower(c);
    					});
					char chars[] = ".,-!:?0123456789; ";
					for (unsigned int i = 0; i < strlen(chars); ++i) {
						display.erase (std::remove(display.begin(), display.end(), chars[i]), display.end());
					}
					std::string s1 = display.substr(0, display.find("'"));
					if (s1 != "you" && s1 != "i" && s1 != "the" && s1 != "and" && s1 != "this" && s1 != "if" && s1 != "or" && s1 != "my" && s1 != "that" && s1 != "your" && s1 != "me" && s1 != "a" && s1 != "to" && s1 != "was" && s1 != "are"  && s1 != "it" && s1 != "with" && s1 != "for" && s1 != "in" && s1 != "is" && s1 != "of" && s1 != "von" && s1 != "on" && s1 != "braun" && s1 != "sir" && s1 != "alphaville") {
						s.append (s1 + " + ");
					}
				}
				s.pop_back();
				s.pop_back();
				s.pop_back();
            			ofxWord2VecVector Vec = embed.words_to_vec(s, &used_indices);
				if (!Vec.empty()) {
					s2 = "";
					int count = 200;
					auto match = embed.match_cos(Vec, count, used_indices);
					cout << "Result:" << endl;
					for (int i = 0; i < match.size(); i++) {
						s2.append(match[i].word + " ");
					}			
					for (SubtitleItem* element : sub) {
						vector<string> v1, v2;
						v2 = element->getIndividualWords();
						string temp;
						std::stringstream stringstream1(s2);
						while (stringstream1 >> temp)
							v1.push_back(temp);
						int counter = 0;
						std::sort(v1.begin(), v1.end());
						std::sort(v2.begin(), v2.end());
						std::vector<string> v_intersection;
						std::set_intersection(v1.begin(), v1.end(),
						v2.begin(), v2.end(),
						std::back_inserter(v_intersection));
						for(string n : v_intersection)
							counter++;
						m[element->getSubNo()] = counter;
					}
					std::map<int,int>::iterator best = std::max_element(m.begin(),m.end(),[] (const std::pair<char,int>& a, const std::pair<char,int>& b)->bool{ return a.second < b.second; } );
					std::cout << best->first << " , " << best->second << "\n";
					std::cout << element->getDialogue() << "\n";
					std::cout << sub[best->first]->getDialogue() << "\n";	
					s3 = sub[best->first]->getDialogue();
				}
			}
		}
	}
}
