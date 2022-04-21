#pragma once

class videoLoaderThread : public ofThread {
public:

	ofVideoPlayer videoPlayer;
	std::vector<ofVideoPlayer*> videoPlayerVector;
	bool loaded;

	void videoLoaderThread::threadedFunction() {
		for (auto element : videoPlayerVector) {
			element->close();
		}
		videoPlayerVector.clear();
		ofDirectory dir("videos");
		dir.allowExt("3gp");
		dir.allowExt("avi");
		dir.allowExt("divx");
		dir.allowExt("dv");
		dir.allowExt("h264");
		dir.allowExt("mkv");
		dir.allowExt("mp4");
		dir.allowExt("mpeg");
		dir.allowExt("mpg");
		dir.allowExt("mov");
		dir.allowExt("mxf");
		dir.allowExt("ogg");
		dir.allowExt("ogm");
		dir.allowExt("ogv");
		dir.allowExt("omf");
		dir.allowExt("qt");
		dir.allowExt("webm");
		dir.allowExt("wmv");
		dir.listDir();
		dir.sort();
		ofVideoPlayer* movie;
		for (int i = 0; i < dir.size(); i++) {
			movie = new ofVideoPlayer();
			movie->load(dir.getPath(i));
			movie->setLoopState(OF_LOOP_NONE);
			videoPlayerVector.push_back(movie);
			std::cout << "Video: " << dir.getPath(i) << ", Video number: " << i << std::endl;
		}
		loaded = true;
		std::cout << "Videos loaded!" << std::endl;
	}
};