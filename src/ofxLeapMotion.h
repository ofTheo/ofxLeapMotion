//ofxLeapMotion - Written by Theo Watson - http://theowatson.com
//Work in progress lightweight wrapper for Leap Motion SDK 
//Simple interface to start with. Subject to change. 

#pragma once

#include "ofMain.h"
#include "Leap.h"
#include "Poco/Mutex.h"

using namespace Leap;

class ofxLeapMotionSimpleHand{
    public:
    
    typedef struct{
        ofPoint pos;
        ofPoint vel;
        ofPoint base;                   // finger's base
        int64_t id;
    }simpleFinger;
    
    vector <simpleFinger>  fingers;
    
    ofPoint handPos; 
    ofPoint handNormal;
                                        
    ofPoint handVelocity;               // palm vel
    ofPoint sphereCenter;               // palm and fingers sphere position
    float sphereRadius;                 // and radius for hand openness

    
    void debugDraw(){
        ofPushStyle();
        
            ofSetColor(190);
            ofSetLineWidth(2);

            ofEnableLighting();
            ofPushMatrix();
                ofTranslate(handPos);
                //rotate the hand by the downwards normal
                ofQuaternion q;
                q.makeRotate(ofPoint(0, -1, 0), handNormal);
                ofMatrix4x4 m;
                q.get(m);
                glMultMatrixf(m.getPtr());
                
                
                //scale it to make it not a box
                ofScale(1, 0.35, 1.0);
#if (OF_VERSION_MAJOR == 0) && (OF_VERSION_MINOR < 8)
                ofBox(0, 0, 0, 60);
#else
                ofDrawBox(0, 0, 0, 60);
#endif
            ofPopMatrix();
        
            // sphere - hand openness debug draw
            ofSetColor(200, 0, 0, 80);
            ofDrawSphere(sphereCenter, sphereRadius);
        
            for(int i = 0; i < fingers.size(); i++){
                //ofDrawArrow(handPos, fingers[i].pos, 10);
            
                // fingers base debug draw
                ofSetColor(190);
                ofLine(handPos, fingers[i].base);
                ofDrawBox(fingers[i].base, 20);
                ofLine(fingers[i].base, fingers[i].pos);
                
                ofSetColor(0, 200, 0);
                ofDrawSphere(fingers[i].pos, 20);

            }
            
            ofSetColor(220, 220, 0);
            for(int i = 0; i < fingers.size(); i++){
                ofDrawArrow(fingers[i].pos + fingers[i].vel/20, fingers[i].pos + fingers[i].vel/10, 10);
            }
        
            ofDisableLighting();
        
        ofPopStyle();
    }
};

class ofxLeapMotion : public Listener{
	public:
    
    // TODO: adding leap gesture support - JRW
    int iGestures;
    
    // swipe data
    float swipeSpeed = 0.0;
    float swipeDurationSeconds = 0.0;
    int64_t swipeDurationMicros = 0.0;
    
    // circle data
    float circleProgress;
    float circleRadius;
    ofPoint circleCenter;
    ofVec3f circleNormal;
    
    //key tap
    ofPoint keyTapPosition;  
    
    // screen tap
    ofPoint  screenTapPosition;
    ofVec3f screenTapDirection;
    
    // TODO: hands + pointables list, id's, global gesture pos? - rux

		ofxLeapMotion(){
            reset();
            resetMapping();
			ourController = new Leap::Controller(); 
		}
        
        void reset(){
            currentFrameID = 0;
            preFrameId = -1;
        }

		~ofxLeapMotion(){
			//note we don't delete the controller as it causes a crash / mutex exception. 
			/// close(); /// JRW - we do not need this...
                         /// JRW - seems fine in this demo but, when I add
                         /// JRW - threaded objects the Leap controller crashes on exit.
		}
		
		void close(){
			if( ourController ){
				ourController->removeListener(*this);
			}
			
			/// JRW - let's delete our Leap controller
			/// call close() on app exit
			delete ourController;
		}

		//--------------------------------------------------------------
		void open(){
            reset();
			ourController->addListener(*this);
		}
    
        // TODO: adding leap gesture support - JRW
        //--------------------------------------------------------------
        void setupGestures(){
            // enables screen tap gesture (forward poke / tap)
            ourController->enableGesture(Gesture::TYPE_SCREEN_TAP);
            
            // enables key tap gesture (down tap)
            ourController->enableGesture(Gesture::TYPE_KEY_TAP);
            
            // enables swipe gesture
            ourController->enableGesture(Gesture::TYPE_SWIPE);
            
            // enables circle gesture
            ourController->enableGesture(Gesture::TYPE_CIRCLE);
        }
    
