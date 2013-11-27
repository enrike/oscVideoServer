
#include "testApp.h"



//--------------------------------------------------------------
testApp::testApp()
{
    connected = false;

    numoflayers = 10;

    isGrabberInit = false;

    // opencv
    trackOpenCV = false;
    bLearnBakground = true;
    threshold = 80;
    plotblobs = false;
}


testApp::~testApp()
{
    printf("closing down app...................\n");
    clearAll();
    vidGrabber.close();
    //dtor
}


void testApp::clearAll()
{
    printf("clearing all objects in server\n");

    //videoDevice.

//    shotImg.clear();
    colorImg.clear();
    grayImage.clear();
    grayBg.clear();
    grayDiff.clear();

    for (int i = 0; i < numoflayers; i++ )
    {
        players.at(i).close();
        players.at( i ).reset();
        images.at(i).reset();
        images.at(i).clear();
        grabbers.at(i).reset();
    }
    // destroy vectors
    grabbers.clear();   // video device renderers
    players.clear();    // video files players
    images.clear();     // image files
}

//--------------------------------------------------------------
void testApp::setup()
{
    //ofSetVerticalSync(true); // sync frame rate with monitor refresh rate
    //ofSetFrameRate(24);

	/////////////////////////////////////////////
    ofSetLogLevel(OF_LOG_VERBOSE);

    // LOADING PREFERENCES //
    if( XML.loadFile("serversettings.xml") )
    {
		printf("serversettings.xml loaded!\n");
	}
	else
	{
		printf("unable to load serversettings.xml check data/ folder\n");
	}

    verbose = XML.getValue("oscVideoServer:verbose", 1); //
    reportinput = XML.getValue("oscVideoServer:reportinput", 0); //

    // PORTS
	string ip = XML.getValue("oscVideoServer:ip", "localhost");
	int port = XML.getValue("oscVideoServer:port", 9000);

    sender.setup( ip, port+1 ); // Send on port +1

    receiver.setup( port ); // hardcoded port
    connected = true;


    // window properties
    string title = XML.getValue("oscVideoServer:title", "oscVideoServer");
    ofSetWindowTitle(title);

    int cursor = XML.getValue("oscVideoServer:cursor", 0);
    if (cursor == 0) ofHideCursor(); // cursor

    // Fullscreen or not ?
	if ( XML.getValue("oscVideoServer:fullscreen", 0) )
	{
	    printf("oscVideoServer going fullscreen!\n");
	    ofSetFullscreen(1);
	    ofSetWindowShape( ofGetScreenWidth(), ofGetScreenHeight() );
	    //printf("%i, %i, %i, %i", ofGetWidth(), ofGetHeight(), ofGetScreenWidth(), ofGetScreenHeight() );
    }
    else
    {
        int w = XML.getValue("oscVideoServer:size:width", 800);
        int h = XML.getValue("oscVideoServer:size:height", 600);
        ofSetWindowShape(w,h);

        int xpos = XML.getValue("oscVideoServer:pos:x", 20);
        int ypos = XML.getValue("oscVideoServer:pos:y", 20);
        ofSetWindowPosition(xpos, ypos);
    }

    //printf( "%i, %i", ofGetScreenWidth(), ofGetScreenHeight() );

	int r = XML.getValue("oscVideoServer:color:red", 255);
	int g = XML.getValue("oscVideoServer:color:green", 255);
	int b = XML.getValue("oscVideoServer:color:blue", 255);
	ofBackground(r,g,b); // bg color

    int fps = XML.getValue("oscVideoServer:fps", 24);
    ofSetFrameRate(fps);

    // length of vectors containing videos and grabbers
    numoflayers = XML.getValue("oscVideoServer:numoflayers", 10);

//    // shot stuff // delete or comment *******************
//    shotLength = XML.getValue("oscVideoServer:shot_time", 5000);

    XML.clear();

    // init instances //
    for (int i = 0; i < numoflayers; i++ )
    {
        players.push_back( oscVPlayer() );
        grabbers.push_back( oscVGrabber(&vidGrabber) );
        images.push_back( oscImage() );
    }

    loadMedia(); //read XML file and setup initial configuration with video, images and grabbers

    XML.clear();

    // broadcast welcome message
    ofxOscMessage m;
    m.setAddress( "/server/welcome" );
    m.addIntArg( 1 );
    sender.sendMessage( m ); // report back that we are alive and clicking here!
}





