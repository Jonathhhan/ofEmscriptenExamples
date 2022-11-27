#include "ofMain.h"

class bang {
    public:

    void setup(float xpos, float ypos, float size);
    void draw();

    void mousePressed(ofMouseEventArgs & args);
    void touchDown(ofTouchEventArgs & args);
        
    float xpos;
    float ypos;
    float size;
    float now;
    bool value;
    bool isTouch;
};