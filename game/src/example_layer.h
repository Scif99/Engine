#pragma once
#include <engine.h>
#include "player.h"

#include "ballistic.h"
#include "cross_fade.h"
#include "forcefield.h"
#include "arrow.h"
#include "house.h"
#include "blob.h"
#include "pickup_container.h"



class pickup;
class player;
class blob;


class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:
	void check_bounce();

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_cow{};
	engine::ref<engine::game_object>	m_tree{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_jeep{};



	engine::ref<engine::game_object>	m_circle{};
	engine::ref<engine::game_object>	m_triangle{};



	house m_house;



	// Characters

	player m_player{};
	blob m_blob;

	// Ballistics
	arrow m_arrow;
	ballistic m_ballistic;
	pickup_container m_pickup_container;
	// Collectibles
	engine::ref<pickup> m_health;
	engine::ref<pickup> m_random;
	engine::ref<pickup> m_bow;
	engine::ref<pickup> m_wand;
	engine::ref<pickup> m_key;
	


	// Materials
	engine::ref<engine::material>		m_material{};
	engine::ref<engine::material> m_tetrahedron_material{};


	// Light Sources
	engine::DirectionalLight            m_directionalLight;



	// Containters for objects
	std::vector<engine::ref<engine::game_object>>   m_game_objects{};
	std::vector<engine::ref<pickup>>	m_collectibles{};
	std::vector<engine::ref<engine::game_object>>	m_enemies{};


	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;

	// Special FX
	engine::ref<cross_fade>							m_cross_fade{};
	engine::ref<alpha_sphere>						m_forcefield{};
	//Variable to track game score
	int m_score{ 0 };
};
