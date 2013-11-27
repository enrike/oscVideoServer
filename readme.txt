oscVideoServer by ixi-audio.net v0.1 november 2009

Contact or feedback :
if you have any questions or want to contribute adding or changing some parts of the code let us know.
info@ixi-audio.net

Download :
http://www.ixi-audio.net/backyard



License :
This library is free software; you can redistribute it and/or modify it under the terms of the GNU, General Public License as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA



Description :
This is just simple app developed in C++ with OpenFrameworks (http://www.openframeworks.cc) 
library that wraps some of its functionality adding a couple of properties and functions to simpify the OSC interface. It allows to display videos, images and webcams in a window and control them via Open Sound Control. OSC messages can be sent from many programming languages (python, perl, java, ruby, PureData, Supercollider ... just choose one).




Requirements:
Quicktime http://www.apple.com/quicktime/download/win.html must be installed.


Usage :
Just run the app. It will listen to port 9000 (by default, but this can be changed). OSC messages
arriving to that port will trigger different functions in the server. 


StartUp files :
media_load.xml -> media to be setup on startup. to activate it set <media_load> tag to 1
serversettings.xml -> this file allows to set certain intialization parameters for the server 
(title, size, position, color, port ...), open the file for more details. Note that if you break
the XML file format or delete the file, the server will take some hardcoded default values.

The numID argument of videoplayers, grabberplayers and images is an integer value that determines the position where the video gets rendered, higher value layers get rendered on top. The number of layers is set by default to 20 this can be adjusted in the settings.xml file. If an image, a grabber and a video are in the same layer the rendering order is : grabber on top / image in middle / video at bottom.

for more details check the OpenFrameworks documentation of OpenFrameworks at 
http://www.openframeworks.cc/documentation


** Note that assets (videos) must be located inside data folder next to the server app binary
executable.***
The video formats that can be opened depend on your system, codecs installed, etc... So you'll have to try!

OSX users, if you want to read the server output you must run the /Applications/Console application. This is useful to debug and know if there has been any error.


Examples :
For PureData we created a basic example (osc_msgs.pd) which explains all OSC messages implemented by the server. But we also created some abstractions to allow this to be used by students or people that dont know but really basic PD.
To be done supercollider and python examples.


Pure Data abstractions description:
oscVideoSever/videoserver	args: IP, port		# controls basic server features
oscVideoSever/grabberdevice	args: device ID		# controls camera device and OpenCV motion track utility
oscVideoSever/grabberplayer	args: numID		# displays image from camera grabber device
oscVideoSever/image		args: numID, file	# displays image file
oscVideoSever/videoplayer	args: numID, file	# displays a video file






# List of OSC messages sent by the video server and its arguments #

/server/welcome		1				# sent on startup
/server/connected	1				# answers to /server/ping incomming OSC message
/player/done		numID				# reports when a movie has reached its end (if not looped)
/server/key		int				# on keypressed (if reportinput is ON)
/server/mouse/moved	x,y				# on mouse moved (if reportinput is ON)
/server/mouse/pressed	x,y,button			# on mouse pressed (if reportinput is ON)
/server/mouse/released	x,y,button			# on mouse released (if reportinput is ON)
/server/mouse/dragged	x,y,button			# on mouse dragged (if reportinput is ON)





# List of OSC messages received at server and its arguments #

- server messages (general)
/server/ping						(triggers /server/connected 1 callback)
/server/verbose		1 or 0				# prints the incomming messages to the console 
/server/bgcolor		three integers range 0 to 255 (r,g,b)
/server/fps		integer
/server/cursor		1 or 0				# shows or hides the cursor
/server/reportinput	1 or 0				# if 1 sends message about keyboard and mouse input in the server
/server/clear						# clears all objects in server

- server messages (to control video grabber devices)
/server/listGrabberDevices				# prints in the server console the list of devices available
/server/initGrabberDevice	int deviceID		# startsup video grabber device with that ID
/server/closeGrabberDevice	int deviceID		# closes video grabber device with that ID
/server/grabberDeviceControls				# tries to open the device control panel (only windows?)

- server messages (OpenCV control) * STILL EXPERIMENTAL *
/server/opencv			1 or 0			# starts / stops analyzing camera data 
/server/opencv/threshold	int (0-255)		# threshold of movement
/server/opencv/snapshot					# takes snapshot reference to calculate movement against
/server/opencv/blobs		1 or 0			# renders blob areas into screen

- video player messages
/player/setup		numID, 	filename		# loads the video "fingers.mov" in numID layer
/player/close		numID				# unloads video in numID layer
/player/render		numID	1 or 0			# toggles rendering on/off for numID layer
/player/play 		numID, 	1 or 0			# plays video in numID layer
/player/pause 		numID, 	1 or 0			# pauses/resumes video in numID layer
/player/x 		numID,  integer			# changes horizontal position of video in numID layer
/player/y		numID, 	integer			# changes vertical position of video in numID layer
/player/width 		numID, 	integer			# changes width of video in numID layer
/player/heigth		numID, 	integer			# changes heigth of video in numID layer
/player/resetsize	numID				# resets to original size 
/player/speed		numID, 	float			# chanes play speed of video in numID layer
/player/loop 		numID, 	1 or 0			# toggles video loop on and off
/player/frame	  	numID, 	integer			# moves playhead across video file frames 
/player/volume  	numID, 	integer			# video sound volume

- video grabber messages
/grabber/setup		numID				# instance of the video stream from grabber device on layer numID
/grabber/close		numID				# closes grabber video in layer numID
/grabber/render		numID	1 or 0			# toggles rendering on/off for numID layer
/grabber/x		numID, 	integer			# changes x loc of image in layer numID
/grabber/y		numID, 	integer			# changes y loc of image in layer numID
/grabber/width		numID, 	integer			# changes width of image in layer numID
/grabber/height		numID, 	integer			# changes height of image in layer numID
/grabber/resetsize	numID				# resets to original size

- image messages
/image/setup		numID,	filename		# loads image "image1.jpg" into numID layer
/image/close		numID				# closes image in layer numID
/image/render		numID,	1 or 0			# toggles rendering image in layer numID
/image/alpha		numID,	1 or 0			# toggles alpha of image in layer numID
/image/x		numID,	integer			# changes x loc of image in layer numID
/image/y		numID,	integer			# changes y loc of image in layer numID
/image/width		numID,	integer			# changes width of image in layer numID
/image/height		numID,	integer			# changes height of image in layer numID
/image/resetsize	numID				# resets to original size



Compiling from source:
We provide binaries but if you want to compile the source you need www.openframeworks.cc 
check the documentation and the forum if you get problems. We also hope to provide -soon- a makefile 
so under Unix (I think) you should be able to do the usual ./configure > make > makeinstall but 
I have not tested it myself so far as I am using CodeBlocks myself to code and compile under Linux and Windows. 



TO DO :
- To be done supercollider and python examples.
- more than one webcam?
- effects? (delay, invert, levels, posterize? ...)
- rename pd abstraction videoserver.pd for control.pd

