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

		void debugDraw();
};

class ofxLeapMotion : public Listener{
	
	public:
    
		// TODO: adding leap gesture support - JRW
		int iGestures;
		
		// swipe data
		float swipeSpeed;
		float swipeDurationSeconds;
		int64_t swipeDurationMicros;
		
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

		ofxLeapMotion();
		~ofxLeapMotion();

		void open();
		void reset();
		void close();

		// TODO: adding leap gesture support - JRW
		void setupGestures();
		void updateGestures();
		
		// Leap event callbacks, inherit and override these to handle events
		virtual void onInit(const Controller& controller);
		virtual void onConnect(const Controller& contr);
		virtual void onDisconnect(const Controller& contr);
		virtual void onExit(const Controller& contr);
		
		//if you want to use the Leap Controller directly - inhereit ofxLeapMotion and implement this function
		//note: this function is called in a seperate thread - so GL commands here will cause the app to crash.
		//also: call onFrameInternal(contr) here if you want to use getHands() / isFrameNew() etc
		virtual void onFrame(const Controller& contr);
		
		virtual void onFocusGained(const Controller& contr);
		virtual void onFocusLost(const Controller& contr);
		virtual void onServiceConnect(const Controller& contr);
		virtual void onServiceDisconnect(const Controller& contr);
		virtual void onDeviceChange(const Controller& contr);
		
		//Simple access to the hands
		vector <Hand> getLeapHands();
		vector <ofxLeapMotionSimpleHand> getSimpleHands();

		bool isConnected();

		void setReceiveBackgroundFrames(bool bReceiveBg);

		bool isFrameNew();
		
		void markFrameAsOld();
		
		int64_t getCurrentFrameID();

		void resetMapping();
		
		void setMappingX(float minX, float maxX, float outputMinX, float outputMaxX);
		void setMappingY(float minY, float maxY, float outputMinY, float outputMaxY);
		void setMappingZ(float minZ, float maxZ, float outputMinZ, float outputMaxZ);
		
		//helper function for converting a Leap::Vector to an ofPoint with a mapping
		ofPoint getMappedofPoint(Vector v);
		
		//helper function for converting a Leap::Vector to an ofPoint
		ofPoint getofPoint(Vector v);

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
