#ifndef _NUMBER
#define _NUMBER

#include "ofMain.h"

class number {

    public:
    number();
    ~number();

    void setup(float xpos, float ypos, float width, float height, float minVal, float maxVal);
    void update();
    void draw();
    void clear();
    
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    void mouseEntered(ofMouseEventArgs & args);
    void mouseExited(ofMouseEventArgs & args);
    
    void touchDown(ofTouchEventArgs & args);
    void touchMoved(ofTouchEventArgs & args);
    void touchUp(ofTouchEventArgs & args);
    void touchDoubleTap(ofTouchEventArgs & args);
    void touchCancelled(ofTouchEventArgs & args);
    
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
    
    bool decimalPlace;
float multiply;
float key;
float previewValue;
bool preview;
float count;
float now;
float previousMouseY;

    ofEvent<float> onMousePressed;
    
    protected:
    bool bRegisteredEvents;
};
#endif
