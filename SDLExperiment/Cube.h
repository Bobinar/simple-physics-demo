#pragma once
#include <gl/glew.h>
#include "CubeGeometryFactory.h"
#include "glm/vec3.hpp" 

class Cube
{

private:
	GLuint m_vertexbuffer;
	GLuint m_colorbuffer;
public:
	glm::vec3 Position;

	Cube()
	{
		CubeGeometryFactory::CreateCubeBuffers(m_vertexbuffer, m_colorbuffer);
	}

	~Cube()
	{
		glDeleteBuffers(1, &m_vertexbuffer);
		glDeleteBuffers(1, &m_colorbuffer);
	}

	void Draw() const
	{
		// need to do something like this

		// GLuint positionID = glGetAttribLocation(programID, "position_modelspace");
		//GLuint uvID = glGetAttribLocation(programID, "uv");

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
};
