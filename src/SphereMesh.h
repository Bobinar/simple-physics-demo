#pragma once

#include <Drawable.h>

#include <GL/glew.h>

class SphereMesh : public Drawable
{
private:
	GLuint m_vertexBuffer;
	GLuint m_colorBuffer;
	GLuint m_indicesBuffer;
	int m_indicesCount;

public:
	SphereMesh(float radius, unsigned int rings, unsigned int sectors);
	~SphereMesh();
	void Draw() const;
};