        // TODO: adding leap gesture support - JRW
        //--------------------------------------------------------------
        void updateGestures(){
            
            Leap::Frame frame = ourController->frame();
            
            if (lastFrame == frame) {
                return;
            }
            
            Leap::GestureList gestures = lastFrame.isValid()    ?
            frame.gestures(lastFrame) :
            frame.gestures();
            
            lastFrame = frame;
            
            size_t numGestures = gestures.count();
            
            for (size_t i=0; i < numGestures; i++) {
                
                // screen tap gesture (forward poke / tap)
                if (gestures[i].type() == Leap::Gesture::TYPE_SCREEN_TAP) {
                    Leap::ScreenTapGesture tap = gestures[i];

                    screenTapPosition = getMappedofPoint(tap.position());   // screen tap gesture data = tap position
                    screenTapDirection = getofPoint(tap.direction());       // screen tap gesture data = tap direction

                    iGestures = 1;
        
                }
                
                // key tap gesture (down tap)
                else if (gestures[i].type() == Leap::Gesture::TYPE_KEY_TAP) {
                    Leap::KeyTapGesture tap = gestures[i];

                    keyTapPosition = getofPoint(tap.position());            // key tap gesture data = tap position

                    iGestures = 2;
                    
                }
                
                // swipe gesture
                else if (gestures[i].type() == Leap::Gesture::TYPE_SWIPE) {
                    Leap::SwipeGesture swipe = gestures[i];
                    Leap::Vector diff = 0.04f*(swipe.position() - swipe.startPosition());
                    ofVec3f curSwipe(diff.x, -diff.y, diff.z);
                    
                    // swipe left
                    if (curSwipe.x < -3 && curSwipe.x > -20) {
                        iGestures = 4;
                    }
                    // swipe right
                    else if (curSwipe.x > 3 && curSwipe.x < 20) {
                        iGestures = 3;
                    }
                    // swipe up
                    if (curSwipe.y < -3 && curSwipe.y > -20) {
                        iGestures = 6;
                    }
                    // swipe down
                    else if (curSwipe.y > 3 && curSwipe.y < 20) {
                        iGestures = 5;
                    }
                    
                    // 3D swiping
                    // swipe forward
                    if (curSwipe.z < -5) {
                        iGestures = 7;
                    }
                    // swipe back
                    else if (curSwipe.z > 5) {
                        iGestures = 8;
                    }
                    
                    // more swipe gesture data
                    swipeSpeed = swipe.speed();                             // swipe speed in mm/s
                    swipeDurationSeconds = swipe.durationSeconds();         // swipe duration in seconds
                    swipeDurationMicros = swipe.duration();                 // swipe duration in micros
                    swipe.position();

                }
                
                // circle gesture
                else if (gestures[i].type() == Leap::Gesture::TYPE_CIRCLE) {
                    Leap::CircleGesture circle = gestures[i];
                    circleProgress = circle.progress();                     // circle progress

                    if (circleProgress >= 1.0f) {
                        
                        circleCenter = getMappedofPoint(circle.center());                           // changed to global
                        circleNormal.set(circle.normal().x, circle.normal().y, circle.normal().z);  // changed to global

                        double curAngle = 6.5;
                        if (circleNormal.z < 0) {
                            curAngle *= -1;
                        }
                        
                        if (curAngle < 0) {
                            // clockwise rotation
                            iGestures = 10;
                        }
                        else {
                            // counter-clockwise rotation
                            iGestures = 9;
                        }
                    }
                    
                }
                
                // kill gesture when done
                // gestures 5 & 6 are always in a STATE_STOP so we exclude
                if (gestures[i].type() != 5 && gestures[i].type() != 6) {
                    if (gestures[i].state() == Leap::Gesture::STATE_STOP) {
                        iGestures = 0;
                    }
                }
            }
        }
    
		//--------------------------------------------------------------
		virtual void onInit(const Controller& controller){
			ofLogVerbose("ofxLeapMotionApp - onInit"); 
		}

		//--------------------------------------------------------------
		virtual void onConnect(const Controller& contr){
			ofLogWarning("ofxLeapMotionApp - onConnect"); 
		}

		//--------------------------------------------------------------
		virtual void onDisconnect(const Controller& contr){
			ofLogWarning("ofxLeapMotionApp - onDisconnect"); 
		}
    
        //--------------------------------------------------------------
        virtual void onExit(const Controller& contr){
            ofLogWarning("ofxLeapMotionApp - onExit");
        }
		
		//if you want to use the Leap Controller directly - inhereit ofxLeapMotion and implement this function
		//note: this function is called in a seperate thread - so GL commands here will cause the app to crash. 
		//-------------------------------------------------------------- 
		virtual void onFrame(const Controller& contr){
			ofLogVerbose("ofxLeapMotionApp - onFrame");

			onFrameInternal(contr); // call this if you want to use getHands() / isFrameNew() etc 
		}
		
		//Simple access to the hands 
		//-------------------------------------------------------------- 
		vector <Hand> getLeapHands(){
		
			vector <Hand> handsCopy; 
			if( ourMutex.tryLock(2000) ){
				handsCopy = hands; 
				ourMutex.unlock();
			}

			return handsCopy;
		}

