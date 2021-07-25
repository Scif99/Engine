#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"



class player
{

public:
	player();
	~player();
	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);
	engine::ref<engine::game_object> object() const { return m_object; }
	void player::strafe_right(engine::timestep ts);
	void player::strafe_left(engine::timestep ts);
	void player::process_mouse(float delta_x, float delta_y);
	void player::turn(float angle_x, float angle_y);
	void update_camera(engine::perspective_camera& camera);
	void jump();
	bool is_fps() { return fps; }
	void toggle_fps() { fps =  !fps; } //Toggle fps camera
	bool sprint = false;
	std::string sprint_notif() { return (sprint) ? "On" : "Off"; } // Part of the HUD
	void set_health(int h) { m_health = h; }
	int health() { return m_health; }

private:
	float m_speed{ 0.f };
	float m_timer;
	bool fps = true; // Boolean to determins whether camera is in fps mode
	int m_health = 100;

	engine::ref< engine::game_object> m_object;
};
