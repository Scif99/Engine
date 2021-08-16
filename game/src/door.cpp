#include "pch.h"
#include "door.h"



door::door()
{}

door::~door()
{}

void door::initialise(float height, float width, float rotation, float x_pos, float y_pos, float z_pos, float thickness)
{
	m_height = height;
	m_width = width;
	m_rotation = rotation;
	m_position = glm::vec3(x_pos, y_pos + m_height / 2.0f, z_pos);

	m_vertical_bar = engine::cuboid::create(glm::vec3(thickness / 2.f, height / 2.f, thickness / 2.f), false);
	m_horizontal_bar = engine::cuboid::create(glm::vec3(width / 2.f, thickness / 2.f, thickness / 2.f), false);

	m_material = engine::material::create(0.5f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1.0f);

	m_vertices.push_back(glm::vec3(-width / 2.0f, -height / 2.0f, 0.f));
	m_vertices.push_back(glm::vec3(width / 2.0f, -height / 2.0f, 0.f));
	m_vertices.push_back(glm::vec3(width / 2.0f, height / 2.0f, 0.f));
	m_vertices.push_back(glm::vec3(-width / 2.0f, height / 2.0f, 0.f));

	glm::mat4 overall_transform(1.0f);
	overall_transform = glm::translate(overall_transform, m_position);
	overall_transform = glm::rotate(overall_transform, m_rotation * glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f));

	left_transform = glm::translate(overall_transform, glm::vec3(-m_width / 2.f, 0.f, 0.f));
	right_transform = glm::translate(overall_transform, glm::vec3(m_width / 2.f, 0.f, 0.f));
	top_transform = glm::translate(overall_transform, glm::vec3(0.f, m_height / 2.f - thickness / 2.0f, 0.f));

	for (uint32_t i = 0; i < m_vertices.size(); i++)
	{
		m_vertices.at(i) = overall_transform * glm::vec4(m_vertices.at(i), 1.f);
	}
}


void door::on_render(const engine::ref<engine::shader>& shader)
{
	m_material->submit(shader);

	engine::renderer::submit(shader, m_vertical_bar->mesh(), left_transform);
	engine::renderer::submit(shader, m_vertical_bar->mesh(), right_transform);
	engine::renderer::submit(shader, m_horizontal_bar->mesh(), top_transform);

	//engine::renderer::submit(shader, );
}
