#pragma once
#include "ofxWord2Vec.h"

class threads : public ofThread {
public:
	std::map<std::pair<int, int>, std::pair<ofxWord2VecVector, int>> mapSubVector;
	std::map<std::pair<int, int>, std::pair<ofxWord2VecVector, int>> mapSubVectorCopy;
	ofxWord2VecEmbedding embed;
	std::string customWords;
	bool bCustomWords;
	int selectedSubtitle;
	int numberOfVideoPlayer;
	int numberOfSubtitles;
	int possibilities;
	double weight;

	void threads::threadedFunction() {
		std::multimap<double, std::tuple<int, int, int>> multimapWeightSub;
		for (auto& element : mapSubVectorCopy) {
			if (element.first.first != numberOfVideoPlayer || element.first.second != selectedSubtitle) {
				if (!bCustomWords || customWords.empty()) {
					multimapWeightSub.insert(std::make_pair(mapSubVectorCopy[{numberOfVideoPlayer, selectedSubtitle}].first.dist_cosine_optimized(element.second.first), std::make_tuple(element.first.first, element.first.second, element.second.second)));
				}
				else {
					multimapWeightSub.insert(std::make_pair(embed.words_to_vec(customWords).dist_cosine_optimized(element.second.first), std::make_tuple(element.first.first, element.first.second, element.second.second)));
				}
			}
		}
		mapSubVectorCopy.erase({ numberOfVideoPlayer, selectedSubtitle });
		if (mapSubVectorCopy.size() > 0) {
			auto it = multimapWeightSub.rbegin();
			weight = it -> first;
			if (it -> first != 0) {
				std::vector<std::tuple<int, int, int>> choosenSubs;
				auto range = multimapWeightSub.equal_range(it -> first);
				for (auto it = range.first; it != range.second; ++it) {
					choosenSubs.push_back(std::make_tuple(get<0>(it -> second), get<1>(it -> second), get<2>(it -> second)));
				}
				possibilities = choosenSubs.size();
				ofSeedRandom();
				int random = ofRandom(choosenSubs.size());
				numberOfVideoPlayer = get<0>(choosenSubs[random]);
				selectedSubtitle = get<1>(choosenSubs[random]);
				numberOfSubtitles = get<2>(choosenSubs[random]);
			}
			else {
				possibilities = mapSubVectorCopy.size();
				auto it = mapSubVectorCopy.begin();
				ofSeedRandom();
				std::advance(it, ofRandom(possibilities));
				numberOfVideoPlayer = it -> first.first;
				selectedSubtitle = it -> first.second;
				numberOfSubtitles = it -> second.second;
			}
		}
		else {
			mapSubVectorCopy = mapSubVector;
			weight = 0;
			possibilities = mapSubVectorCopy.size();		
			auto it = mapSubVectorCopy.begin();
			ofSeedRandom();
			std::advance(it, ofRandom(possibilities));
			numberOfVideoPlayer = it -> first.first;
			selectedSubtitle = it -> first.second;
			numberOfSubtitles = it -> second.second;
		}
	}
};