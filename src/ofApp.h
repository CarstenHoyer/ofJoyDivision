#pragma once

#include "ofMain.h"
#include "ofxClipper.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp, public ofxMidiListener {

public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    ofxClipperClipType currentClipperType;
    bool noDraw;
    vector<ofPolyline> lines;
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10;
    vector<ofPolyline> clipped;
    bool capture;
    
private:
    vector<vector<ofPoint>> getLinePoints();
    vector<ofPolyline> getLines(vector<vector<ofPoint>> linePoints);
    vector<ofPolyline> clip(vector<ofPolyline> lines);
    int variance;
    int step;
    int granularity;
		
};
