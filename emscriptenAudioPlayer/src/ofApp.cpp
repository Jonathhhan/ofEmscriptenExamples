#include "ofApp.h"
#include "emscripten.h"
#include "emscripten/bind.h"

using namespace emscripten;

ofEvent<std::vector<float>> audioInLeftEvent; 
ofEvent<std::vector<float>> audioInRightEvent; 	

void audioInLeft(const emscripten::val & audioIn) {                     
	std::vector<float> rv;   
	const auto l = audioIn["length"].as<unsigned>();      
	rv.resize(l);   
	emscripten::val memoryView{emscripten::typed_memory_view(l, rv.data())};        
	memoryView.call<void>("set", audioIn);  
	audioInLeftEvent.notify(rv); 
}  

void audioInRight(const emscripten::val & audioIn) {                     
	std::vector<float> rv;   
	const auto l = audioIn["length"].as<unsigned>();      
	rv.resize(l);   
	emscripten::val memoryView{emscripten::typed_memory_view(l, rv.data())};        
	memoryView.call<void>("set", audioIn);  
	audioInRightEvent.notify(rv); 
} 

EMSCRIPTEN_BINDINGS(Module) {
	emscripten::function("audioInLeft", & audioInLeft);
	emscripten::function("audioInRight", & audioInRight);
}

void ofApp::audioChangedLeft(std::vector<float> & rv) {
	pd.sendFloat(patch.dollarZeroStr() + "-arraySize", rv.size());
	pd.resizeArray(patch.dollarZeroStr() + "-left", rv.size());
	pd.writeArray(patch.dollarZeroStr() + "-left", rv);
}

void ofApp::audioChangedRight(std::vector<float> & rv) {	
	pd.sendFloat(patch.dollarZeroStr() + "-arraySize", rv.size());
	pd.resizeArray(patch.dollarZeroStr() + "-right", rv.size());
	pd.writeArray(patch.dollarZeroStr() + "-right", rv);
}

//--------------------------------------------------------------
void ofApp::toggle_1onMousePressed(bool & e){
	pd.sendFloat(patch.dollarZeroStr() + "-play", e);
}

//--------------------------------------------------------------
void ofApp::hSlider_1onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-tempo", e);
}

//--------------------------------------------------------------
void ofApp::hSlider_2onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-lowpass", e);
}

//--------------------------------------------------------------
void ofApp::hSlider_3onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-volume", e);
}

//--------------------------------------------------------------
void ofApp::bang_1onMousePressed(bool & e){
	EM_ASM_(
	loadAudio();
	);
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(100, 100, 100);
	ofAddListener(toggle_1.onMousePressed, this, & ofApp::toggle_1onMousePressed);
	ofAddListener(hSlider_1.onMousePressed, this, & ofApp::hSlider_1onMousePressed);
	ofAddListener(hSlider_2.onMousePressed, this, & ofApp::hSlider_2onMousePressed); 
	ofAddListener(hSlider_3.onMousePressed, this, & ofApp::hSlider_3onMousePressed);
	ofAddListener(bang_1.onMousePressed, this, & ofApp::bang_1onMousePressed);
	ofAddListener(audioInLeftEvent, this, & ofApp::audioChangedLeft);
	ofAddListener(audioInRightEvent, this, & ofApp::audioChangedRight);
	label_1.setup(20, 20, 300, 20, "Audio Player");
	label_2.setup(120, 100, 200, 20, "Play");
	label_3.setup(120, 140, 200, 20, "Tempo");
	label_4.setup(120, 180, 200, 20, "Lowpass");
	label_5.setup(120, 220, 200, 20, "Volume");
	label_7.setup(120, 60, 200, 20, "Load Audio");
	toggle_1.setup(20, 100, 20);
	toggle_1.value = true;
	bang_1.setup(20, 60, 20);
	hSlider_1.setup(20, 140, 80, 20, -2, 2);
	hSlider_1.slider = 0.75;
	hSlider_2.setup(20, 180, 80, 20, 1, 125);
	hSlider_2.slider = 0.8;
	hSlider_3.setup(20, 220, 80, 20, 0, 1);
	hSlider_3.slider = 0.5;
	
	//ofSetLogLevel("Pd", OF_LOG_VERBOSE); // see verbose info inside

	// double check where we are ...
	cout << ofFilePath::getCurrentWorkingDirectory() << endl;

	// the number of libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)
	#ifdef TARGET_LINUX_ARM
		// longer latency for Raspberry PI
		int ticksPerBuffer = 32; // 32 * 64 = buffer len of 2048
		int numInputs = 0; // no built in mic
	#else
		int ticksPerBuffer = 2; // 8 * 64 = buffer len of 512
		int numInputs = 0;
	#endif

	// setup OF sound stream
	ofSoundStreamSettings settings;
	settings.numInputChannels = 0;
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
	
	pd.sendFloat(patch.dollarZeroStr() + "-play", true);
	pd.sendFloat(patch.dollarZeroStr() + "-tempo", 1);
	pd.sendFloat(patch.dollarZeroStr() + "-lowpass", 100);
	pd.sendFloat(patch.dollarZeroStr() + "-volume", 0.75);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(50, 50, 50);
        ofDrawRectangle(10, 10, 320, 240);
	label_1.draw();
	label_2.draw();
	label_3.draw();
	label_4.draw();
	label_5.draw();
	label_7.draw();
	hSlider_1.draw();
	hSlider_2.draw();
	hSlider_3.draw();
	toggle_1.draw();
	bang_1.draw();
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

//--------------------------------------------------------------
void ofApp::print(const std::string & message) {

}

//--------------------------------------------------------------
void ofApp::receiveBang(const std::string & dest) {

}

void ofApp::receiveFloat(const std::string & dest, float value) {

}

void ofApp::receiveSymbol(const std::string & dest, const std::string & symbol) {

}

void ofApp::receiveList(const std::string & dest, const pd::List & list) {

}

void ofApp::receiveMessage(const std::string & dest, const std::string & msg, const pd::List & list) {

}
