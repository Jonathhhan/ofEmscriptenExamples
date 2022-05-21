#include "ofMain.h"

class hRadio {
    public:

    void setup(float xpos, float ypos, float size, float number);
    void draw();
    
    void mousePressed(ofMouseEventArgs & args); 
    void touchDown(ofTouchEventArgs & args);
        
    float xpos;
    float ypos;
    float size;
    float number;
    bool isTouch;
    float value;
    bool boolean;
};