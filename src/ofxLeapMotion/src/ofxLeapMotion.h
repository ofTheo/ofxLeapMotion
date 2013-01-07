//ofxLeapMotion - Written by Theo Watson - http://theowatson.com
//Work in progress lightweight wrapper for Leap Motion SDK 
//Simple interface to start with. Subject to change. 

#pragma once

#include "ofMain.h"
#include "Leap.h"
#include "Poco/Mutex.h"

using namespace Leap;

class ofxLeapMotion : public Listener{
	public:
	
		ofxLeapMotion(){
			currentFrameID = 0;
			preFrameId = -1;
			
			xOffsetIn = 0;
			yOffsetIn = 0;
			zOffsetIn = 0;

			xOffsetOut = 0;
			yOffsetOut = 0;
			zOffsetOut = 0;

			xScale = 1;
			yScale = 1;
			zScale = 1;
			
			ourController = new Leap::Controller(); 
		}

		~ofxLeapMotion(){
			//note we don't delete the controller as it causes a crash / mutex exception. 
			close();
		}
		
		void close(){
			if( ourController ){
				ourController->removeListener(*this);
			}
		}

		//--------------------------------------------------------------
		void open(){
			ourController->addListener(*this);
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
		
		//if you want to use the Leap Controller directly - inhereit ofxLeapMotion and implement this function
		//note: this function is called in a seperate thread - so GL commands here will cause the app to crash. 
		//-------------------------------------------------------------- 
		virtual void onFrame(const Controller& contr){
			ofLogVerbose("ofxLeapMotionApp - onFrame"); 

			onFrameInternal(contr); // call this if you want to use getHands() / isFrameNew() etc 
		}
		
		//Simple access to the hands 
		//-------------------------------------------------------------- 
		vector <Hand> getHands(){
		
			vector <Hand> handsCopy; 
			if( ourMutex.tryLock(2000) ){
				handsCopy = hands; 
				ourMutex.unlock();
			}

			return handsCopy;
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
			
			Poco::FastMutex ourMutex;
};


