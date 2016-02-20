## IAco79 very original game Dots Kollide source code now for free!

I started this project to learn about native cross-platform game development in C/C++ by using only "free" open source libraries.

### You can find more about dots kollide  and other projects in my blog: 
* Dots kollide (what this source code is about).
https://iaco79.wordpress.com/2014/04/07/dots-kollide-is-now-available-on-google-play/

* Addions (a similar game based on the same engine).
https://iaco79.wordpress.com/2014/10/13/addions-the-free-moving-numbers-puzzle/

### Project structure:

> Source directories:
>* jni/Game/ : game engine files (more like an integrator)
>* jni/tonatiuh : Dots Kollide game code
>* src/ : Android java project
>* win32src/ : win32 entry point.
>* assets/ : game assets 

> Base libraries (source code included) are:
>* irrlicht : graphics (opengl-es 2.0) 
>* SDL / SDL Mixer : audio / music
>* libogg/vorbis : ogg music
>* librocket : GUI 
>* lua/oolua : basic scripting
>* box2d : 2d physics
>* freetype 

#### HOW TO BUILD - Android
	Requirements:
		* Android SDK 
		* Android NDK

	android update project -p .
	export NDK_MODULE_PATH=./jni 
	ndk-build
	ant debugenter'''



##### HOW TO BUILD - Win32
	Requirements:
		* libGLES emulator lib
		* Visual Studio 2010 or later.

	*Build GameDemo10.0.sln solution*

Enjoy!


