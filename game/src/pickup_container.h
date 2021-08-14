#pragma once
#include <engine.h>



class pickup_container
{
public:
	pickup_container();
	~pickup_container();
	//bool is_goal_scored(football& football);
	void initialise();
	void on_update(float dt);
	void on_render(const engine::ref<engine::shader>& shader);

	engine::ref<engine::triangle> triangle() const { return m_triangle; }
	engine::ref<engine::circle> circle() const { return m_circle; }

private:
	engine::ref<engine::triangle> m_triangle;
	engine::ref<engine::circle> m_circle;

	engine::ref<engine::material> m_material;

	float m_height, m_width, m_rotation;
	glm::vec3 m_position;

	glm::mat4 circle_transform, triangle_transform;

	std::vector<glm::vec3> m_vertices;

};
