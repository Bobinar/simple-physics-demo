# simple-physics-demo
WIP Small real-time physics demo. C++ with SDL and OpenGL, cross compilable to WebGL. Works on any OS supported by both SDL and OpenGL, plus web WebGL enabled web browsers.

WebGL build: https://dl.dropboxusercontent.com/u/28748252/main.html

For now only the rendering of the scene (with shadows) and very simple bouncing ball physics are implemented.
Needs refactoring and moving code from .h files to .cpp

To run this project in a web browser, compile with emscripten by running "SDLExperiment/build_emscripten.bat".
You can get emscripten at http://emscripten.org/

Project can be generated with cmake. Tested on Windows, OSX and linux.

Using libraries:
+ OpenGL Mathematics (GLM): http://glm.g-truc.net/
+ SDL2: https://www.libsdl.org/
+ GLEW: http://glew.sourceforge.net/

Reference tutorials:
+ http://www.opengl-tutorial.org/