		//-------------------------------------------------------------- 
		vector <ofxLeapMotionSimpleHand> getSimpleHands(){
		
			vector <ofxLeapMotionSimpleHand> simpleHands; 
			vector <Hand> leapHands = getLeapHands();
            
            for(int i = 0; i < leapHands.size(); i++){
                ofxLeapMotionSimpleHand curHand;
            
                curHand.handPos     = getMappedofPoint( leapHands[i].palmPosition() );
                curHand.handNormal  = getofPoint( leapHands[i].palmNormal() );
                curHand.handVelocity = getofPoint( leapHands[i].palmVelocity() );           //  more hand data - hand velocity
                curHand.sphereRadius = leapHands[i].sphereRadius();                         //  more hand data - hand openness
                curHand.sphereCenter = getMappedofPoint( leapHands[i].sphereCenter() );     //  more hand data - sphere center


                for(int j = 0; j < leapHands[i].fingers().count(); j++){
                    const Finger & finger = hands[i].fingers()[j];
                
                    Leap::Vector basePosition = -finger.direction() * finger.length();      //  calculate finger base position
                    basePosition += finger.tipPosition();                                   //  calculate finger base position

                    ofxLeapMotionSimpleHand::simpleFinger f;
                    f.pos = getMappedofPoint( finger.tipPosition() );
                    f.vel = getMappedofPoint(finger.tipVelocity());
                    f.base = getMappedofPoint(basePosition);                                //
                    f.id = finger.id();
                    
                    curHand.fingers.push_back( f );                    
                }
                
                simpleHands.push_back( curHand ); 
            }

			return simpleHands;
		}
    
        //--------------------------------------------------------------
        bool isConnected() {
            return (ourController && ourController->isConnected());
        }
        
        //--------------------------------------------------------------
        void setReceiveBackgroundFrames(bool bReceiveBg) {
            if (ourController) {
                ourController->setPolicyFlags(bReceiveBg? Leap::Controller::POLICY_BACKGROUND_FRAMES : Leap::Controller::POLICY_DEFAULT);
            }
        }

		//-------------------------------------------------------------- 
		bool isFrameNew(){
			return currentFrameID != preFrameId;
		}

		//-------------------------------------------------------------- 
		void markFrameAsOld(){
			preFrameId = currentFrameID; 
		}
		
		//-------------------------------------------------------------- 
		int64_t getCurrentFrameID(){
			return currentFrameID;
		}
        
		//-------------------------------------------------------------- 
        void resetMapping(){
            xOffsetIn = 0;
			yOffsetIn = 0;
			zOffsetIn = 0;

			xOffsetOut = 0;
			yOffsetOut = 0;
			zOffsetOut = 0;

			xScale = 1;
			yScale = 1;
			zScale = 1;
        }
		
		//-------------------------------------------------------------- 
		void setMappingX(float minX, float maxX, float outputMinX, float outputMaxX){
			xOffsetIn	= minX;
			xOffsetOut	= outputMinX;			 
			xScale  =   ( outputMaxX - outputMinX ) / ( maxX - minX ); 
		}
		
		//-------------------------------------------------------------- 
		void setMappingY(float minY, float maxY, float outputMinY, float outputMaxY){
			yOffsetIn	= minY;
			yOffsetOut	= outputMinY;			 
			yScale  =   ( outputMaxY - outputMinY ) / ( maxY - minY ); 
		}

		//-------------------------------------------------------------- 
		void setMappingZ(float minZ, float maxZ, float outputMinZ, float outputMaxZ){
			zOffsetIn	= minZ;
			zOffsetOut	= outputMinZ;
			zScale  =   ( outputMaxZ - outputMinZ ) / ( maxZ - minZ ); 
		}
		
		//helper function for converting a Leap::Vector to an ofPoint with a mapping
		//-------------------------------------------------------------- 
		ofPoint getMappedofPoint( Vector v ){
			ofPoint p = getofPoint(v);
			p.x = xOffsetOut + (p.x - xOffsetIn) * xScale;
			p.y = yOffsetOut + (p.y - yOffsetIn) * yScale;
			p.z = zOffsetOut + (p.z - zOffsetIn) * zScale;

			return p;
		}
		
		//helper function for converting a Leap::Vector to an ofPoint
		//-------------------------------------------------------------- 
		ofPoint getofPoint(Vector v){
			return ofPoint(v.x, v.y, v.z); 
		}

		protected:
			
			//if you want to use the Leap Controller directly - inhereit ofxLeapMotion and implement this function
			//note: this function is called in a seperate thread - so GL commands here will cause the app to crash. 
			//-------------------------------------------------------------- 
			virtual void onFrameInternal(const Controller& contr){
				ourMutex.lock();
			
					const Frame & curFrame	= contr.frame();
					const HandList & handList	= curFrame.hands();

					hands.clear(); 
					for(int i = 0; i < handList.count(); i++){
						hands.push_back( handList[i] ); 
					}
									
					currentFrameID = curFrame.id();
				
				ourMutex.unlock();
			}
			
			int64_t currentFrameID;
			int64_t preFrameId;
			
			float xOffsetIn, xOffsetOut, xScale;
			float yOffsetIn, yOffsetOut, yScale;
			float zOffsetIn, zOffsetOut, zScale;
			 
			vector <Hand> hands; 
			Leap::Controller * ourController;
    
            // TODO: added for Gesture support - JRW
            Leap::Frame lastFrame;
			
			Poco::FastMutex ourMutex;
};


