#pragma once
#include "ofxWord2Vec.h"

class embedLoaderThread : public ofThread {
public:

	ofxWord2VecEmbedding embed;
	bool loaded;

	void embedLoaderThread::threadedFunction() {
		ofDirectory dir("embedding file");
		dir.allowExt("bin");
		dir.listDir();
		dir.sort();
		std::cout << "Loading embeddings file: " << dir.getPath(0) << std::endl;
		embed.load_binary(dir.getPath(0), false);
		std::cout << "Words in " << dir.getPath(0) << ": " << embed.words << std::endl;
		std::cout << "Dimensions in " << dir.getPath(0) << ": " << embed.size << std::endl;
		loaded = true;
	}
};