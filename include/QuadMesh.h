#pragma once
#include <Drawable.h>

#include <GL/glew.h>

// Quad contained in the XY plane at z = 0
class QuadMesh : public Drawable
{
private:
	GLuint m_vertexBuffer;
	GLuint m_normalBuffer;
	GLuint m_indicesBuffer;

public:
	QuadMesh(float halfWidth, float z);
	~QuadMesh();
	void Draw() const;
};
