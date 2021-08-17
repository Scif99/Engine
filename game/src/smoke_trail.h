#pragma once
#include <engine.h>

class smoke_trail
{

public:

	smoke_trail();
	~smoke_trail();
	void initialise(glm::vec3 smoke_trail_start, glm::vec3 smoke_trail_direction, int num_total_particles, float width, float height); //Remove num?
	void on_update(const engine::timestep& timestep);
	void on_render(const engine::perspective_camera& camera, engine::ref<engine::shader> shader);
	void load_texture(char* filename);
	bool is_active() { return s_active; }

private:

	engine::ref<engine::texture_2d> m_texture;
	engine::ref<engine::mesh> m_quad;
	std::vector<engine::particle> m_particles;
	int m_num_alive;
	glm::vec3 m_start, m_direction;
	float m_width, m_height;
	int m_num_total_particles;
	bool s_active;
	float accumulatedTime = 0.0f;

	void clear();
	void add_particle();
};
