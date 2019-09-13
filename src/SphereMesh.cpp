#include <SphereMesh.h>

#include "RenderConstants.h"

#include <glm/gtc/constants.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <vector>

namespace {
	// from http://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c
	inline void push_indices(std::vector<GLushort> &indices, int sectors, int r, int s) {
		int curRow = r * sectors;
		int nextRow = (r + 1) * sectors;

		indices.push_back(curRow + s);
		indices.push_back(nextRow + s);
		indices.push_back(nextRow + (s + 1));

		indices.push_back(curRow + s);
		indices.push_back(nextRow + (s + 1));
		indices.push_back(curRow + (s + 1));
	}

	void createSphere(std::vector<glm::vec3> &vertices, std::vector<GLushort> &indices, std::vector<glm::vec3> &normals,
					  std::vector<glm::vec2> &texcoords,
					  float radius, unsigned int rings, unsigned int sectors) {
		float const R = 1.f / (float) (rings - 1);
		float const S = 1.f / (float) (sectors - 1);

		for (unsigned int r = 0; r < rings; ++r) {
			for (unsigned int s = 0; s < sectors; ++s) {

				float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
				float const x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
				float const z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

				texcoords.push_back(glm::vec2(s * S, r * R));
				vertices.push_back(glm::vec3(x, y, z) * radius);
				normals.push_back(glm::vec3(x, y, z));
				push_indices(indices, sectors, r, s);
			}
		}
	}
}

SphereMesh::SphereMesh(float radius, unsigned int rings, unsigned int sectors)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<GLushort> indices;

	createSphere(vertices, indices, normals, texcoords, radius, rings, sectors);

	// HACK! correct out of bounds indices!
	// TODO: replace sphere creation code instead of doing this
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		if (indices[i] >= vertices.size())
			indices[i] = indices[i] % vertices.size();
	}

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

SphereMesh::~SphereMesh()
{
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_colorBuffer);
	glDeleteBuffers(1, &m_indicesBuffer);
}

void SphereMesh::Draw() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(RenderConstants::PositionAttributeLocation, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(RenderConstants::PositionAttributeLocation);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glVertexAttribPointer(RenderConstants::NormalAttributeLocation, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(RenderConstants::NormalAttributeLocation);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);

	glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_SHORT, (void*)0);
}