/* tries to load media reading media_load.xml file
*/
void testApp::loadMedia()
{
    // LOADING Media on startup //
    if( !XML.loadFile("media_load.xml") ) // if error loading
    {
        printf("no media_load.xml. just skipping...\n");
	}
	else
	{
	    if (XML.getValue("media_load", 0) == 1) // Flag is ON
	    {
            printf("trying to load data from media_load.xml file\n");

            // VIDEOS
            int numVideos = XML.getNumTags("video");

            if (numVideos > 0)
            {
                for (int i=0; i < numVideos; i++)
                {
                    int id = XML.getValue("video:numID", 0, i);

                    string ff = XML.getValue("video:file", "none", i);
                    players.at( id ).setUpVideo( ff );

                    players.at( id ).x = XML.getValue("video:x", 0, i);
                    players.at( id ).y = XML.getValue("video:y", 0, i);
                    players.at( id ).w = XML.getValue("video:width", 100, i);
                    players.at( id ).h = XML.getValue("video:height", 100, i);

                    ofLoopType lp = XML.getValue("video:loop", 1, i) ? OF_LOOP_NORMAL : OF_LOOP_NONE ;
                    players.at( id ).setLoopState(lp);

                    int volume = XML.getValue("video:volume", 255, i);
                    players.at( id ).setVolume(volume);

                    int frame = XML.getValue("video:frame", 0, i);
                    players.at( id ).setFrame(frame);

                    float speed = XML.getValue("video:speed", 1, i);
                    players.at( id ).setSpeed(speed);

                    int play = XML.getValue("video:play", 1, i);
                    if (play==1) players.at( id ).play();
                }
            }
            // GRABBERS
            int numGrabbers = XML.getNumTags("grabber");

            if (numGrabbers > 0)
            {
                if (!isGrabberInit)
                {
                    printf("grabber deviced not init yet, trying to init now\n");
                    for (int i=0; i < 20; i++)
                    {
                        if (initGrabberDevice(i)) break;
                    }
                }

                if (isGrabberInit) // could init grabber
                {
                    for (int i=0; i < numGrabbers; i++)
                    {
                        int id = XML.getValue("grabber:numID", 0, i);

                        grabbers.at( id ).setup( );
                        grabbers.at( id ).x = XML.getValue("grabber:x", 0, i);
                        grabbers.at( id ).y = XML.getValue("grabber:y", 0, i);
                        grabbers.at( id ).w = XML.getValue("grabber:width", 100, i);
                        grabbers.at( id ).h = XML.getValue("grabber:height", 100, i);
                    }
                }
                else
                {
                    printf("could not init grabber device, do you have a camera connected to the computer??\n");
                }
            }
            // IMAGES
            int numImages = XML.getNumTags("image");

            if (numImages > 0)
            {
                for (int i=0; i < numImages; i++)
                {
                    int id = XML.getValue("image:numID", 0, i);

                    string ff = XML.getValue("image:file", "none", i);
                    images.at( id ).setup( ff );

                    images.at( id ).x = XML.getValue("image:x", 0, i);
                    images.at( id ).y = XML.getValue("image:y", 0, i);
                    images.at( id ).w = XML.getValue("image:width", 100, i);
                    images.at( id ).h = XML.getValue("image:height", 100, i);

                    int alpha = XML.getValue("image:alpha", false, i) ? true : false ;
                    images.at( id ).alpha = alpha;
                }
            }// end images
	    }
	} // xml file loaded
}



bool testApp::initGrabberDevice(int id)
{
    vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID( id );

    if ( ofGetWindowMode()== OF_FULLSCREEN )
    {
        isGrabberInit = vidGrabber.initGrabber( ofGetScreenWidth(),  ofGetScreenHeight());
    }
    else
    {
        isGrabberInit = vidGrabber.initGrabber( ofGetWidth(),  ofGetHeight());
    }
    //isGrabberInit = vidGrabber.initGrabber( ofGetWidth(),  ofGetHeight());

    if ( isGrabberInit == false )
    {
        printf("oscVideoServer error : could not init grabber device with ID %i \n", id);
    }
//    else {
//        // shot stuff delete later ********************************
//        shotImg.allocate( vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_COLOR );
////                        colorImg.allocate( vidGrabber.getWidth(), vidGrabber.getHeight() );
////                        grayImage.allocate( vidGrabber.getWidth(), vidGrabber.getHeight() );
////                        grayBg.allocate( vidGrabber.getWidth(), vidGrabber.getHeight() );
////                        grayDifcout << right << msg_string << endl;f.allocate( vidGrabber.getWidth(), vidGrabber.getHeight() );
//    }
    return isGrabberInit;
}


