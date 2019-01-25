#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    ofSetColor(0);
    ofEnableAntiAliasing();
    
    midiIn.openPort(0);
    midiIn.addListener(this);
    
    variance = 50;
    step = 10;
    granularity = 50;
    capture = false;
    lines = getLines(getLinePoints());
}

//--------------------------------------------------------------
vector<ofPolyline> ofApp::clip(vector<ofPolyline> lines) {
    vector<ofPolyline> polylines;
    vector<ofPolyline> subject;
    vector<ofPolyline> subjects;
    vector<ofPolyline> mask;
    
    for (auto line = lines.begin(); line != lines.end(); ++line) {
        subjects.clear();
        subjects.push_back((*line));
        for (auto next = line + 1; next != lines.end(); ++next) {
            if (!(*line).getBoundingBox().intersects((*next).getBoundingBox())) {
                continue;
            }
            vector<ofPolyline> temp;
            for (auto sub = subjects.begin(); sub != subjects.end(); ++sub) {
                if (!(*sub).getBoundingBox().intersects((*next).getBoundingBox())) {
                    temp.push_back((*sub));
                    continue;
                }
                
                (*sub).setClosed(false);
                (*next).setClosed(true);
                
                subject.clear();
                mask.clear();
                
                subject.push_back((*sub));
                mask.push_back((*next));
                
                ofxClipper clipper;
                ofxPolylines clips;
                clipper.addPolylines(subject, OFX_CLIPPER_SUBJECT);
                clipper.addPolylines(mask, OFX_CLIPPER_CLIP);
                clipper.clip(OFX_CLIPPER_DIFFERENCE, clips);
                
                for (auto j = clips.begin(); j != clips.end(); j++) {
                    temp.push_back((*j));
                }
            }
            
            subjects.clear();
            for (auto j = temp.begin(); j != temp.end(); j++) {
                subjects.push_back((*j));
            }
        }
        
        for (auto i = subjects.begin(); i != subjects.end(); ++i) {
            polylines.push_back((*i));
        }
    }
    
    return polylines;
}

//--------------------------------------------------------------
vector<vector<ofPoint>> ofApp::getLinePoints() {
    float width = ofGetWidth();
    float height = ofGetHeight();
    vector<vector<ofPoint>> lines;
    for (auto y = step; y <= height - step; y += step) {
        vector<ofPoint> points;
        for (auto x = step; x <= width - step; x += granularity) {
            float distanceToCenter = abs(x - width / 2);
            float localVariance = max((float) width / 2 - 50 - distanceToCenter, (float) 0.0);
            float random = abs(ofRandomf() * localVariance / 4) * -1;
            points.push_back(ofPoint(x, min((float) y , y + random)));
        }
        lines.push_back(points);
    }
    return lines;
}

//--------------------------------------------------------------
vector<ofPolyline> ofApp::getLines(vector<vector<ofPoint>> linePoints) {
    vector<ofPolyline> lines;
    for (int i = 0; i < linePoints.size(); ++i) {
        if (i < 15) continue;
        ofPolyline line;
        for (int j = 0; j < linePoints[i].size(); ++j) {
            ofPoint& p1 = linePoints[i][j];
            line.addVertex(p1);
            if (j < linePoints[i].size() - 1) {
                ofPoint& p2 = linePoints[i][j + 1];
                line.bezierTo(
                              p1.x + ((p2.x - p1.x) * 0.5), p1.y + abs((p2.y - p1.y) * 0.65),
                              p2.x - ((p2.x - p1.x) * 0.5), p2.y +  + abs((p2.y - p1.y) * 0.65),
                              p2.x, p2.y
                              );
            }
        }
        lines.push_back(line);
    }
    return lines;
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    // add the latest message to the message queue
    midiMessages.push_back(msg);
    
    // remove any old messages if we have too many
    while(midiMessages.size() > maxMessages) {
        midiMessages.erase(midiMessages.begin());
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::update(){
    for(unsigned int i = 0; i < midiMessages.size(); ++i) {
        ofxMidiMessage &message = midiMessages[i];
        if (message.channel == 16 && message.status == MIDI_CONTROL_CHANGE) {
            variance = message.value;
            step = 10;
            granularity = variance;
            lines = getLines(getLinePoints());
        }
    }
    midiMessages.clear();
    if (!noDraw) {
        clipped = clip(lines);
        noDraw = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (capture) {
        ofBeginSaveScreenAsSVG("/Users/carstenhoyer/Desktop/test.svg");
    }
    
    for (auto i = clipped.begin(); i != clipped.end(); ++i) {
        ofBeginShape();
        (*i).setClosed(false);
        (*i).draw();
        ofEndShape();
    }
    
    if(capture){
        ofEndSaveScreenAsSVG();
        capture = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's'){
        capture = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
