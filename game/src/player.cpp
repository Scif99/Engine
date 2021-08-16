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
	m_object->animated_mesh()->set_default_animation(1);
	// Starting animation?

	m_walking = false;
	m_sprinting = false;
	m_idle = false;
}
void player::on_update(const engine::timestep& timestep)
{
	// Processing mouse movement
	auto [mouse_delta_x, mouse_delta_y] = engine::input::mouse_position();

	process_mouse(mouse_delta_x, mouse_delta_y);
		



	//m_object->set_position(m_object->position() += m_object->forward() * m_speed *
		//(float)timestep);


	float theta = atan2(m_object->forward().x, m_object->forward().z); // Align object with camera
	m_object->animated_mesh()->on_update(timestep); //Moved from example_layer::on_update


	 // Process movement
	if (engine::input::key_pressed(engine::key_codes::KEY_W))
	{
			m_speed = 4.f;
			m_object->set_position(m_object->position() + m_speed * timestep * m_object->forward());
			if (!m_walking)
			{
				m_object->animated_mesh()->switch_animation(4);
				m_walking = true;
			}
	}


	if (engine::input::key_pressed(engine::key_codes::KEY_S))
	{
		float move_speed = 3.f;
		m_object->set_position(m_object->position()  - move_speed * timestep * m_object->forward());
		if (!m_walking)
		{
			m_object->animated_mesh()->switch_animation(1);
			m_walking =true;
		}
	}

	if (engine::input::key_pressed(engine::key_codes::KEY_A))
	{
		strafe_left(timestep);
		if (engine::input::key_pressed(engine::key_codes::KEY_W))
		{
			theta -= glm::pi<float>() / 4.f;

		}
	}

	if (engine::input::key_pressed(engine::key_codes::KEY_D))
	{
		strafe_right(timestep);
		if (engine::input::key_pressed(engine::key_codes::KEY_W))
		{
			theta += glm::pi<float>() / 4.f;

		}
	}


	if (engine::input::key_pressed(engine::key_codes::KEY_SPACE))
	{
		jump();
	}

	if (m_timer > 0.f)
	{
		m_timer -= float(timestep);
		if (m_timer < 0.f)
		{
			m_object->animated_mesh()->switch_root_movement(false);

			if (m_speed > 0)
			{
				m_object->animated_mesh()->switch_animation(1);
			}
			else { m_object->animated_mesh()->switch_animation(2); }
		}
	}


	m_object->set_rotation_amount(theta); // Rotate model towards direction

	// NED TO FIX
	if (m_object->position().y != 0.5f)
	{
		// Stop character from flying lol
		m_object->set_position(glm::vec3(m_object->position().x, 0.5f, m_object->position().z));
	}

	// Otherwise, if stationary
	else
	{
		m_speed = 0.f;
		if (!m_idle)
		{
			m_object->animated_mesh()->switch_animation(2);
			m_idle = true;
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



void player::turn(float angle_x, float angle_y)
{
	m_object->set_forward(glm::rotate(m_object->forward(),  - angle_x, glm::vec3(0.f, 1.f, 0.f))); // left-right rotation. Note the negative sign is to prevent inverted controls

	glm::vec3 m_right_vector = glm::normalize(glm::cross(m_object->forward(), glm::vec3(0.f, 1.f, 0.f)));
	m_object->set_forward(glm::rotate(m_object->forward(), angle_y, m_right_vector)); // up-down rotation



}


void player::process_mouse(float delta_x, float delta_y)
{
	// Sensitivity of the mouse
	//float sensitivity = 0.003f;
	float sensitivity = 0.003f;
	delta_x *= sensitivity;
	delta_y *= sensitivity;

	turn(delta_x, delta_y);


}

void player::strafe_right(engine::timestep ts)
{
	glm::vec3 m_right_vector = glm::normalize(glm::cross(m_object->forward(), glm::vec3(0.f,1.f,0.f)) );
	m_object->set_position(m_object->position() + 3.f * ts * m_right_vector);
	
	m_object->animated_mesh()->switch_animation(5);

}

void player::strafe_left(engine::timestep ts)
{
	glm::vec3 m_left_vector =  - glm::normalize(glm::cross(m_object->forward(), glm::vec3(0.f, 1.f, 0.f)));
	m_object->set_position(m_object->position() + 3.f * ts * m_left_vector);
	m_object->animated_mesh()->switch_animation(6);

}

void player::jump()
{
	
	m_timer = m_object->animated_mesh()->animations().at(3)->mDuration;
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(3);

}

