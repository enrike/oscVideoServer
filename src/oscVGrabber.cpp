
#include "oscVGrabber.h"
#include "ofMain.h"

oscVGrabber::oscVGrabber(ofVideoGrabber* vg)
{
//    //defaults to fullscreen
//    w = ofGetWidth();
//    h = ofGetHeight();
//    x = 0;
//    y = 0;
//
//    init = false;
    reset();

    vGr = vg;
        //printf("%i", vGr->getWidth());
}


void oscVGrabber::resetSize()
{
    w = NULL;
    h = NULL;
}

void oscVGrabber::setup()
{
    init = true;
}

void oscVGrabber::reset()
{
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

    init = false;

    //ofsetUseTexture(1);
}

void oscVGrabber::draw()
{
    if (init==true)
    {
        if (w==NULL && h==NULL)
        {
            vGr->draw(x,y);
        }
        else
        {
            vGr->draw(x,y,w,h);
        }
    }
}



void oscVGrabber::setRender(int b)
{
    init = b ? true : false ;
    //ofVideoPlayer::setUseTexture(n);
}



//oscVGrabber::~oscVGrabber()
//{
//    //dtor
//}
