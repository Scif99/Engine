#pragma once
#include <engine.h>




class container : public engine::game_object {
public:
	container(const engine::game_object_properties props);
	~container();
	void init();
	void update(glm::vec3 c, float dt);
	void activate();
	bool activated() { return s_open; }
	static engine::ref<container> create(const engine::game_object_properties& props);
private:
	bool s_active; // Indicates if the container has been opened by the player
	float m_timer;
	float m_max_time;
	bool s_open;
	bool top; // Identifies whether tetrahedron is top or bottom part of container
};
