Your Monocle app will run on the iPhone and iPad! (Assumes iOS 3.1+)

This document is meant to be a list of steps to create an iOS version of your Monocle app, assuming that you have xcode4 and some other shiny stuff.

1) Prepare the MonocleCore xcode4 project.
Right now, premake4 doesn't support iOS, and it's not likely going to soon. Fortunately, it does support MAC, so start by building your MonocleCore project with:
: premake4 xcode4

2) Create a new iPad/iPhone/UniversaliOS app using xcode. You will a WINDOW-BASED application. Make sure you add the following frameworks:
OpenGLES
OpenAL
QuartzCore

3) Remove all the sources provided, including the AppDelegate.m and device specific sources.  Copy main.mm, MonocleIOSAppDelegate.mm and MonocleIOSAppDelegate.h from the Templates/iOS folder.

4) The template assumes OGMO from samples is provided. 
    - change ogmo.h to the header containing your initial scene
    - change applicationDidFinishLaunching to initialize your initial scene and send it to Game::SetScene().

5) To include resources you simply drag your content folder into the project, but when asked add the contents AS FOLDER REFERENCE. This will retain your directory structure, and should update itself automagically.