#pragma once
#include <engine.h>
class collectible_list : public engine::game_object {
public:
	collectible_list(const engine::game_object_properties props);
	~collectible_list();
	void init();
	void update(glm::vec3 c, float dt);
	bool active() { return m_is_active; }
	static engine::ref<pickup> create(const engine::game_object_properties& props);
private:
	std::vector<shared_ptr<collectible>> pickups;

};
