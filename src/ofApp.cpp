#include "ofApp.h"
#include "ofxCv.h"

//--------------------------------------------------------------
void ofApp::setup(){
    video.load("video.mp4");
    video.setLoopState(OF_LOOP_NORMAL);
    video.play();

    scaleFactor = 0.5;

    scaledWidth = ((float) video.getWidth()) * scaleFactor;
    scaledHeight = ((float) video.getHeight()) * scaleFactor;
}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();

    auto videoPixels = video.getPixels();
    auto imgMat = ofxCv::toCv(videoPixels);

    // Our analysis goes here.
    cv::cvtColor(imgMat, imgMat, CV_BGR2GRAY);
    cv::resize(imgMat, imgMat, cv::Size(scaledWidth, scaledHeight));
    cv::GaussianBlur(imgMat, imgMat, cv::Point(25, 25), 0);
    if (framesCount < 10) {
        imgMat.copyTo(firstFrame);
        framesCount++;
    }
    cv::absdiff(firstFrame, imgMat, imgMat);
    const int thresh = 25; // <--- Just experiment
    cv::threshold(imgMat, imgMat, thresh, 255, cv::THRESH_BINARY);
    cv::dilate(imgMat, imgMat, {}, cv::Point(-1, -1), 2);

    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::Mat forContours;
    imgMat.copyTo(forContours);
    cv::findContours(forContours, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    boundingBoxes.clear();
    for (auto contour: contours) {
        if (cv::contourArea(contour) < 500) {
            continue;
        }
        boundingBoxes.push_back(cv::boundingRect(cv::Mat(contour)));
    }

    ofxCv::toOf(imgMat, analysisPixels);
    analysisTexture.loadData(analysisPixels);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xFFFFFF);
    video.draw(0, 0);
//    analysisTexture.draw(0, 0);

    ofSetHexColor(0x00FF00);
    ofNoFill();
    ofSetLineWidth(2);
    for (auto boundingBox : boundingBoxes) {
        int x = ((float) boundingBox.x) / scaleFactor;
        int y = ((float) boundingBox.y) / scaleFactor;
        int width = ((float) boundingBox.width) / scaleFactor;
        int height = ((float) boundingBox.height) / scaleFactor;
        ofDrawRectangle(x, y, width, height);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
