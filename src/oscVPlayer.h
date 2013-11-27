#ifndef OSCVPLAYER_H
#define OSCVPLAYER_H

#include <ofVideoPlayer.h>


class oscVPlayer : public ofVideoPlayer
{
    public:
        oscVPlayer();
        //virtual ~oscVPlayer();

        void draw();
        void reset();
        void resetSize();

        void setUpVideo(string movie);
        //void setUpVideo(string movie, int x, int y, int w, int h, float _speed);

        void setLoop();
        //void setPaused(int b);
        //void setRender(int b);
        //void setZ(int z);

        int x,y,w,h;
        bool donereported;
        ofLoopType loopflag;
        ofColor col;

    protected:
    private:
};

#endif // OSCVPLAYER_H
