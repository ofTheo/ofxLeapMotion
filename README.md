ofxLeapMotion
=========

Copyright (c) 2012-2013 ofxLeapMotion Team

MIT License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See https://github.com/ofTheo/ofxLeapMotion for documentation as well as the [OF forums](http://forum.openframeworks.cc/index.php).

Description
-----------

ofxLeapMotion is an Open Frameworks addon for the LeapMotion that runs on Mac OSX.
Support for Windows and Linux is coming soon. 

Installation
------------

* Use of this addon requires a Developer License from LeapMotion see: https://www.leapmotion.com/developers

* Once registered you should download and install the latest Leap Software from https://beta.leapmotion.com/ 

* To use ofxLeapMotion, first you need to download and install [openFrameworks](https://github.com/openframeworks/openFrameworks).

* To get a copy of the repository you can download the source from [http://github.com/ofTheo/ofxLeapMotion](http://github.com/ofTheo/ofxLeapMotion/) or, alternatively, you can use git clone:

`git clone git://github.com/ofTheo/ofxLeapMotion.git`

* The addon should sit in `openFrameworks/addons/ofxLeapMotion/`.


Running the Example Project
---------------------------

### OSX

Xcode4: Open the Xcode project, select the "leapMotionExample Debug" scheme, and hit "Run".


How to Create a New ofxLeapMotion Project
-------------------------------------

To develop your own project based on ofxLeapMotion, simply copy the example project and rename it. You probably want to put it in your apps folder, for example, after copying:

`openFrameworks/addons/ofxLeapMotion/example/ => openFrameworks/apps/myApps/example/`

Then after renaming:

`openFrameworks/apps/myApps/myLeapProject/`

### Mac (Xcode):

Rename the project in Xcode (do not rename the .xcodeproj file in Finder!): Slow double click the project name and rename (Xcode4)


Adding ofxLeapMotion to an Existing Project
---------------------------------------


### Mac (Xcode):

In the Xcode project browser:

* add the ofxLeapMotion sources to the project:
  * create a new group "ofxLeapMotion"
  * drag these directories from ofxLeapMotion into this new group: ofxLeapMotion/src & ofxLeapMotion/libs

* IMPORTANT! 

* On OS X, you must have this in the Run Script Build Phase of your Xcode project. 
where the first path ../../../addons/ofxLeapMotion/ is the path to the ofxLeapMotion addon. 

```
cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
```

   If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib



Developing ofxLeapMotion
--------------------

Feel free to log bug reports and issues to the ofxLeapMotion Github page: https://github.com/ofTheo/ofxLeapMotion/issues

