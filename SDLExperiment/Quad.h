#pragma once
#include "glm/vec3.hpp" 
#include <gl/glew.h>

// Quad contained in the XY plane at z = 0
class Quad
{
private:
	GLuint m_vertexBuffer;
	GLuint m_colorBuffer;
	GLuint m_indicesBuffer;

public:

	float z;
	float halfWidth;
	
	Quad()
		: z(0)
		, halfWidth(1)
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

		glGenBuffers(1, &m_colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

		glGenBuffers(1, &m_indicesBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	~Quad()
	{
		glDeleteBuffers(1, &m_vertexBuffer);
		glDeleteBuffers(1, &m_colorBuffer);
		glDeleteBuffers(1, &m_indicesBuffer);
	}

	void Draw() const
	{
		// need to do something like this

		// GLuint positionID = glGetAttribLocation(programID, "position_modelspace");
		//GLuint uvID = glGetAttribLocation(programID, "uv");

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	}

};

