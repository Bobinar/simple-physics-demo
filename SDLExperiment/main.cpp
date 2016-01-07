#if defined(__EMSCRIPTEN__)
	#include <emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <gl\glew.h>
#include <SDL2/SDL_opengl.h>

#include <stdio.h>
#include <time.h>

#include <iostream>
#include "SceneRenderer.h"

bool gRenderQuad;

SceneRenderer * g_sceneRenderer;

clock_t g_previousTime;
float g_remainingTime;

void Update()
{
	const float FixedTimestepDelta = 0.001f;

	clock_t currentTime = clock();
	float deltaTime = float(currentTime - g_previousTime) / (float)CLOCKS_PER_SEC;
	g_previousTime = currentTime;
	
	g_remainingTime += deltaTime;
	
	while (g_remainingTime > FixedTimestepDelta)
	{
		g_sceneRenderer->SimulationUpdate(FixedTimestepDelta);
		g_remainingTime -= FixedTimestepDelta;
	}
}

void Draw()
{
	g_sceneRenderer->Draw();
}

// unused for now
void handleKeys(unsigned char key, int x, int y)
{
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

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 

	GLuint windowWidth = 800;
	GLuint windowHeight = 800;
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window = SDL_CreateWindow("sdl_fog_density", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	if (!window) {
		printf("Unable to create window: %s\n", SDL_GetError());
		return 1;
	}

	context = SDL_GL_CreateContext(window);

	g_previousTime = clock();
	g_remainingTime = 0;

#if !defined(__EMSCRIPTEN__)
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}
#endif

	g_sceneRenderer = new SceneRenderer(windowWidth, windowHeight);

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
