#include "pch.h"
#include "circle.h"
#include <engine.h>

// Class to create a circle in the x-z plane, of a given radius


engine::circle::circle(float radius) : m_radius(radius)
{
	// Normal is the y-axis
	glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);

	std::vector<mesh::vertex> circle_vertices;

	engine::mesh::vertex centre(glm::vec3(0.f, 0.f, 0.f), normal, glm::vec2(0.f, 0.f));
	circle_vertices.push_back(centre);

	for (int i = 0; i < 30+1; i++) // Need to loop 1 extra over, else the first triangle is skipped
	{
		float theta = (i / 30.f) * 2 * engine::PI;

		engine::mesh::vertex vertex(
		0.5f * glm::vec3(m_radius*cos(theta),0.f, m_radius * sin(theta)), // These vertices will be local to the game object derived from this shape
		normal,
		glm::vec2(cos(theta) / 2.0f + 0.25f, sin(theta) / 2.0f + 0.25f));
		circle_vertices.push_back(vertex);
	}

	std::vector<uint32_t> circle_indices;
	for (int i = 1; i < 30 + 2;++i) // Need to loop back over for the first triangle
	{
		circle_indices.push_back(0);
		circle_indices.push_back(i+1);
		circle_indices.push_back(i);
	}


	m_mesh = engine::mesh::create(circle_vertices, circle_indices);
}


engine::circle::~circle() {}


engine::ref<engine::circle> engine::circle::create(float
	radius)
{
	return std::make_shared<engine::circle>(radius);
}

