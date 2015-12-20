#pragma once
#include <gl/glew.h>
#include <vector>
#include <SDL2/SDL_stdinc.h>

inline void push_indices(std::vector<GLushort>& indices, int sectors, int r, int s) {
	int curRow = r * sectors;
	int nextRow = (r + 1) * sectors;

	indices.push_back(curRow + s);
	indices.push_back(nextRow + s);
	indices.push_back(nextRow + (s + 1));

	indices.push_back(curRow + s);
	indices.push_back(nextRow + (s + 1));
	indices.push_back(curRow + (s + 1));
}

void createSphere(std::vector<glm::vec3>& vertices, std::vector<GLushort>& indices, std::vector<glm::vec3> &normals, std::vector<glm::vec2>& texcoords,
	float radius, unsigned int rings, unsigned int sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	
	for (int r = 0; r < rings; ++r) {
		for (int s = 0; s < sectors; ++s) {
			float const y = sin(-glm::half_pi<float>() + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			texcoords.push_back(glm::vec2(s*S, r*R));
			vertices.push_back(glm::vec3(x, y, z) * radius);
			normals.push_back(glm::vec3(x, y, z));
			push_indices(indices, sectors, r, s);
		}
	}
}


class SphereMesh
{
private:
	GLuint m_vertexBuffer;
	GLuint m_colorBuffer;
	GLuint m_indicesBuffer;
	int m_indicesCount;

public:
	SphereMesh(float radius, unsigned int rings, unsigned int sectors)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texcoords;
		std::vector<GLushort> indices;

		createSphere(vertices, indices, normals, texcoords, radius, rings, sectors);

		// HACK! correct out of bounds indices! 
		// TODO: replace sphere creation code instead of this
		for (int i = 0; i < indices.size(); i++)
		{
			if (indices[i] >= vertices.size())
				indices[i] = indices[i] % vertices.size();
		}

		printf("vertices = %d,  normals = %d, indices = %d\n", vertices.size(), normals.size(), indices.size());

		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) *3, vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat)*3, normals.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_indicesBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
		m_indicesCount = indices.size();
	}

	~SphereMesh()
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

		glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_SHORT, (void*)0);
	}
};