#include "ofApp.h"
#include "emscripten.h"
#include "emscripten/bind.h"

using namespace emscripten;

ofEvent<std::string> loadAudioUrlEvent; 	
ofEvent<std::string> loadVideoUrlEvent; 

void loadAudioUrl(const std::string & url) {
	std::string urlCopy = url;         
	loadAudioUrlEvent.notify(urlCopy); 
} 

void loadVideoUrl(const std::string & url) {
	std::string urlCopy = url;         
	loadVideoUrlEvent.notify(urlCopy); 
} 

EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("loadAudioUrl", & loadAudioUrl);
	emscripten::function("loadVideoUrl", & loadVideoUrl);
}

void ofApp::loadAudioUrlX(std::string & rv) {
	if (audioPlayer.isPlaying())	
	audioPlayer.stop();
	audioPlayer.loadString(rv);
}

void ofApp::loadVideoUrlX(std::string & rv) {
	if (videoPlayer.isPlaying())	
	videoPlayer.stop();
	videoPlayer.loadString(rv);
}

//--------------------------------------------------------------
void ofApp::hSlider_1onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-reverb", e); 
	label_6.symbol = ofToString(e); 
}

//--------------------------------------------------------------
void ofApp::hSlider_2onMousePressed(float & e){ 
	pd.sendFloat(patch.dollarZeroStr() + "-lowpass", e);  
	label_8.symbol = ofToString(e);
}

//--------------------------------------------------------------
void ofApp::hSlider_3onMousePressed(float & e){ 
	pd.sendFloat(patch.dollarZeroStr() + "-volume", e);
	label_10.symbol = ofToString(e); 
}   

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e){ 
	EM_ASM(
	var file_selector = document.createElement('input');
	file_selector.setAttribute('type', 'file');
	file_selector.setAttribute('accept','.wav, .mp3, .ogg');
	file_selector.addEventListener("change", function(e){
	var file = e.target.files[0]; 
	var url = URL.createObjectURL(file);
	Module.loadAudioUrl(url);
	});
	file_selector.click();
	);
}   

//--------------------------------------------------------------
void ofApp::bang_2onMousePressed(bool & e){ 
	EM_ASM(
	var file_selector = document.createElement('input');
	file_selector.setAttribute('type', 'file');
	file_selector.setAttribute('accept','.mp4, .webm');
	file_selector.addEventListener("change", function(e){
	var file = e.target.files[0]; 
	var url = URL.createObjectURL(file);
	Module.loadVideoUrl(url);
	});
	file_selector.click();
	);
}   

//--------------------------------------------------------------
void ofApp::bang_3onMousePressed(bool & e){ 	
	audioPlayer.play();
}   
 
//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(100, 100, 100);
	videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
	videoPlayer.setUsePixels(false);
	ofAddListener(hSlider_1.onMousePressed, this, & ofApp::hSlider_1onMousePressed);
	ofAddListener(hSlider_2.onMousePressed, this, & ofApp::hSlider_2onMousePressed); 
	ofAddListener(hSlider_3.onMousePressed, this, & ofApp::hSlider_3onMousePressed);
	ofAddListener(bang_1.onMousePressed, this, & ofApp::bang_1onMousePressed);
	ofAddListener(bang_2.onMousePressed, this, & ofApp::bang_2onMousePressed);
	ofAddListener(bang_3.onMousePressed, this, & ofApp::bang_3onMousePressed);
	ofAddListener(loadAudioUrlEvent, this, & ofApp::loadAudioUrlX);
	ofAddListener(loadVideoUrlEvent, this, & ofApp::loadVideoUrlX);
	bang_1.setup(50, 125, 20);
	bang_2.setup(50, 150, 20);
	bang_3.setup(50, 175, 20);
	label_1.setup(50, 30, 205, 75, "");
	label_2.setup(155, 125, 100, 20, "Load audio");
	label_3.setup(155, 150, 100, 20, "Load video");
	label_4.setup(155, 175, 100, 20, "Play");
	label_5.setup(155, 200, 100, 20, "Reverb");
	label_6.setup(155, 225, 100, 20, "0");
	label_7.setup(155, 250, 100, 20, "Lowpass");
	label_8.setup(155, 275, 100, 20, "100");
	label_9.setup(155, 300, 100, 20, "Volume");
	label_10.setup(155, 325, 100, 20, "50");
	hSlider_1.setup(50, 200, 100, 20, 0, 20);
	hSlider_1.slider = 0;
	hSlider_2.setup(50, 250, 100, 20, 0, 100);
	hSlider_2.slider = 0.8;
	hSlider_3.setup(50, 300, 100, 20, 0, 100);
	hSlider_3.slider = 0.5;
	
	//ofSetLogLevel("Pd", OF_LOG_VERBOSE); // see verbose info inside

	// double check where we are ...
	cout << ofFilePath::getCurrentWorkingDirectory() << endl;

	// the number of libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)
	#ifdef TARGET_LINUX_ARM
		// longer latency for Raspberry PI
		int ticksPerBuffer = 32; // 32 * 64 = buffer len of 2048
		int numInputs = 2; // no built in mic
	#else
		int ticksPerBuffer = 2; // 8 * 64 = buffer len of 512
		int numInputs = 2;
	#endif

	// setup OF sound stream
	ofSoundStreamSettings settings;
	settings.numInputChannels = 2;
	settings.numOutputChannels = 2;
	settings.sampleRate = 44100;
	settings.bufferSize = ofxPd::blockSize() * ticksPerBuffer;
	settings.setInListener(this);
	settings.setOutListener(this);
	ofSoundStreamSetup(settings);

	if(!pd.init(2, numInputs, 44100, ticksPerBuffer, false)) {
		OF_EXIT_APP(1);
	}

	// subscribe to receive source names
	pd.subscribe("toOF");
	pd.addReceiver(*this); // automatically receives from all subscribed sources 
	pd.start();
	patch = pd.openPatch("pd/test.pd");
	 
	pd.sendFloat(patch.dollarZeroStr() + "-reverb", 0);
	pd.sendFloat(patch.dollarZeroStr() + "-lowpass", 100);
	pd.sendFloat(patch.dollarZeroStr() + "-volume", 50);
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSoundUpdate();
	videoPlayer.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(240, 220, 100);
        ofDrawRectangle(20, 20, 270, 450);
        ofSetColor(0, 0, 0);
        ofDrawRectangle(50, 350, 205, 110);
	label_1.draw();
	label_2.draw();
	label_3.draw();
	label_4.draw();
	label_5.draw();
	label_6.draw();
	label_7.draw();
	label_8.draw();
	label_9.draw();
	label_10.draw();
	bang_1.draw();
	bang_2.draw();
	bang_3.draw();
	hSlider_1.draw();
	hSlider_2.draw();
	hSlider_3.draw();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Load an audio file", 60, 55);
	ofDrawBitmapString("or an video file", 60, 75);
	ofDrawBitmapString("or use some audio input", 60, 95);
	if (videoPlayer.getTexture() -> isAllocated()) {
	videoPlayer.getTexture() -> draw(50, 350, 205, 110);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofSoundStreamStop();
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}
