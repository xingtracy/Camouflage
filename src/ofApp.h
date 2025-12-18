#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void gotMessage(ofMessage msg);
		void dragEvent(ofDragInfo dragInfo);

		// Camera
		ofVideoGrabber vidGrabber;
		ofxCvColorImage colorImg;
		ofxCvGrayscaleImage grayImg;

		// Cascade Classifier
		ofxCv::ObjectFinder faceFinder;

		enum EffectType{
			BLUR,
			PIXELATE,
			GRAYSCALE,
			INVERT,
			NONE
		};

		EffectType currentEffect;

		// UI
		bool showFaces;
		int facesDetected;


	private:

		void applyEffect(ofRectangle faceRect);
		void applyBlur(ofRectangle faceRect);
		void applyPixelate(ofRectangle faceRect);
		void applyGrayscale(ofRectangle faceRect);
		void applyInvert(ofRectangle faceRect);
		
};
