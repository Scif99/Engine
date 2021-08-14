#include "pch.h"
#include "cone.h"
#include <engine.h>

// Class to create a cone in the x-z plane, of a given radius and height


//engine::cone::cone(float radius, float height) : m_radius(radius), m_height(height)
//{
//	// Define the normals for the base circle and angled triangles seperately
//
//
//	glm::vec3 circle_centre = glm::vec3(0.f, 0.f, 0.f);
//	glm::vec3 apex = glm::vec3(0.f, m_height, 0.f);
//
//	std::vector<glm::vec3> normals;
//
//	std::vector<mesh::vertex> cone_vertices;
//
//	// Circle Vertices
//
//	for (int i = 0; i < 30 + 1; i++) // Need to loop 1 extra over, else the first triangle is skipped
//	{
//		float theta = (i / 30.f) * 2 * engine::PI;
//
//		engine::mesh::vertex vertex(
//			0.5f * glm::vec3(m_radius * cos(theta), 0.f, m_radius * sin(theta)),  //Position (local to the game object derived from this shape)
//			glm::vec3(0.f,-1.f,0.f),																  // Normal
//			glm::vec2(0.f,0.f));												  //Need to fix texture coords
//		cone_vertices.push_back(vertex);
//	}
//
//	
//
//	for (int i = 0;i < 30 + 1;++i)
//	{
//		float theta = (i / 30.f) * 2 * engine::PI;
//
//		glm::vec3 normal = cone_vertices[i].position;
//
//		// position		 normal          tex coord
//		cone_vertices.push_back({ apex, normal, { 0.f, 0.f } });
//		cone_vertices.push_back({ apex, normal, { 0.f, 0.f } });
//		cone_vertices.push_back({ apex, normal, { 0.f, 0.f } });
//	}
//	// Apex of cone
//
//
//	// Indices for base circle
//	std::vector<uint32_t> cone_indices;
//	for (int i = 1; i < 30 + 2;++i) // Need to loop back over for the first triangle
//	{
//		cone_indices.push_back(0);
//		cone_indices.push_back(1 + i);
//		cone_indices.push_back(i);
//	}
//
//	// Indices for triangles connected to raised vertex
//	for (int i = 1;i < 32 + 2;++i)
//	{
//		cone_indices.push_back(32+1);
//		cone_indices.push_back(1 + i);
//		cone_indices.push_back(i);
//
//	}
//
//
//	m_mesh = engine::mesh::create(cone_vertices, cone_indices);
//}
//
//
//engine::cone::~cone() {}
//
//
//engine::ref<engine::cone> engine::cone::create(float
//	radius, float height)
//{
//	return std::make_shared<engine::cone>(radius, height);
//}
