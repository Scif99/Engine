#include "pch.h"
#include "circle.h"
#include <engine.h>

engine::circle::circle(glm::vec3 centre) : m_centre(centre)
{
	// Normal is the y-axis
	glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);

	std::vector<mesh::vertex> circle_vertices;
	for (int i = 0; i < 30; i++)
	{
		float theta = (i / 30.f) * 2 * engine::PI;

		engine::mesh::vertex vertex(
		0.5f *glm::vec3(cos(theta),0.f, sin(theta)), // These vertices will be local to the position of the prop_
		normal,
		glm::vec2(cos(theta) / 2.0f + 0.25f, sin(theta) / 2.0f + 0.25f));
		circle_vertices.push_back(vertex);
	}

	std::vector<uint32_t> circle_indices;
	for (int i = 1; i < 30 + 1;++i)
	{
		circle_indices.push_back(0);
		circle_indices.push_back(1+i);
		circle_indices.push_back(i);
	}


	m_mesh = engine::mesh::create(circle_vertices, circle_indices);
}


engine::circle::~circle() {}


engine::ref<engine::circle> engine::circle::create(glm::vec3
	centre)
{
	return std::make_shared<engine::circle>(centre);
}

