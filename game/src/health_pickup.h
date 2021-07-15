#pragma once
#include <engine.h>




class health_pickup : public engine::game_object {
public:
	health_pickup(const engine::game_object_properties props);
	~health_pickup();
	void init();
	void update(glm::vec3 c, float dt);
	bool active() { return m_is_active; }
	static engine::ref<health_pickup> create(const engine::game_object_properties& props);
private:
	bool m_is_active; // Indicates if pickup is active
	
};
