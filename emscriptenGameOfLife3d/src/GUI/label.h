#include "ofMain.h"

class label{
    public:

    void setup(float xpos, float ypos, float width, float height, std::string symbol);
    void draw();

    float xpos;
    float ypos;
    float width;
    float height;
    std::string symbol;
};