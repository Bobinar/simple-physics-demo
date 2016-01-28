#pragma once
#include <gl/glew.h>


class Triangle
{
private:
	GLuint vertexPosObject;
	GLuint vertexColorObject;

public:
	Triangle()
	{
		GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f };

		GLfloat vVertexColors[] = { 1.0f, 0.0f, 0.f,
			0.0f, 1.0f, 0.f,
			0.f,  0.f,  1.f };

		glGenBuffers(1, &vertexPosObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
		glBufferData(GL_ARRAY_BUFFER, 9 * 4, vVertices, GL_STATIC_DRAW);

		
		glGenBuffers(1, &vertexColorObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexColorObject);
		glBufferData(GL_ARRAY_BUFFER, 9 * 4, vVertexColors, GL_STATIC_DRAW);
	}

	~Triangle()
	{
		glDeleteBuffers(1, &vertexPosObject);
	}

	void Draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexColorObject);
		glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};
