#pragma once
#include <engine.h>

class quad;

class hud
{
public:
	hud(const std::string& path, float max_time, float width, float height);
	~hud();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);
	void toggle();

	static engine::ref<hud> create(const std::string& path, float max_time, float width, float height);
	bool s_active;
	int m_hearts_left;
private:
	//bool s_active;

	float m_timer;
	float m_max_time;
	float m_width;
	float m_height;
	float m_health;
	float m_score;


	engine::ref<engine::text_manager>	m_text_manager{};


	engine::ref<engine::texture_2d> m_heart_texture;
	engine::ref<engine::texture_2d> m_background_texture;


	float m_transparency;
	engine::ref<quad> m_heart;
	engine::ref<quad> m_background;

};
