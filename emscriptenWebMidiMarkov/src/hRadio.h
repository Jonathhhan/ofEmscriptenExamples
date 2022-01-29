#ifndef _HRADIO
#define _HRADIO

#include "ofMain.h"

class hRadio{

    public:
    hRadio();
    ~hRadio();

    void setup(float xpos, float ypos, float size, float number);
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
        
    float xpos;
    float ypos;
    float size;
    float number;
    bool isTouch;
    float value;
    ofEvent<float> onMousePressed;
    
    protected:
    bool bRegisteredEvents;
};
#endif
