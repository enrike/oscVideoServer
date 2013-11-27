#ifndef OSCIMAGE_H
#define OSCIMAGE_H

#include <ofImage.h>


class oscImage : public ofImage
{
    public:
        oscImage();
        //virtual ~oscVPlayer();

        void draw();
        void reset();
        void resetSize();

        void setup(string file);

        int x,y,w,h;
        bool bLoaded, alpha;

    protected:
    private:
};

#endif //
