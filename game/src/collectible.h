#pragma once
#include <engine.h>
class collectible : public engine::game_object {
public:
	collectible(const engine::game_object_properties props);
	~collectible();
	void init();
	void update(glm::vec3 c, float dt);
	bool active() { return m_is_active; }
	static engine::ref<collectible> create(const engine::game_object_properties& props);
private:
	bool m_is_active; // Indicates if the collectible is active

};
