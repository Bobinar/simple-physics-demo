#pragma once
#include <gl/glew.h>
#include "CubeGeometryFactory.h"
#include "ShaderConstants.h"
#include "glm/vec3.hpp" 

class Cube
{

private:
	GLuint m_vertexbuffer;
	GLuint m_normalBuffer;
public:
	glm::vec3 Position;

	Cube()
	{
		CubeGeometryFactory::CreateCubeBuffers(m_vertexbuffer, m_normalBuffer);
	}

	~Cube()
	{
		glDeleteBuffers(1, &m_vertexbuffer);
		glDeleteBuffers(1, &m_normalBuffer);
	}

	void Draw() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glVertexAttribPointer(ShaderConstants::PositionAttributeLocation, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(ShaderConstants::PositionAttributeLocation);

		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
		glVertexAttribPointer(ShaderConstants::NormalAttributeLocation, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(ShaderConstants::NormalAttributeLocation);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
};
