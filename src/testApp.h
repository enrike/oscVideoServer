#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
// addons
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

#include <vector> // for stacks

// mine
#include "oscVPlayer.h"
#include "oscVGrabber.h"
#include "oscImage.h"




// listen on port
//#define PORT 9000
//#define NUMOFLAYERS 10


class testApp : public ofBaseApp
{
    public:
        testApp();
        ~testApp();
        void clearAll();

    private:
        // OSC
        ofxOscReceiver	receiver;
        ofxOscSender    sender;
        bool connected;

        ofxXmlSettings XML;

		string msg_string; // to print error on verbose
		int verbose, numoflayers, reportinput;
		//bool  ;

        bool isGrabberInit;
        ofVideoGrabber vidGrabber;

        //vector<ofVideoGrabber> videoDevices; //VIDEO DEVICES
        vector<oscVGrabber> grabbers;   // video device renderers
        vector<oscVPlayer>  players;    // video files players
        vector<oscImage>    images;     // image files

        // OPENCV //
        ofxCvColorImage		    colorImg;
        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;
        ofxCvContourFinder 	    contourFinder;
		int threshold;
		bool bLearnBakground, trackOpenCV, plotblobs;

		// snapshot stuff // delete or comment out!
//        bool takeShot;
//		ofImage shotImg;
//		int shotTimeOut;
//		int shotLength;


		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void loadMedia(); // reading XML
		bool initGrabberDevice(int id);



        void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
};

#endif
