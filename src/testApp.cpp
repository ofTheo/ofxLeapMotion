#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

	leap.open(); 

	l1.setPosition(200, 300, 50);
	l2.setPosition(-200, -200, 50);

	cam.setOrientation(ofPoint(-20, 0, 0));

	glEnable(GL_DEPTH_TEST);
}


//--------------------------------------------------------------
void testApp::update(){

	fingersFound.clear();
	
	//here is a simple example of getting the hands and using them to draw trails from the fingertips. 
	//the leap data is delivered in a threaded callback - so it can be easier to work with this copied hand data
	
	//if instead you want to get the data as it comes in then you can inherit ofxLeapMotion and implement the onFrame method. 
	//there you can work with the frame data directly. 
	
	vector <Hand> hands = leap.getHands(); 
	
	if( leap.isFrameNew() && hands.size() ){

		//leap returns data in mm - lets set a mapping to our world space. 
		//you can get back a mapped point by using ofxLeapMotion::getMappedofPoint with the Leap::Vector that tipPosition returns  
		leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
		leap.setMappingZ(-150, 150, -200, 200);
				
		for(int i = 0; i < hands.size(); i++){
			for(int j = 0; j < hands[i].fingers().count(); j++){
				ofPoint pt; 
			
				const Finger & finger = hands[i].fingers()[j];
				
				cout << "pos is " << finger.tipPosition() << endl; 
				
				//here we convert the Leap point to an ofPoint - with mapping of coordinates
				//if you just want the raw point - use ofxLeapMotion::getofPoint 
				pt = leap.getMappedofPoint( finger.tipPosition() );
								
				//lets get the correct trail (ofPolyline) out of our map - using the finger id as the key 
				ofPolyline & polyline = fingerTrails[finger.id()]; 
				
				//if the distance between the last point and the current point is too big - lets clear the line 
				//this stops us connecting to an old drawing
				if( polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50 ){
					polyline.clear(); 
				}
				
				//add our point to our trail
				polyline.addVertex(pt); 
				
				//store fingers seen this frame for drawing
				fingersFound.push_back(finger.id());
			}
		}
		
	
	}
	
	//tell ofxLeapMotion that the frame is no longer new. 
	leap.markFrameAsOld();
	

}

//--------------------------------------------------------------
void testApp::draw(){
	ofDisableLighting();
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
	
	ofSetColor(200);
	ofDrawBitmapString("ofxLeapMotion - Example App\nMake sure the Leap.app is running while running this example\nYou may need to restart Leap.app to get data", 20, 20);

	cam.begin();	

	ofPushMatrix();
		ofRotate(90, 0, 0, 1);
		ofSetColor(20);
		ofDrawGridPlane(800, 20, false);
	ofPopMatrix();
	
	ofEnableLighting();
	l1.enable();
	l2.enable();
	
	m1.begin(); 
	m1.setShininess(0.6);
	
	for(int i = 0; i < fingersFound.size(); i++){
		ofxStrip strip;
		int id = fingersFound[i];
		
		ofPolyline & polyline = fingerTrails[id];
		strip.generate(polyline.getVertices(), 15, ofPoint(0, 0.5, 0.5) );
		
		ofSetColor(255 - id * 15, 0, id * 25);
		strip.getMesh().draw();
	}
	
	m1.end();
	cam.end();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
