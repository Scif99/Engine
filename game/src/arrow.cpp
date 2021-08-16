#include "pch.h"
#include "arrow.h"
#include "ballistic.h"


arrow::arrow()
{}

arrow::~arrow()
{}

void arrow::initialise(float height, float width, float rotation, float x_pos, float z_pos, float thickness)
{
	m_height = height;
	m_width = width;
	m_rotation = rotation;
	m_position = glm::vec3(x_pos, m_height / 2.0f, z_pos);

	m_shaft = engine::cuboid::create(glm::vec3(thickness / 2.f, thickness/ 2.f, height / 2.f), false);

	std::vector<glm::vec3> tetrahedron_vertices;
	tetrahedron_vertices.push_back(glm::vec3(-0.1f, 0.1f, -0.1f));//0
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.2f, 0.f));//1
	tetrahedron_vertices.push_back(glm::vec3(-0.2f, 0.2f, 0.f)); //2
	tetrahedron_vertices.push_back(glm::vec3(-0.2f, 0.f, 0.f)); //3
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.f, 0.f)); //4
	m_head =engine::tetrahedron::create(tetrahedron_vertices);

	m_material = engine::material::create(0.5f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1.0f);


	// Add triangles to the end of shaft

	glm::mat4 overall_transform(1.0f);
	overall_transform = glm::translate(overall_transform, m_position);

	shaft_transform = glm::translate(overall_transform, glm::vec3(0.f, 0.f, 0.f));
	//shaft_transform = glm::rotate(overall_transform, -0.5f * glm::pi<float>(), glm::vec3(1.f, 0.f, 0.f));
	head_transform = glm::translate(overall_transform, glm::vec3(0.f, 0.f, -height / 2.f));


}



void arrow::on_render(const engine::ref<engine::shader>& shader)
{
	m_material->submit(shader);

	engine::renderer::submit(shader, m_shaft->mesh(), shaft_transform);
	engine::renderer::submit(shader, m_head->mesh(), head_transform);
	

	//engine::renderer::submit(shader, );
}
