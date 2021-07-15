#include "collectible.h"
collectible::collectible(const engine::game_object_properties props) : engine::game_object(props)
{}
collectible::~collectible()
{}
void collectible::init()
{
	m_is_active = true;
}

void collectible::update(glm::vec3 c, float dt)
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
engine::ref<collectible> collectible::create(const engine::game_object_properties& props)
{
	return std::make_shared<collectible>(props);
}
