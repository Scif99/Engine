#pragma once
#include <engine.h>

//class arrow;

class arrow
{
public:
	arrow();
	~arrow();
	//bool is_goal_scored(football& football);
	void initialise(float height, float width, float rotation, float x_pos, float z_pos, float thickness);
	//bool goal_scored(const football& ball);
	void on_render(const engine::ref<engine::shader>& shader);

	engine::ref<engine::cuboid> shaft() const { return m_shaft; }
	engine::ref<engine::tetrahedron> head() const { return m_head; }

private:
	engine::ref<engine::cuboid> m_shaft;
	engine::ref<engine::tetrahedron> m_head;

	engine::ref<engine::material> m_material;

	float m_height, m_width, m_rotation;
	glm::vec3 m_position;

	glm::mat4 left_transform, right_transform, top_transform;

	std::vector<glm::vec3> m_vertices;

	bool check_collision(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 r0, glm::vec3 r1, glm::vec3& intersection, double& t);
};
