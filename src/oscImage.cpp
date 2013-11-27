#include "oscImage.h"
#include "ofMain.h"



oscImage::oscImage()
{
    reset();
//        x = 0;
//    y = 0;
//    w = ofGetWidth();
//    h = ofGetHeight();
//    bLoaded = false;
//    alpha = false;
}


void oscImage::reset()
{
    x = 0;
    y = 0;
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
    bLoaded = false;
    alpha = false;

    if (bLoaded )
        setUseTexture(1);
}


void oscImage::setup(string file)
{
    if (file != "none")
    {
        if ( loadImage(file) )
        {
            printf("loaded image\n");
            bLoaded = true;
        }
        else
        {
            bLoaded = false;
            printf("CANNOT load image\n");
        }
    }
    else
    {
        printf("oscImage::setup -> file parameter is none, cannot load it\n");
    }
}


void oscImage::resetSize()
{
    w = NULL;
    h = NULL;
}




void oscImage::draw()
{
	if (bLoaded)
	{
//	    if (alpha)
//	    {
            if (alpha) ofEnableAlphaBlending();

            if (w==NULL && w==NULL)
            {
                ofImage::draw(x, y);
            }
            else
            {
                ofImage::draw(x, y, w, h);
            }

            if (alpha) ofDisableAlphaBlending();
//        } else {
//            ofImage::draw(x, y, w, h);
//        }
	}
}


//oscVPlayer::~oscVPlayer()
//{
//    //dtor
//}
