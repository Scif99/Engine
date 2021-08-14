#include "pch.h"
#include "pickup_container.h"


pickup_container::pickup_container()
{}

pickup_container::~pickup_container()
{}

void pickup_container::initialise()
{

	m_position = glm::vec3(2.1,0.5,3.f);


	m_circle = engine::circle::create(1.f);

	std::vector<glm::vec3> triangle_vertices;
	triangle_vertices.push_back(glm::vec3(0.f, 0.5f, 0.f));//base
	triangle_vertices.push_back(glm::vec3(0.f, 0.5f, 0.5f));//1
	triangle_vertices.push_back(glm::vec3(0.f, 1.0f, 0.25f)); //2
	m_triangle = engine::triangle::create(triangle_vertices);


	m_material = engine::material::create(0.5f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1.0f); //colour



	glm::mat4 overall_transform(1.0f);
	overall_transform = glm::translate(overall_transform, m_position);
	//overall_transform = glm::rotate(overall_transform, m_rotation, glm::vec3(0.f, 1.f, 0.f));

	triangle_transform = glm::translate(overall_transform, glm::vec3(  0.f, 0.f, 0.f));
	//triangle_transform = glm::rotate(overall_transform, -0.5f * glm::pi<float>(), m_circle->cen);
	circle_transform = glm::translate(overall_transform, glm::vec3( 0.f, 0.f, 0.f));

	


}

void pickup_container::on_update(float dt)
{

}

void pickup_container::on_render(const engine::ref<engine::shader>& shader)
{
	m_material->submit(shader);

	engine::renderer::submit(shader, m_triangle->mesh(), triangle_transform);
	engine::renderer::submit(shader, m_circle->mesh(), circle_transform);


	//engine::renderer::submit(shader, );
}

