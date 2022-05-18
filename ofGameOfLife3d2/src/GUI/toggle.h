#include "ofMain.h"

class toggle {
    public:

    void setup(float xpos, float ypos, float size);
    void draw();

    void mousePressed(ofMouseEventArgs & args);
    void touchDown(ofTouchEventArgs & args);
        
    float xpos;
    float ypos;
    float size;
    bool isTouch;
    bool value;
};