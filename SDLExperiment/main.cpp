#if defined(__EMSCRIPTEN__)
	#include <emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <gl\glew.h>
#include <SDL2/SDL_opengl.h>

#include <stdio.h>
#include <time.h>

#include "Cube.h"

#include "glm/vec3.hpp" 
#include "glm/vec4.hpp" 
#include "glm/mat4x4.hpp" 
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/constants.hpp" 
#include <iostream>
#include "SceneRenderer.h"

bool gRenderQuad;

SceneRenderer * g_sceneRenderer;

///
// Draw a triangle using the shader pair created in Init()
//
GLuint esContextWidth, esContextHeight;
float rotationAngle = 0.0f;
clock_t begin_time;

void Update()
{
	clock_t currentTime = clock();
	float deltaTime = float(currentTime - begin_time) / CLOCKS_PER_SEC;
	begin_time = currentTime;
	rotationAngle += 1.f * deltaTime;
	//printf("%f \n", deltaTime);

	g_sceneRenderer->Update(deltaTime);
}

void Draw()
{
	g_sceneRenderer->Draw();
}

void handleKeys(unsigned char key, int x, int y)
{
	//Toggle quad
	if (key == 'q')
	{
		gRenderQuad = !gRenderQuad;
	}
}


void MainLoop()
{
	Update();
	Draw();
}

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_GLContext context;

	// Slightly different SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // *new*

	GLuint esContextWidth = 800;
	GLuint esContextHeight = 800;
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window = SDL_CreateWindow("sdl_fog_density", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, esContextWidth, esContextHeight, SDL_WINDOW_OPENGL);
	if (!window) {
		printf("Unable to create window: %s\n", SDL_GetError());
		return 1;
	}

	context = SDL_GL_CreateContext(window);
	printf("%p\n", context);

	begin_time = clock();

#if !defined(__EMSCRIPTEN__)
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}
#endif

	g_sceneRenderer = new SceneRenderer(esContextWidth, esContextHeight);

	bool quit = false;
#if !defined(__EMSCRIPTEN__)
	SDL_Event e;

	//Enable text input
	SDL_StartTextInput();
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			//Handle keypress with current mouse position
			else if (e.type == SDL_TEXTINPUT)
			{
				int x = 0, y = 0;
				SDL_GetMouseState(&x, &y);
				handleKeys(e.text.text[0], x, y);
			}
		}

		MainLoop();

		//Update screen
		SDL_GL_SwapWindow(window);
	}

	SDL_StopTextInput();
#else
	emscripten_set_main_loop(MainLoop, 0, 1);
#endif

	SDL_Quit();

	return 0;
}
