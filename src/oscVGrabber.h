#ifndef OSCVGRABBER_H
#define OSCVGRABBER_H

#include <ofVideoGrabber.h>


class oscVGrabber //: public ofVideoGrabber
{
    public:
        oscVGrabber(ofVideoGrabber* vg);
        //virtual ~oscVGrabber();

        void draw();
        void setup();
        void setRender(int b);
        void resetSize();

        void reset();

        int x,y,w,h;
        bool init;

    protected:
    private:
        ofVideoGrabber* vGr;
};

#endif // OSCVGRABBER_H
