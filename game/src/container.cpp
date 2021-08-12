#pragma once
#include "pch.h"
#include "container.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"


container::container(const engine::game_object_properties props) : engine::game_object(props)
{}
container::~container()
{}
void container::init()
{
	s_active = false;
	m_max_time = 0.5;
	m_timer = 0;
	s_open = false;
}

void container::update(glm::vec3 c, float time_step)
{

	if (s_active)
	{
		glm::vec3 v = c - position();
		float d = glm::length(v);
		m_timer += (float)time_step;


		set_position(position() + time_step * glm::vec3(0.f, 1.f, 0.f));
		if (m_timer > m_max_time)
		{
			s_active = false;
			s_open = true;

		}
	}

	else
	{
		if (s_open)
		{
			set_rotation_amount(rotation_amount() + time_step * 1.5f);
		}
		else return;
	}

}

void container::activate()
{
	s_active = true;
}
engine::ref<container> container::create(const engine::game_object_properties& props)
{
	return std::make_shared<container>(props);
}