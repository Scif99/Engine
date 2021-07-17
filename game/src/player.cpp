#pragma once
#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"


player::player()
	:m_speed(0.f), m_timer(0.f) {}

player::~player() {}

void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(0.f, 0.5, 10.f));
	m_object->animated_mesh()->set_default_animation(1);
}
void player::on_update(const engine::timestep& timestep)
{
	m_object->set_position(m_object->position() += m_object->forward() * m_speed *
		(float)timestep);

	m_object->set_rotation_amount(atan2(m_object->forward().x,m_object->forward().z)); // Rotate model towards direction

	m_object->animated_mesh()->on_update(timestep); //Moved from example_layer::on_update



	if (engine::input::key_pressed(engine::key_codes::KEY_W))
	{
		m_speed = 1.f;
	}

	if (engine::input::key_pressed(engine::key_codes::KEY_S))
	{
		m_speed = 0.f;
	}

	if (engine::input::key_pressed(engine::key_codes::KEY_1)) // left
		turn(1.0f * timestep);




}

void player::update_camera(engine::perspective_camera& camera)
{
	// Constants for determining camera view (see lab 4)
	float A = 2.f;
	float B = 3.f;
	float C = 6.f;

	glm::vec3 cam_pos = m_object->position() - B * glm::normalize(m_object->forward()) + A * glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 look_at = m_object->position() + C * glm::normalize(m_object->forward());
	//look_at.y = 0.f;
	camera.set_view_matrix(cam_pos, look_at);
}

//void player::move(e_direction direction, engine::timestep ts) {}
void player::turn(float angle)
{
	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f, 0.f))); // Rotate forward vector about y axis
}

