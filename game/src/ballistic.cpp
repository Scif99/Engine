#pragma once
#include "pch.h"
#include "ballistic.h"

ballistic::ballistic() {}
ballistic::~ballistic() {}

void ballistic::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;

}

void ballistic::on_render(const engine::ref<engine::shader>& shader)
{
	engine::renderer::submit(shader, m_object);

}

void ballistic::on_update(const engine::timestep& time_step)
{
	m_object->set_position(m_object->position() + m_object->forward() * float(time_step) * m_speed);

}

// Function to set the ballistic to the correct position before firing
void ballistic::fire(const engine::ref<engine::game_object> player, float speed)
{
	m_object->set_position(player->position() + glm::vec3(0.1f, 0.5f, 0.f));
	m_object->set_velocity(25.0f * player->forward());
	//m_object->set_acceleration(3.0f * kick * camera.front_vector());
	m_object->set_forward(player->forward());
	m_speed = speed;

}
