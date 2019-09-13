#if defined(__EMSCRIPTEN__)
	#include <emscripten.h>
#endif

#include <stdio.h>
#include <time.h>
#include <iostream>

#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <SceneManager.h>
#include <SceneInitialization.h>


SceneManager * g_sceneManager;

std::chrono::time_point<std::chrono::system_clock> g_previousTime;
float g_remainingTime;
bool g_quit = false;

void Update()
{
	const float FixedTimestepDelta = 0.001f;

	auto currentTime = std::chrono::system_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime-g_previousTime).count() * 0.000001f;
	g_previousTime = currentTime;
	
	g_remainingTime += deltaTime;
	
	while (g_remainingTime > FixedTimestepDelta)
	{
		g_sceneManager->Update(FixedTimestepDelta);
		g_remainingTime -= FixedTimestepDelta;
	}
}

void Draw()
{
	g_sceneManager->Draw();
}

void HandleKeys(unsigned char key, int x, int y)
{
	if (key == 's')
	{
		g_sceneManager->ShootSphere(x,y);
	}
}

void HandleMouseUp(int x, int y)
{
	g_sceneManager->ShootSphere(x, y);
}

void MainLoop()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			g_quit = true;
		}
		else if (e.type == SDL_TEXTINPUT)
		{
			int x = 0, y = 0;
			SDL_GetMouseState(&x, &y);
			HandleKeys(e.text.text[0], x, y);
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&e;
			int x = 0, y = 0;
			SDL_GetMouseState(&x, &y);
			HandleMouseUp(x, y);
		}
	}

	Update();
	Draw();
}

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_GLContext context;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	GLuint windowWidth = 800;
	GLuint windowHeight = 800;
	window = SDL_CreateWindow("Simple Physics Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	if (!window) {
		printf("Unable to create window: %s\n", SDL_GetError());
		return 1;
	}

	context = SDL_GL_CreateContext(window);

	g_previousTime = std::chrono::system_clock::now();
	g_remainingTime = 0;

#if !defined(__EMSCRIPTEN__)
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}
#endif

	g_sceneManager = SceneInitialization::CreateScene(windowWidth, windowHeight);
		
	SDL_StartTextInput();

#if !defined(__EMSCRIPTEN__)
	while (!g_quit)
	{
		MainLoop();
		SDL_GL_SwapWindow(window);
	}
#else
	emscripten_set_main_loop(MainLoop, 0, 1);
#endif

	SDL_StopTextInput();

	delete g_sceneManager;
	SDL_Quit();

	return 0;
}
