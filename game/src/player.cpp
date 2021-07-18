#pragma once
#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"


player::player()
	:m_speed(0.f), m_timer(0.f), fps(false){}

player::~player() {}

void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(0.f, 0.5, 10.f));
	m_object->animated_mesh()->set_default_animation(2);
	// Starting animation?
}
void player::on_update(const engine::timestep& timestep)
{
	m_object->set_position(m_object->position() += m_object->forward() * m_speed *
		(float)timestep);

	m_object->set_rotation_amount(atan2(m_object->forward().x,m_object->forward().z)); // Rotate model towards direction

	m_object->animated_mesh()->on_update(timestep); //Moved from example_layer::on_update


	 // Proccess movement
	if (engine::input::key_pressed(engine::key_codes::KEY_W))
	{
		if (sprint)
		{
			m_speed = 5.f;
			m_object->animated_mesh()->switch_animation(4);
		}

		else
		{
			m_speed = 3.f;
			m_object->animated_mesh()->switch_animation(1);
		}

	}

	// Sprint?
	if (engine::input::key_pressed(engine::key_codes::KEY_LEFT_SHIFT))
	{
		if (!sprint) { sprint = true; }
		else if (m_speed>0) // If already moving
		{
			sprint = false;
			m_speed = 3.f;
			m_object->animated_mesh()->switch_animation(1);
		}
	}


	if (engine::input::key_pressed(engine::key_codes::KEY_S))
	{
		m_speed = 0.f;
		m_object->animated_mesh()->switch_animation(2);
	}

	if (engine::input::key_pressed(engine::key_codes::KEY_A)) // left
		turn(1.0f * timestep);

	if (engine::input::key_pressed(engine::key_codes::KEY_D)) // Right
		turn(-1.0f * timestep);

	if (engine::input::key_pressed(engine::key_codes::KEY_SPACE))
		jump();

	if (m_timer > 0.f)
	{
		m_timer -= float(timestep);
		if (m_timer < 0.f)
		{
			m_object->animated_mesh()->switch_root_movement(false);
			if (sprint)
			{
				m_object->animated_mesh()->switch_animation(1); // If sprint finished, revert back to regular run
				sprint = false;
			}
			m_speed = (sprint) ? 3.f : 0.f; // If sprinting, revert back to running
		}
	}
}

void player::update_camera(engine::perspective_camera& camera)
{
	// Constants for determining camera view (see lab 4)
	float A = (is_fps()) ? 1.f : 2.f;
	float B = (is_fps() ) ? -0.5f : 3.f; // ternary operator: variable = (condition) ? expressionTrue : expressionFalse;
	float C = 6.f;

	glm::vec3 cam_pos = m_object->position() - B * glm::normalize(m_object->forward()) + A * glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 look_at = (is_fps()) ? cam_pos + C * glm::normalize(m_object->forward()) : m_object->position() + C * glm::normalize(m_object->forward());
	//look_at.y = 0.f;
	camera.set_view_matrix(cam_pos, look_at);
}






void player::turn(float angle)
{
	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f, 0.f))); // Rotate forward vector about y axis
}



void player::jump()
{
	m_timer = m_object->animated_mesh()->animations().at(3)->mDuration;
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(3);

}

