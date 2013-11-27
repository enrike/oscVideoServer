#include "oscVPlayer.h"
#include "ofMain.h"



oscVPlayer::oscVPlayer()
{
    reset();
//        w = ofGetWidth();
//    h = ofGetHeight();
//    x = 0;
//    y = 0;
//    donereported = false;
}

void oscVPlayer::reset()
{   //defaults to fullscreen
//    if ( ofGetWindowMode()== OF_FULLSCREEN )
//    {
//        w = ofGetScreenWidth(); //ofGetWidth();
//        h = ofGetScreenHeight(); //ofGetHeight();
//    }
//    else
//    {
//        w = ofGetWidth();
//        h = ofGetHeight();
//    }
    w = NULL;
    h = NULL;
    x = 0;
    y = 0;
    donereported = false;
    loopflag = OF_LOOP_NONE;
    
    col.r = 0;
    col.g = 0;
    col.b = 0;
    col.a = 255;

    if (isLoaded())
	{
        setFrame(0);
        setUseTexture(1);
        setPaused(0);
        setLoopState(OF_LOOP_NORMAL);
        setSpeed(1);
        setVolume(255);
        stop();
	}
}


void oscVPlayer::setUpVideo(string movie)
{
    if (movie != "none")
    {
        if ( loadMovie(movie) )
        {
            printf("movie loaded\n");
            //printf("loaded movie %s \n", movie);
    //        setLoopState(OF_LOOP_NORMAL); //just in case
            ofVideoPlayer::update();
            //play();
            //donereported = false;
            //printf("loop in setupvideo %i\n", loopflag);
        }
        else
        {
            printf("CANNOT load movie\n");
            //printf("CANNOT load movie %s \n", movie);
        }
    }
    else
    {
        printf("movie parameter is none, cannot load it\n");
    }
}




void oscVPlayer::setLoop()
{
    setLoopState(loopflag); // go back to your state. otherwise play resets it to loop ON
    //printf("loop in %i\n", loopflag);
}

//void oscVPlayer::setPaused(int b)
//{
//    int n = b ? true : false ;
//    ofVideoPlayer::setPaused(n);
//}
//
//void oscVPlayer::setRender(int b)
//{
//    int n = b ? true : false ;
//    ofVideoPlayer::setUseTexture(n);
//}


void oscVPlayer::resetSize()
{
    w = NULL;
    h = NULL;
}


void oscVPlayer::draw()
{
	if (isLoaded())
	{
		 if (col.a < 255) ofEnableAlphaBlending();
	    //ofSetHexColor(0xFFFFFF);
	    ofSetColor(col.r,col.g,col.b, col.a);
	    ofVideoPlayer::update();
	    if (w==NULL && h==NULL)
	    {
	        ofVideoPlayer::draw(x, y);
	    }
	    else
	    {
            ofVideoPlayer::draw(x, y, w, h);
        }
        if (col.a < 255) ofDisableAlphaBlending(); 
	}
}

//oscVPlayer::~oscVPlayer()
//{
//    //dtor
//}
