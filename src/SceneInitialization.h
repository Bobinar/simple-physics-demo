#pragma once
#include <GL/glew.h>

class SceneManager;

namespace SceneInitialization
{
	SceneManager* CreateScene(GLuint width, GLuint height);
}
