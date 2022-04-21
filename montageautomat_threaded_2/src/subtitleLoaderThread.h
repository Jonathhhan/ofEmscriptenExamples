#pragma once
#include "ofxWord2Vec.h"
#include "srtparser.h"

class subtitleLoaderThread : public ofThread {
public:
	std::map<std::pair<int, int>, std::pair<ofxWord2VecVector, int>> mapSubVector;

	std::vector<std::vector<SubtitleItem*>> subVector;
	std::vector<std::string> stopWords;
	ofxWord2VecEmbedding embed;
	bool loaded;

	void subtitleLoaderThread::threadedFunction() {
		int counter = 0;
		std::vector<std::string> currentWords;
		ofDirectory dir("subtitles");
		dir.allowExt("srt");
		dir.listDir();
		dir.sort();
		subVector.clear();
		mapSubVector.clear();

		for (int i = 0; i < dir.size(); i++) {
			SubtitleParserFactory* subParserFactory = new SubtitleParserFactory("data/" + dir.getPath(i));
			SubtitleParser* parser = subParserFactory->getParser();
			std::vector<SubtitleItem*> sub = parser->getSubtitles();
			subVector.push_back(sub);
			for (auto element : sub) {
				std::string lowerString = ofToLower(element->getDialogue());
				ofStringReplace(lowerString, "'", " ");
				ofStringReplace(lowerString, "-", " ");
				char chars[] = "0123456789.,!:?;()\"";
				for (auto element : chars) {
					ofStringReplace(lowerString, ofToString(element), "");
				}
				std::vector<std::string> splitWords = ofSplitString(lowerString, " ");
				for (auto& element : splitWords) {
					if (embed.find_case_sensitive(element) != -1) {
						currentWords.push_back(element);
					}
				}
				for (auto& element : stopWords) {
					currentWords.erase(std::remove(currentWords.begin(), currentWords.end(), element), currentWords.end());
				}
				if (element->getDialogue().back() == '.' || element->getDialogue().back() == '?' || element->getDialogue().back() == '!' || element->getDialogue().back() == '"' || element->getDialogue().back() == '\'' || element->getDialogue().back() == ';') {
					std::string currentDialogue = ofJoinString(currentWords, " ");
					currentWords.clear();
					if (!currentDialogue.empty()) {
						mapSubVector.emplace(std::make_pair(i, element->getSubNo() - counter - 1), std::make_pair(embed.words_to_vec(currentDialogue), counter));
					}
					counter = -1;
				}
				counter++;
			}
			std::cout << "Subtitles: " << dir.getPath(i) << ", Subtitle size: " << sub.size() << ", Subtitle number: " << i << std::endl;
		}
		loaded = true;
		std::cout << "Subtitles loaded!" << std::endl;
	}
};