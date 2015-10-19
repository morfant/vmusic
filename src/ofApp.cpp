#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	frameByframe = false;
    
	// Uncomment this to show movies with alpha channels
	// fingerMovie.setPixelFormat(OF_PIXELS_RGBA);
    
	movie.loadMovie("movies/test.mov");
	movie.play();
    
    movWidth = movie.getWidth();
    movHeight = movie.getHeight();
    
//    movie.nextFrame();
//    movie.update();
    
    cout << "total frame: " << movie.getTotalNumFrames() << endl;
    
    

    // blob grabber
	threshold = 32;
    
    cvImgColor.allocate(movWidth, movHeight);
	cvImgGrayscale.allocate(movWidth, movHeight);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetLogLevel(OF_LOG_VERBOSE);
    
//    cout << "cur frame: " << movie.getCurrentFrame() << endl;
//    cout << "cur position: " << movie.getPosition() << endl;

    
    movie.update();
    
    if(movie.isFrameNew()) {
		movie.update();
		cvImgColor.setFromPixels(movie.getPixels(), movWidth, movHeight);
		cvImgGrayscale.setFromColorImage(cvImgColor);
		cvImgGrayscale.threshold(threshold, ofGetKeyPressed());
		contourFinder.findContours(cvImgGrayscale, 64 * 64, movWidth * movHeight, 5, false, true);
		
		polylines.clear();
		smoothed.clear();
		resampled.clear();
		boundingBoxes.clear();
		closestPoints.clear();
		closestIndices.clear();
		
		ofPoint target;
		target.set(mouseX, mouseY);
        
		for(unsigned int i = 0; i < contourFinder.blobs.size(); i++) {
			ofPolyline cur;
			// add all the current vertices to cur polyline
			cur.addVertices(contourFinder.blobs[i].pts);
			cur.setClosed(true);
			
			// add the cur polyline to all these vector<ofPolyline>
			polylines.push_back(cur);
			smoothed.push_back(cur.getSmoothed(8));
			resampled.push_back(cur.getResampledByCount(100).getSmoothed(8));
			
			boundingBoxes.push_back(cur.getBoundingBox());
			
			unsigned int index;
			closestPoints.push_back(resampled.back().getClosestPoint(target, &index));
			closestIndices.push_back(index);
		}
	}
}

//--------------------------------------------------------------
void drawWithNormals(const ofPolyline& polyline) {
	for(int i=0; i< (int) polyline.size(); i++ ) {
		bool repeatNext = i == (int)polyline.size() - 1;
	
		const ofPoint& cur = polyline[i];
		const ofPoint& next = repeatNext ? polyline[0] : polyline[i + 1];
		
		float angle = atan2f(next.y - cur.y, next.x - cur.x) * RAD_TO_DEG;
		float distance = cur.distance(next);
	
		if(repeatNext) {
			ofSetColor(255, 0, 255);
		}
		glPushMatrix();
		glTranslatef(cur.x, cur.y, 0);
		ofRotate(angle);
		ofLine(0, 0, 0, distance);
		ofLine(0, 0, distance, 0);
		glPopMatrix();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);
	
	ofSetColor(255);
	cvImgGrayscale.draw(0, 0);
    
	for(unsigned int i = 0; i < polylines.size(); i++) {
		ofNoFill();
	
		ofSetColor(255, 0, 0);
		drawWithNormals(polylines[i]);
		
		ofSetColor(0, 255, 0);
		drawWithNormals(smoothed[i]);
		
		ofSetColor(0, 0, 255);
		drawWithNormals(resampled[i]);
		
		ofSetColor(0, 255, 255);
		ofRect(boundingBoxes[i]);
		
		ofSetColor(255, 0, 0);
		ofLine(closestPoints[i], ofPoint(mouseX, mouseY));
		ofSetColor(0, 0, 255);
		ofLine(resampled[i][closestIndices[i]], ofPoint(mouseX, mouseY));
	}
	
	ofSetColor(255, 0, 0);	
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
	ofDrawBitmapString("Click and drag to set a new threshold.", 10, 40);
	ofDrawBitmapString("Hold down any key to invert the thresholding.", 10, 60);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	threshold = x;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
