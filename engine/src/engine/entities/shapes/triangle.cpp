#include "pch.h"
#include "triangle.h"
#include <engine.h>

// Class to create a triangle in the 


engine::triangle::triangle(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	// Normal is the y-axis
	glm::vec3 normal = (glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) -
		vertices.at(1)));

	std::vector<mesh::vertex> triangle_vertices
	{
		//front
	// position			normal			tex coord
	{ vertices.at(0), normal, { 0.f, 0.f } },
	{ vertices.at(2), normal, { 1.f, 0.f } },
	{ vertices.at(1), normal, { 0.5f, 1.f } },

	};


	std::vector<uint32_t> triangle_indices = { 0,1,2, 0,2,1 }; //Double winding to render both sides?




	m_mesh = engine::mesh::create(triangle_vertices, triangle_indices);

}


engine::triangle::~triangle() {}


engine::ref<engine::triangle> engine::triangle::create(std::vector<glm::vec3>
	vertices)
{
	return std::make_shared<engine::triangle>(vertices);
}
