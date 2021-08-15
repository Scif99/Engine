#include "pch.h"
#include "cone.h"
#include <engine.h>

// Class to create a cone in the x-z plane, of a given radius, height and number of stacks


engine::cone::cone(float radius, float height, int stacks) : m_radius(radius), m_height(height), m_stacks(stacks)
{

	glm::vec3 circle_centre = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 apex = glm::vec3(0.f, m_height, 0.f);


	float angle_step = 2.f * engine::PI / float(stacks);

	for (int i = 0; i < m_stacks ; i++) // Need to loop 1 extra over, else the first triangle is skipped
	{
		float theta = i * angle_step;

		m_vertices.push_back(glm::vec3(0.5f * glm::vec3(m_radius * cos(theta), 0.f, m_radius * sin(theta) ) )); // Creates the vertices around base circle
	}



	std::vector<glm::vec3> normals;

	for (int i = 0; i < m_stacks; i++) // Need to loop 1 extra over, else the first triangle is skipped
	{

		normals.push_back(glm::cross(apex - m_vertices[(i+1)%m_stacks],apex - m_vertices[i]));
	}




	std::vector<mesh::vertex> cone_vertices;
	// Angled vertices
	for (int i = 0; i < m_stacks; i++) // Need to loop 1 extra over, else the first triangle is skipped
	{

		cone_vertices.push_back(engine::mesh::vertex(apex, normals.at(i), { 0.f, 0.f }));
		cone_vertices.push_back(engine::mesh::vertex(m_vertices[(i+1)% m_stacks], normals.at(i), { 0.f, 0.f }));
		cone_vertices.push_back(engine::mesh::vertex(m_vertices[i], normals.at(i), { 0.f, 0.f }));
	}

	glm::vec3 circle_normal = glm::vec3(0.f, 1.f, 0.f);
	// At this point, there are 3 * m_stack vertices 
	engine::mesh::vertex centre(glm::vec3(0.f, 0.f, 0.f), circle_normal, glm::vec2(0.f, 0.f));

	cone_vertices.push_back(centre); //This vertex will be used when we provide the indices of the base circle triangles

	// Base vertices
	for (int i = 0; i < m_stacks; i++) // Need to loop 1 extra over, else the first triangle is skipped
	{

		cone_vertices.push_back(engine::mesh::vertex(circle_centre, circle_normal, { 0.f, 0.f }));
		cone_vertices.push_back(engine::mesh::vertex(m_vertices[(i + 1) % m_stacks], circle_normal, { 0.f, 0.f }));
		cone_vertices.push_back(engine::mesh::vertex(m_vertices[i], circle_normal, { 0.f, 0.f }));

	}

	// Indices for angled triangles
	std::vector<uint32_t> cone_indices;
	for (int i = 0;i < 3 * m_stacks;i++)
	{
		cone_indices.push_back((i));
	}


	// Indices for base triangles
	// We loop over the vertices of the base triangles only
	for (int i = (3* m_stacks) + 1; i < (6* m_stacks) + 2;++i) // Need to loop back over for the first triangle
	{
		// Choose a winding s.t. the circle is rendered facing 'outwards'
		cone_indices.push_back((3 * m_stacks));
		cone_indices.push_back(i+1);
		cone_indices.push_back(i);
	}


	m_mesh = engine::mesh::create(cone_vertices, cone_indices);
}


engine::cone::~cone() {}


engine::ref<engine::cone> engine::cone::create(float
	radius, float height, int stacks)
{
	return std::make_shared<engine::cone>(radius, height, stacks);
}
