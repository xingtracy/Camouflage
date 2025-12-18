#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetWindowTitle("Face Filter App");
    ofSetVerticalSync(true);
    
    // Setup camera
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(640, 480);
    
    // Allocate images
    colorImg.allocate(640, 480);
    grayImg.allocate(640, 480);
    
    // Setup face detector
    // Note: You need to copy haarcascade file to bin/data/
    faceFinder.setup("haarcascade_frontalface_default.xml");
    faceFinder.setPreset(ofxCv::ObjectFinder::Fast);
    faceFinder.getTracker().setPersistence(15);
    
    // Initial settings
    currentEffect = BLUR;
    showFaces = true;
    facesDetected = 0;
    
    // Copy cascade file if needed
    if (!ofFile::doesFileExist("haarcascade_frontalface_default.xml")) {
        ofFile::copyFromTo(ofToDataPath("../../../../../addons/ofxOpenCv/libs/opencv/etc/haarcascades/haarcascade_frontalface_default.xml", true),
                          "haarcascade_frontalface_default.xml");
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew()) {
        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImg = colorImg;
        
        // Detect faces
        faceFinder.update(grayImg);
        
        // Apply effects to detected faces
        for (int i = 0; i < faceFinder.size(); i++) {
            applyEffect(faceFinder.getObjectSmoothed(i));
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    
    // Draw camera feed
    colorImg.draw(0, 0);
    
    // Draw face rectangles if enabled
    if (showFaces) {
        ofNoFill();
        ofSetColor(ofColor::green);
        for (int i = 0; i < faceFinder.size(); i++) {
            ofRectangle rect = faceFinder.getObjectSmoothed(i);
            ofDrawRectangle(rect);
        }
    }
    
    // Draw UI
    ofSetColor(ofColor::white);
    ofDrawBitmapString("Faces: " + ofToString(faceFinder.size()), 10, 20);
    
    // Draw effect info
    string effectName;
    switch(currentEffect) {
        case BLUR: effectName = "BLUR"; break;
        case PIXELATE: effectName = "PIXELATE"; break;
        case GRAYSCALE: effectName = "GRAYSCALE"; break;
        case INVERT: effectName = "INVERT"; break;
        case NONE: effectName = "NO EFFECT"; break;
    }
    ofDrawBitmapString("Effect: " + effectName, 10, 40);
    
    // Draw instructions
    ofDrawBitmapString("Press 1-5 to change effects", 10, ofGetHeight() - 60);
    ofDrawBitmapString("Press F to toggle face rectangles", 10, ofGetHeight() - 40);
    ofDrawBitmapString("Press SPACE to save screenshot", 10, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::applyEffect(ofRectangle faceRect) {
    // Extract face region
    ofPixels pixels = colorImg.getPixels();
    int x = faceRect.x;
    int y = faceRect.y;
    int w = faceRect.width;
    int h = faceRect.height;
    
    // Apply effect based on current selection
    switch(currentEffect) {
        case BLUR:
            applyBlur(faceRect);
            break;
        case PIXELATE:
            applyPixelate(faceRect);
            break;
        case GRAYSCALE:
            applyGrayscale(faceRect);
            break;
        case INVERT:
            applyInvert(faceRect);
            break;
        case NONE:
            // Do nothing
            break;
    }
}

//--------------------------------------------------------------
void ofApp::applyBlur(ofRectangle faceRect) {
    // Simple box blur effect
    ofPixels& pixels = colorImg.getPixels();
    int blurRadius = 15;
    
    // Create a copy for reading
    ofPixels originalPixels = pixels;
    
    for (int y = faceRect.y; y < faceRect.y + faceRect.height; y++) {
        for (int x = faceRect.x; x < faceRect.x + faceRect.width; x++) {
            int r = 0, g = 0, b = 0;
            int count = 0;
            
            // Average surrounding pixels
            for (int dy = -blurRadius; dy <= blurRadius; dy++) {
                for (int dx = -blurRadius; dx <= blurRadius; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= faceRect.x && nx < faceRect.x + faceRect.width &&
                        ny >= faceRect.y && ny < faceRect.y + faceRect.height) {
                        ofColor c = originalPixels.getColor(nx, ny);
                        r += c.r;
                        g += c.g;
                        b += c.b;
                        count++;
                    }
                }
            }
            
            if (count > 0) {
                pixels.setColor(x, y, ofColor(r / count, g / count, b / count));
            }
        }
    }
    colorImg.setFromPixels(pixels);
}

//--------------------------------------------------------------
void ofApp::applyPixelate(ofRectangle faceRect) {
    // Simple pixelation effect
    int pixelSize = 10;
    ofPixels& pixels = colorImg.getPixels();
    
    for (int y = faceRect.y; y < faceRect.y + faceRect.height; y += pixelSize) {
        for (int x = faceRect.x; x < faceRect.x + faceRect.width; x += pixelSize) {
            ofColor color = pixels.getColor(x, y);
            for (int py = 0; py < pixelSize && y + py < faceRect.y + faceRect.height; py++) {
                for (int px = 0; px < pixelSize && x + px < faceRect.x + faceRect.width; px++) {
                    pixels.setColor(x + px, y + py, color);
                }
            }
        }
    }
    colorImg.setFromPixels(pixels);
}

//--------------------------------------------------------------
void ofApp::applyGrayscale(ofRectangle faceRect) {
    ofPixels& pixels = colorImg.getPixels();
    
    for (int y = faceRect.y; y < faceRect.y + faceRect.height; y++) {
        for (int x = faceRect.x; x < faceRect.x + faceRect.width; x++) {
            ofColor color = pixels.getColor(x, y);
            float gray = color.getBrightness();
            pixels.setColor(x, y, ofColor(gray));
        }
    }
    colorImg.setFromPixels(pixels);
}

//--------------------------------------------------------------
void ofApp::applyInvert(ofRectangle faceRect) {
    ofPixels& pixels = colorImg.getPixels();
    
    for (int y = faceRect.y; y < faceRect.y + faceRect.height; y++) {
        for (int x = faceRect.x; x < faceRect.x + faceRect.width; x++) {
            ofColor color = pixels.getColor(x, y);
            pixels.setColor(x, y, ofColor(255 - color.r, 255 - color.g, 255 - color.b));
        }
    }
    colorImg.setFromPixels(pixels);
}


//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case '1':
            currentEffect = BLUR;
            break;
        case '2':
            currentEffect = PIXELATE;
            break;
        case '3':
            currentEffect = GRAYSCALE;
            break;
        case '4':
            currentEffect = INVERT;
            break;
        case '5':
            currentEffect = NONE;
            break;
        case 'f':
        case 'F':
            showFaces = !showFaces;
            break;
        case ' ':
            // Save screenshot
            ofImage screenshot;
            screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            screenshot.save("screenshot_" + ofGetTimestampString() + ".png");
            break;
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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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
