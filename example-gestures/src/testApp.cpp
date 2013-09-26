#include "testApp.h"

/* Note on OS X, you must have this in the Run Script Build Phase of your project. 
where the first path ../../../addons/ofxLeapMotion/ is the path to the ofxLeapMotion addon. 

cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";

   If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
*/

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	leap.open();
	leap.setupGestures();   // we enable our gesture detection here
}


//--------------------------------------------------------------
void testApp::update()
{
	leap.updateGestures();  // check for gesture updates
	leap.markFrameAsOld();	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new. 
}

//--------------------------------------------------------------
void testApp::draw()
{
     /* Leap iGesture Key
     --------------------------------
     1 = Screen Tap
     2 = Key Tap
     3 = Swipe Right
     4 = Swipe Left
     5 = Swipe Down
     6 = Swipe Up
     7 = Swipe Forward
     8 = Swipe Backward (towards yourself)
     9 = Circle Left (counter-clockwise)
     10 = Circle Right (clockwise)
     --------------------------------
     */
    
    string msg;
    
    switch (leap.iGestures)
    {
        case 1:
            msg = "Screen Tap";
            break;
            
        case 2:
            msg = "Key Tap";
            break;
            
        case 3:
            msg = "Swipe Right";
            break;
            
        case 4:
            msg = "Swipe Left";
            break;
            
        case 5:
            msg = "Swipe Down";
            break;
            
        case 6:
            msg = "Swipe Up";
            break;
            
        case 7:
            msg = "Swipe Forward";
            break;
            
        case 8:
            msg = "Swipe Backwards";
            break;
            
        case 9:
            msg = "Circle Left";
            break;
            
        case 10:
            msg = "Circle Right";
            break;
            
        default:
            msg = "Waiting for hand movement...";
            break;
    }
    
    ofDrawBitmapString(msg, 20, 20);  // let's draw our message to the screen
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

//--------------------------------------------------------------
void testApp::exit()
{
    leap.close();  // let's close down Leap and kill the controller
}
