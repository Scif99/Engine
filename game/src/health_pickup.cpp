#include "health_pickup.h"
health_pickup::health_pickup(const engine::game_object_properties props) : engine::game_object(props)
{}
health_pickup::~health_pickup()
{}
void health_pickup::init()
{
	m_is_active = true;
}

void health_pickup::update(glm::vec3 c, float dt)
{
	
	set_rotation_amount(rotation_amount() + dt * 1.5f);

	glm::vec3 v = c - position();
	float d = glm::length(v);

	
	if (d < 1.0f)
	{
		// Play some events..
		m_is_active = false;
	}

}
engine::ref<health_pickup> health_pickup::create(const engine::game_object_properties & props)
{
	return std::make_shared<health_pickup>(props);
}