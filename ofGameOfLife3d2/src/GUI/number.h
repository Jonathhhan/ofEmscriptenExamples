#include "ofMain.h"

class number {
    public:

    void setup(float xpos, float ypos, float width, float height, float minVal, float maxVal);
    void draw();

    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void touchDown(ofTouchEventArgs & args);
    void touchMoved(ofTouchEventArgs & args);
    void keyPressed(ofKeyEventArgs & args);
    void keyReleased(ofKeyEventArgs & args);
        
    float xpos;
    float ypos;
    float width;
    float height;
    float minVal;
    float maxVal;
    float value;
    bool mouseState;
    bool isTouch;

    float key;
    float previewValue;
    bool preview;
    float count;
    float now;
    float previousMouseY;
};