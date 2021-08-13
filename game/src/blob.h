#pragma once
#include <engine.h>

//class arrow;

class blob
{
public:
	blob();
	~blob();
	//bool is_goal_scored(football& football);
	void initialise(float x_pos, float z_pos, float health=50);
	//bool goal_scored(const football& ball);

	void on_update(float dt);
	void on_render(const engine::ref<engine::shader>& shader);
	void attack(float dt);
	glm::vec3 position() const { return m_position; }
	float health() const { return m_health; }
	void set_health(int h) { m_health = h; }
	bool dead() const { return m_is_dead; }
	engine::ref<engine::sphere> top() const { return m_top; }
	engine::ref<engine::sphere> bottom() const { return m_bottom; }

private:
	engine::ref<engine::sphere> m_top;
	engine::ref<engine::sphere> m_bottom;

	engine::ref<engine::material> m_material;

	glm::vec3 m_position;

	glm::mat4 top_transform, bottom_transform;

	std::vector<glm::vec3> m_vertices;
	float m_health;
	bool m_is_dead;
	bool check_collision(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 r0, glm::vec3 r1, glm::vec3& intersection, double& t);
};
