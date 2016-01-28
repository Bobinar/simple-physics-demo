# simple-physics-demo
WIP Small real-time physics demo. C++ with SDL and OpenGL, cross compilable to WebGL. Works on any OS supported by both SDL and OpenGL, plus web WebGL enabled web browsers.

For now only the rendering of the scene (with shadows) and very simple bouncing ball physics are implemented.
Needs refactoring and moving code from .h files to .cpp

To run this project in a web browser, compile with emscripten by running "SDLExperiment/build_emscripten.bat". Assumes GLM include files located at "C:\Tools\glm".
You can get emscripten at http://emscripten.org/

The only included build mechanisms at the moment are a Visual Studio Community 2015 solution and the batch file to build on emscripten. Include and lib paths will need updating depending on where libraries are placed in your machine. I will look into including all dependencies in the repo.

Using libraries:
+ OpenGL Mathematics (GLM): http://glm.g-truc.net/
+ SDL2: https://www.libsdl.org/
+ GLEW: http://glew.sourceforge.net/

Reference tutorials:
+ http://www.opengl-tutorial.org/