//--------------------------------------------------------------
void testApp::update()
{
    if ( connected == true )
    {
        int fail = 0;
        //printf("%i, %i, %i, %i\n", ofGetWidth(), ofGetHeight(), ofGetScreenWidth(), ofGetScreenHeight() );


        // check for waiting messages
        while( receiver.hasWaitingMessages() )
        {
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage( &m );

            // check for mouse moved message
            if ( m.getAddress() == "/server/verbose" )
            {
                verbose = m.getArgAsInt32( 0 ) ;
            }
            if ( m.getAddress() == "/server/reportinput" )
            {
                reportinput = m.getArgAsInt32( 0 ) ;
            }
            else if (m.getAddress() == "/server/ping")
            {
                ofxOscMessage m;
                m.setAddress( "/server/connected" );
                m.addIntArg( 1 );
                sender.sendMessage( m ); // report back that we are alive and clicking here!
            }
            else if (m.getAddress() == "/server/clear")
            {
                printf("clearing all objects in server\n");
                for (int i = 0; i < numoflayers; i++ )
                {
                    players.at(i).close();
                    players.at( i ).reset();
                    images.at(i).reset();
                    images.at(i).clear();
                    grabbers.at(i).reset();
                }
            }
            else if (m.getAddress() == "/server/bgcolor")
            {
                ofBackground( m.getArgAsInt32( 0 ), m.getArgAsInt32( 1 ), m.getArgAsInt32( 2 ) );
            }
            else if (m.getAddress() == "/server/fps")
            {
                ofSetFrameRate( m.getArgAsInt32( 0 ) );
            }
            else if (m.getAddress() == "/server/cursor")
            {
                if ( m.getArgAsInt32( 0 ) == 1 )
                {
                    ofShowCursor();
                }
                else
                {
                    ofHideCursor();
                }
            }
            else if (m.getAddress() == "/server/initGrabberDevice")
            {
                if (isGrabberInit == false) //only if not already on
                {
                    initGrabberDevice( m.getArgAsInt32( 0 ) );
                }
            }
            else if (m.getAddress() == "/server/closeGrabberDevice")
            {
                vidGrabber.close();
                isGrabberInit = false;
            }
            else if (m.getAddress() == "/server/listGrabberDevices")
            {
                vidGrabber.listDevices();
            }
            else if (m.getAddress() == "/server/grabberDeviceControls")
            {
                vidGrabber.videoSettings();
            }



//            // snapshot stuff. delete or coment out
//            else if(m.getAddress() == "/server/takeshot")
//            {
//                takeShot = true;
//            }




            //  VIDEO PLAYERS //////////////////////////////
            else if ( m.getAddress() == "/player/setup")
            {   // loads video
                players.at( m.getArgAsInt32( 0 ) ).setUpVideo( m.getArgAsString( 1 ) );
            }
            else if ( m.getAddress() == "/player/close")
            {
                players.at( m.getArgAsInt32( 0 ) ).close(); // unloads the video
                players.at( m.getArgAsInt32( 0 ) ).reset(); // reinit all vars
            }
            else if ( m.getAddress() == "/player/play")
            {
                if (players.at( m.getArgAsInt32( 0 ) ).isLoaded()) // otherwise it crashes
                {
                    if ( m.getArgAsInt32( 1 ) == 1 )
                    {
                        //printf("looped 1 : %i\n", players.at( m.getArgAsInt32( 0 ) ).loopflag);
                        //players.at( m.getArgAsInt32( 0 ) ).setLoop(); // set loop before playing!
                        players.at( m.getArgAsInt32( 0 ) ).setFrame(1);// rewind before playing
                        players.at( m.getArgAsInt32( 0 ) ).play();
                        players.at( m.getArgAsInt32( 0 ) ).setLoop(); // set loop after playing!
                        players.at( m.getArgAsInt32( 0 ) ).donereported = false;
                        //printf("looped 2 : %i\n", players.at( m.getArgAsInt32( 0 ) ).loopflag);
                    }
                    else
                    {
                        players.at( m.getArgAsInt32( 0 ) ).stop();
                    }
                }
                else
                {
                    printf("movie not loaded yet!\n");
                }
            }
            else if (m.getAddress() == "/player/pause")
            {
                bool b = m.getArgAsInt32( 1 ) ? true : false ;  // not sure this is good
                players.at( m.getArgAsInt32( 0 ) ).setPaused( b );
            }
            else if (m.getAddress() == "/player/x")
            {
                players.at( m.getArgAsInt32( 0 ) ).x = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/player/y")
            {
                players.at( m.getArgAsInt32( 0 ) ).y = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/player/width")
            {
                players.at( m.getArgAsInt32( 0 ) ).w = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/player/height")
            {
                players.at( m.getArgAsInt32( 0 ) ).h = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/player/resetsize")
            {
                players.at( m.getArgAsInt32( 0 ) ).resetSize();
            }
            else if (m.getAddress() == "/player/speed")
            {
                players.at( m.getArgAsInt32( 0 ) ).setSpeed( m.getArgAsFloat( 1 ) );
            }
            else if (m.getAddress() == "/player/loop")
            {   // beware that i is an int and not a bool
                if (players.at( m.getArgAsInt32( 0 ) ).isLoaded()) // otherwise it crashes
                {
                    ofLoopType i = m.getArgAsInt32( 1 ) ? OF_LOOP_NORMAL : OF_LOOP_NONE ; // converts 0/1 to 1/3
                    players.at( m.getArgAsInt32( 0 ) ).loopflag = i; // remember for setting when playing
                    players.at( m.getArgAsInt32( 0 ) ).setLoopState(i);
                    if ( !players.at(i).getIsMovieDone() ) players.at(i).donereported = false; // make sure it reports on stop
                    //printf("setting loop to %i\n", i);
                }
            }
            else if (m.getAddress() == "/player/frame")
            {
                int f = m.getArgAsInt32( 1 ) ;
                int total = players.at( m.getArgAsInt32( 0 ) ).getTotalNumFrames();
                if ( f > 0 && f <= total ) // within movie bounds
                {
                    //float s = players.at( m.getArgAsInt32( 0 ) ).getSpeed();
                    //players.at( m.getArgAsInt32( 0 ) ).setSpeed( 0.00001 ); // almost pause, this is because a bug somewhere in OF
                    players.at( m.getArgAsInt32( 0 ) ).setFrame( f );
                    //players.at( m.getArgAsInt32( 0 ) ).setSpeed( s ); // restore
                }
            }
            else if (m.getAddress() == "/player/render")
            {
                if ( players.at( m.getArgAsInt32( 0 ) ).isLoaded() ) // otherwise crashes
                {
                    bool b = m.getArgAsInt32( 1 ) ? true : false ; // not sure this is good
                    players.at( m.getArgAsInt32( 0 ) ).setUseTexture( b );
                }
                else
                {
                    printf("movie not loaded yet!\n");
                }
            }
            else if (m.getAddress() == "/player/volume")
            {
                players.at( m.getArgAsInt32( 0 ) ).setVolume( m.getArgAsInt32( 1 ) );
            }


            // GRABBERS //////////////////////////
            else if (m.getAddress() == "/grabber/setup")
            {
                grabbers.at( m.getArgAsInt32( 0 ) ).setup();
            }
            else if (m.getAddress() == "/grabber/close")
            {
                grabbers.at( m.getArgAsInt32( 0 ) ).reset(); // just reset the variables
            }
            else if (m.getAddress() == "/grabber/render")
            { //in this case we just dont draw it
                grabbers.at( m.getArgAsInt32( 0 ) ).setRender( m.getArgAsInt32( 1 ) );
            }
            else if (m.getAddress() == "/grabber/x")
            {
                grabbers.at( m.getArgAsInt32( 0 ) ).x = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/grabber/y")
            {
                grabbers.at( m.getArgAsInt32( 0 ) ).y = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/grabber/resetsize")
            {
                grabbers.at( m.getArgAsInt32( 0 ) ).resetSize();
            }
            else if (m.getAddress() == "/grabber/width")
            {
                grabbers.at( m.getArgAsInt32( 0 ) ).w = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/grabber/height")
            {
                grabbers.at( m.getArgAsInt32( 0 ) ).h = m.getArgAsInt32( 1 );
            }


            // images //////////////////////////////////
            else if (m.getAddress() == "/image/setup")
            {
                images.at( m.getArgAsInt32( 0 ) ).setup( m.getArgAsString( 1 ) );
            }
            else if (m.getAddress() == "/image/close")
            {
                images.at( m.getArgAsInt32( 0 ) ).reset();
                images.at( m.getArgAsInt32( 0 ) ).clear(); // just reset the variables
            }
            else if (m.getAddress() == "/image/render")
            {
                bool b = m.getArgAsInt32( 1 ) ? true : false ; // not sure this is good
                images.at( m.getArgAsInt32( 0 ) ).setUseTexture( b );
            }
            else if (m.getAddress() == "/image/alpha")
            {
                images.at( m.getArgAsInt32( 0 ) ).alpha = m.getArgAsInt32( 1 ) ? true : false ;
            }
            else if (m.getAddress() == "/image/x")
            {
                images.at( m.getArgAsInt32( 0 ) ).x = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/image/y")
            {
                images.at( m.getArgAsInt32( 0 ) ).y = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/image/width")
            {
                images.at( m.getArgAsInt32( 0 ) ).w = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/image/height")
            {
                images.at( m.getArgAsInt32( 0 ) ).h = m.getArgAsInt32( 1 );
            }
            else if (m.getAddress() == "/image/resetsize")
            {
                images.at( m.getArgAsInt32( 0 ) ).resetSize();
            }

            // OpenCV //////////////////////////////////
            else if (m.getAddress() == "/opencv")
            {
                trackOpenCV = m.getArgAsInt32( 0 ) ? true : false ; // not sure this is good
                if (trackOpenCV && isGrabberInit )
                {
                    // INIT openCV images
                    colorImg.allocate( vidGrabber.getWidth(), vidGrabber.getHeight() );
                    grayImage.allocate( vidGrabber.getWidth(), vidGrabber.getHeight() );
                    grayBg.allocate( vidGrabber.getWidth(), vidGrabber.getHeight() );
                    grayDiff.allocate( vidGrabber.getWidth(), vidGrabber.getHeight() );
                }
            }
            else if (m.getAddress() == "/opencv/threshold")
            {
                threshold = m.getArgAsInt32( 0 );
            }
//            else if (m.getAddress() == "/opencv/snapshot")
//            {
//                bLearnBakground = true;
//            }
            else if (m.getAddress() == "/opencv/plotblobs")
            {
                plotblobs =  m.getArgAsInt32( 0 ) ? true : false ; ;
            }


            // FAIL ////////////////////////////////////
            else // unrecognized message
            {
                fail = 1;
            }


            // PRINT if verbose
            if (verbose == 1 or fail == 1)
            {
                msg_string = "";
                if (fail==1) msg_string += "unrecognized message address! -> ";
                msg_string = m.getAddress();
                msg_string += ": ";

                for ( int i=0; i<m.getNumArgs(); i++ )
                {
                    // get the argument type
                    msg_string += " // ";
                    msg_string += m.getArgTypeName( i );
                    msg_string += " : ";

                    // display the argument - make sure we get the right type
                    if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
                        msg_string += ofToString( m.getArgAsInt32( i ) );
                    else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
                        msg_string += ofToString( m.getArgAsFloat( i ) );
                    else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
                        msg_string += m.getArgAsString( i );
                    else
                        msg_string += "unknown";
                }
                // add to the list of strings to display
                //printf("%s", msg_string);

                cout << right << msg_string << endl;

            } // end verbose
        }// end while


        if (reportinput == 1)
        {
            ofxOscMessage m;
            m.setAddress( "/server/mouse/loc" );
            m.addIntArg( mouseX ); // mouse x loc
            m.addIntArg( mouseY ); // mouse y loc
            sender.sendMessage( m );
        }
	} // end if connected



    // Update camera device
    if (isGrabberInit)
    {
        vidGrabber.grabFrame();

        if ( vidGrabber.isFrameNew() )
        {
//            if ( takeShot )
//            {
//                //shotImg.clear();
//                shotImg.setFromPixels( vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_COLOR, true );
//                takeShot = false;
//                shotTimeOut = ofGetElapsedTimeMillis() + shotLength; // secs to wait from now
//            }

            if ( trackOpenCV )
            {
                colorImg.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight());

                grayImage = colorImg;
                if (bLearnBakground == true){
                    grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
                    bLearnBakground = false;
                }

                    // take the abs value of the difference between background and incoming and then threshold:
                grayDiff.absDiff(grayBg, grayImage);
                grayDiff.threshold(threshold);
            //		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
            //		// also, find holes is set to true so we will get interior contours as well....
                contourFinder.findContours(grayDiff, 20, (vidGrabber.getWidth()*vidGrabber.getHeight())/3, 10, true);	// find holes

                ofxOscBundle b;
                ofxOscMessage m;

                for (int n=0; n < contourFinder.blobs.size(); n++ )
                {
                    m.setAddress( "/server/opencv" );
                    m.addIntArg( n );
                    m.addIntArg( contourFinder.blobs[n].centroid.x );
                    m.addIntArg( contourFinder.blobs[n].centroid.y );
                    b.addMessage(m);
                    m.clear();
                }
                sender.sendBundle( b );
//                players.at(i).donereported = true;
            }
        }
    }


    // Update videos //
    for (int i = 0; i < numoflayers; i++ )
    {
        if ( players.at(i).isLoaded() )
        {
            players.at(i).idleMovie();

            if ( players.at(i).getIsMovieDone() && players.at(i).donereported == false ) // Video ended
            {
                ofxOscMessage m;
                m.setAddress( "/player/done" );
                m.addIntArg( i );
                sender.sendMessage( m );
                players.at(i).donereported = true;
                if (verbose) printf("done with video in layer %i\n", i);
            }
        }
    }



}

//--------------------------------------------------------------
void testApp::draw()
{
    for (int i = 0; i < numoflayers; i++ )
    {
        players.at(i).draw();
        images.at(i).draw();
        grabbers.at(i).draw();
    }

    if (plotblobs && trackOpenCV) // tracking and want to plot the blobs
    {
        for (int i = 0; i < contourFinder.nBlobs; i++)
            contourFinder.draw( (float)0, (float)0, vidGrabber.getWidth(), vidGrabber.getHeight() );
    }

//    // snapshot stuff. to later delete
//    if ( ofGetElapsedTimeMillis() < shotTimeOut )
//    {
//        //shotImg.draw( 0, 0, ofGetWidth(), ofGetHeight() );
//        if ( ofGetWindowMode()== OF_FULLSCREEN )
//        {
//            shotImg.draw( 0, 0, ofGetScreenWidth(), ofGetScreenHeight() );
//        }
//        else
//        {
//            shotImg.draw( 0, 0, ofGetWidth(), ofGetHeight() );
//        }
//    }

//    if (verbose == 1)
//    {
//        ofSetColor(0xffffff);
//        ofRect(5,0, 400, 16); // white marquee
//        ofSetColor(0x000000);
//        ofDrawBitmapString(msg_string, 10,10);
//    }
}





void testApp::keyPressed  (int key)
{
    if (reportinput == 1)
    {
        ofxOscMessage m;
        m.setAddress( "/server/key" );
        m.addIntArg(key);
        sender.sendMessage( m );
    }
	// in fullscreen mode, on a pc at least, the
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...
	if (key == 's' || key == 'S')
	{
		vidGrabber.videoSettings();
	}
	// shot stuff // delete or comment
//	else if( key == ' ') // spacebar
//	{
//	    takeShot = true;
//	}
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
    if (reportinput==1)
    {
        ofxOscMessage m;
        m.setAddress( "/server/mouse/moved" );
        m.addIntArg(x);
        m.addIntArg(y);
        sender.sendMessage( m );
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    if (reportinput==1)
    {
        ofxOscMessage m;
        m.setAddress( "/server/mouse/dragged" );
        m.addIntArg(x);
        m.addIntArg(y);
        m.addIntArg(button);
        sender.sendMessage( m );
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    if (reportinput==1)
    {
        ofxOscMessage m;
        m.setAddress( "/server/mouse/pressed" );
        m.addIntArg(x);
        m.addIntArg(y);
        m.addIntArg(button);
        sender.sendMessage( m );
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if (reportinput==1)
    {
        ofxOscMessage m;
        m.setAddress( "/server/mouse/released" );
        m.addIntArg(x);
        m.addIntArg(y);
        m.addIntArg(button);
        sender.sendMessage( m );
    }
}

