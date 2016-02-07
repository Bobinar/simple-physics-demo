#pragma once
#include <gl/glew.h>
#include "RenderConstants.h"
#include "Drawable.h"

// Quad contained in the XY plane at z = 0
class QuadMesh : public Drawable
{
private:
	GLuint m_vertexBuffer;
	GLuint m_normalBuffer;
	GLuint m_indicesBuffer;

public:
	QuadMesh(float halfWidth, float z)
	{
		static const GLfloat g_vertex_buffer_data[] = {
			-halfWidth,-halfWidth,z,
			halfWidth,-halfWidth,z,
			-halfWidth, halfWidth,z,
			halfWidth, halfWidth,z
		};
		
		static const GLfloat normals[] = {
			0,0,1,
			0,0,1,
			0,0,1,
			0,0,1
		};		

		static const unsigned short indices[] = {
			0,1,2,2,1,3
		};

		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &m_normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

		glGenBuffers(1, &m_indicesBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	~QuadMesh()
	{
		glDeleteBuffers(1, &m_vertexBuffer);
		glDeleteBuffers(1, &m_normalBuffer);
		glDeleteBuffers(1, &m_indicesBuffer);
	}

	void Draw() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glVertexAttribPointer(RenderConstants::PositionAttributeLocation, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(RenderConstants::PositionAttributeLocation);

		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
		glVertexAttribPointer(RenderConstants::NormalAttributeLocation, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(RenderConstants::NormalAttributeLocation);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	}

};

