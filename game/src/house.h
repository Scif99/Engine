#pragma once
#include <engine.h>

class house
{
public:
	house();
	~house();
	void initialise(float height, float width, float x_pos, float z_pos, int cone_triangles);

	void on_render(const engine::ref<engine::shader>& shader);


private:
	engine::ref<engine::cuboid> m_base;
	engine::ref<engine::cone> m_roof;
	engine::ref<engine::cuboid> m_chimney;


	engine::ref<engine::material> m_material;

	float m_height, m_width, m_rotation;
	glm::vec3 m_position;

	glm::mat4 base_transform, roof_transform, chimney_transform;

	std::vector<glm::vec3> m_vertices;

	//bool check_collision(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 r0, glm::vec3 r1, glm::vec3& intersection, double& t);
};
