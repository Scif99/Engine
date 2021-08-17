#include "pch.h"
#include "house.h"


house::house()
{}

house::~house()
{}

void house::initialise(float height, float width, float x_pos, float z_pos, int cone_triangles)
{
	m_height = height;
	m_width = width;
	m_position = glm::vec3(x_pos, m_height / 2.0f, z_pos);

	m_base = engine::cuboid::create(glm::vec3( width /2.f, height / 2.f, width / 2.f), false);
	m_chimney = engine::cuboid::create(glm::vec3(0.07f, 0.3f, 0.07f), false);

	float hyp = std::sqrt(2.f) * width;
	m_roof = engine::cone::create(hyp, width / 4.f, cone_triangles);


	m_material = engine::material::create(0.5f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1.0f);

	//m_vertices.push_back(glm::vec3(-width / 2.0f, -height / 2.0f, 0.f));
	//m_vertices.push_back(glm::vec3(width / 2.0f, -height / 2.0f, 0.f));
	//m_vertices.push_back(glm::vec3(width / 2.0f, height / 2.0f, 0.f));
	//m_vertices.push_back(glm::vec3(-width / 2.0f, height / 2.0f, 0.f));

	glm::mat4 overall_transform(1.0f);
	overall_transform = glm::translate(overall_transform, m_position);

	base_transform = glm::translate(overall_transform, glm::vec3(0.f, 0.f, 0.f));

	roof_transform = glm::translate(overall_transform, glm::vec3(0.f, m_height / 2.f, 0.f));
	chimney_transform = glm::translate(overall_transform, glm::vec3(m_width / 3.f, m_height / 2.f + 0.3f, m_width / 3.f));

	

	//for (uint32_t i = 0; i < m_vertices.size(); i++)
	//{
	//	m_vertices.at(i) = overall_transform * glm::vec4(m_vertices.at(i), 1.f);
	//}
}


void house::on_render(const engine::ref<engine::shader>& shader)
{
	m_material->submit(shader);

	engine::renderer::submit(shader, m_base->mesh(), base_transform);
	engine::renderer::submit(shader, m_roof->mesh(), roof_transform);
	engine::renderer::submit(shader, m_chimney->mesh(), chimney_transform);

	//engine::renderer::submit(shader, );
}
