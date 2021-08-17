#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/events/mouse_event.h"
#include "engine/utils/track.h"


// Includes for collectibles
#include "pickup.h"
#include "player.h"


example_layer::example_layer() 
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f), 
    m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())

{

    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::event, "bounce"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/DST-impuretechnology.mp3", engine::sound_type::track, "music");  // Royalty free music from http://www.nosoapradio.us/
	//m_audio_manager->play("music");


	// Initialise the shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

	// Set properties of directional light variable
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	// Point light?


	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);

	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f); // Sets specular power (higher mean sharper)

	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));

	// Set properties of materials
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	m_tetrahedron_material = engine::material::create(32.0f,
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.3f);



	// Skybox texture from https://opengameart.org/content/interstellar-skybox
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/zpos.png", true),
		  engine::texture_2d::create("assets/textures/skybox/xpos.png", true),
		  engine::texture_2d::create("assets/textures/skybox/zneg.png", true),
		  engine::texture_2d::create("assets/textures/skybox/xneg.png", true),
		  engine::texture_2d::create("assets/textures/skybox/ypos.png", true),
		  engine::texture_2d::create("assets/textures/skybox/yneg.png", true)
		});

	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/right_strafe.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/left_strafe.dae");
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f/ glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(3.0f, 0.5f, -5.0f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	m_mannequin = engine::game_object::create(mannequin_props);
		
	m_player.initialise(m_mannequin);

	// ENVIRONMENT

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/red.bmp", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	// Load the cow model. Create a cow object. Set its properties
	engine::ref <engine::model> cow_model = engine::model::create("assets/models/static/cow4.3ds");
	engine::game_object_properties cow_props;
	cow_props.meshes = cow_model->meshes();
	cow_props.textures = cow_model->textures();
	float cow_scale = 1.f / glm::max(cow_model->size().x, glm::max(cow_model->size().y, cow_model->size().z));
	cow_props.position = { 3.5f,0.5f, 4.f };
	cow_props.scale = glm::vec3(cow_scale);
	cow_props.bounding_shape = cow_model->size() / 2.f * cow_scale;
	m_cow = engine::game_object::create(cow_props);

	// Load the jeep model .Create a Jeep object. Set properties.
	engine::ref <engine::model> jeep_model = engine::model::create("assets/models/static/jeep1.3ds");
	engine::game_object_properties jeep_props;
	jeep_props.meshes = jeep_model->meshes();
	jeep_props.textures = jeep_model->textures();
	float jeep_scale = 0.125f ;
	jeep_props.position = { 0.f,0.5f, 10.f };
	jeep_props.scale = glm::vec3(jeep_scale);
	jeep_props.bounding_shape = jeep_model->size() / 2.f * jeep_scale;
	m_jeep = engine::game_object::create(jeep_props);

	// Load the npc model. From https://opengameart.org/content/wandering-vendor-npc
	engine::ref <engine::model> npc_model = engine::model::create("assets/models/static/wandering_trader.obj");
	engine::ref<engine::texture_2d> npc_texture =
		engine::texture_2d::create("assets/textures/text_corpoV.png", true);
	engine::game_object_properties npc_props;
	npc_props.meshes = npc_model->meshes();
	npc_props.textures = { npc_texture };
	float npc_scale = 0.5f;
	npc_props.position = { 3.5f, 0.5f, 5.f };
	npc_props.scale = glm::vec3(npc_scale);
	npc_props.bounding_shape = npc_model->size() / 2.f * npc_scale;
	m_npc = engine::game_object::create(npc_props);


	// Load the tree model. Create a tree object. Set its properties
	engine::ref <engine::model> tree_model = engine::model::create("assets/models/static/elm.3ds");
	engine::game_object_properties tree_props;
	tree_props.meshes = tree_model->meshes();
	tree_props.textures = tree_model->textures();
	float tree_scale = 3.f / glm::max(tree_model->size().x, glm::max(tree_model->size().y, tree_model->size().z));
	tree_props.position = { 4.f, 0.5f, -5.f };
	tree_props.bounding_shape = tree_model->size() / 2.f * tree_scale;
	tree_props.scale = glm::vec3(tree_scale);
	m_tree = engine::game_object::create(tree_props);

	//  Set properties of sphere
	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.f, 5.f, -5.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	//Texture
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.000001f;
	m_ball = engine::game_object::create(sphere_props);

	//ENEMIES

	//Load blob model
	m_blob.initialise(10.f,10.f);
	engine::game_object_properties blob_props;
	blob_props.position = { 10.f,1.f,10.f};
	blob_props.meshes = { m_blob.top()->mesh(), m_blob.bottom()->mesh() };
	////Texture
	blob_props.type = 1;
	blob_props.restitution = 0.92f;
	blob_props.mass = 0.000001f;
	//m_blob.initialise(engine::game_object::create(blob_props));// Is this right?


	m_arrow.initialise(1.0f, 0.5f, 0.f, -1.f, -1.f, 0.1f);
	engine::game_object_properties arrow_props;
	arrow_props.position = { 0.f, 10.f, -5.f };
	arrow_props.meshes = { m_arrow.head()->mesh(), m_arrow.shaft()->mesh() };
	////Texture
	arrow_props.type = 1;
	//arrow_props.bounding_shape = glm::vec3(0.5f);
	arrow_props.restitution = 0.92f;
	arrow_props.mass = 0.000001f;
	m_ballistic.initialise(engine::game_object::create(arrow_props));// Is this right?




	 // Circle
	engine::ref<engine::circle> circle_shape =	engine::circle::create(1);
	engine::game_object_properties circle_props;
	circle_props.position = { -2.f,0.6f,2.f };
	circle_props.meshes = { circle_shape->mesh() };
	m_circle = engine::game_object::create(circle_props);

	// Platforms
	engine::ref<engine::cone> platform_shape = engine::cone::create(2, 0.5, 6);
	engine::game_object_properties platform_props;
	platform_props.position = { 0.f,0.f,-10.f };
	platform_props.meshes = { platform_shape->mesh() };
	m_platform = engine::game_object::create(platform_props);


	// House
	m_house.initialise(2.f, 2.f, 5.f, 5.f, 20);
	m_mountain.initialise(10.f, 4.f, 20.f, 0.f, 4);


	//Door
	m_door.initialise(3.0f, 2.f, 0.f, 0.f, 3.f, -25.f, 0.4f);


	// Collectibles

	// Cuboid ptr for shared use by collectibles
	engine::ref<engine::cuboid> cube_shape = engine::cuboid::create(glm::vec3(0.5f), false);

	// Medkit texture from https://www.textures.com/download/manmadeboxes0007/105116
	engine::ref<engine::texture_2d> health_pickup_texture =
								engine::texture_2d::create("assets/textures/medkit.jpg", true);
	engine::game_object_properties health_pickup_props;
	health_pickup_props.position = { 5.f, 1.f, 5.f };
	health_pickup_props.meshes = { cube_shape->mesh() };
	health_pickup_props.textures = { health_pickup_texture };
	m_health = pickup::create(health_pickup_props);
	m_health->init();


	// Powerup texture from https://gamebanana.com/sprays/52947
	engine::ref<engine::texture_2d> q_pickup_texture =
		engine::texture_2d::create("assets/textures/question.jpg", true);
	engine::game_object_properties q_pickup_props;
	q_pickup_props.position = { 10.f, 1.f, 5.f };
	q_pickup_props.meshes = { cube_shape->mesh() };
	q_pickup_props.textures = { q_pickup_texture };
	m_random = pickup::create(q_pickup_props);
	m_random->init();

	// Weapon model (from https://opengameart.org/content/bailisk-crossbow)
	engine::ref <engine::model> bow_model = engine::model::create("assets/models/static/cbow.obj");
	engine::game_object_properties bow_props;
	bow_props.meshes = bow_model->meshes();
	bow_props.textures = bow_model->textures();
	float bow_scale = 0.125f;
	bow_props.position = { -2.f,1.f, 2.f };
	bow_props.scale = glm::vec3(bow_scale);
	bow_props.bounding_shape = bow_model->size() / 2.f * bow_scale;
	m_bow = pickup::create(bow_props);
	m_bow->init();


	// Key model from https://free3d.com/3d-model/low-poly-key-680139.html
	engine::ref <engine::model> key_model = engine::model::create("assets/models/static/key.obj");
	engine::game_object_properties key_props;
	key_props.meshes = key_model->meshes();
	key_props.textures = key_model->textures();
	float key_scale = 0.005f;
	key_props.position = { 0.f,1.5f, -20.f };
	key_props.scale = glm::vec3(key_scale);
	key_props.bounding_shape = key_model->size() / 2.f * key_scale;
	m_key = pickup::create(key_props);
	m_key->init();


	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_ball);
	//m_game_objects.push_back(m_cow);
	//m_game_objects.push_back(m_tree);
	//m_game_objects.push_back(m_pickup);


	// Vector to store the in-game collectibles
	m_collectibles.push_back(m_health);
	m_collectibles.push_back(m_random);
	m_collectibles.push_back(m_bow);
	m_collectibles.push_back(m_key);

	//Vector to store enemies
	//m_enemies.push_back(m_blob);

	// Vector to store available arrows;
	//m_arrow_pouch.push_back();
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	// create  special FX
	m_cross_fade = cross_fade::create("assets/textures/red.bmp", 2.0f, 1.6f, 0.9f);
	m_forcefield = alpha_sphere::create(glm::vec3(1.f, 0.f, 1.0f), glm::vec3(0.f, 0.f, 1.f), true, 0.25f, 2.0f);
	m_smoke_trail.load_texture("assets/textures/grey_smoke.tga");

	m_text_manager = engine::text_manager::create();

	// Create HUD
	m_hud = hud::create("assets/textures/heart.png", 2.0f, 1.6f, 0.9f);

	m_skinned_mesh->switch_animation(1);
}

example_layer::~example_layer() {}


// Update various components
void example_layer::on_update(const engine::timestep& time_step)
{
	// Pause
	if (m_hud->s_active)
		return;


	// Update all collectibles
	for (auto pickup : m_collectibles)
	{
		pickup->update(m_mannequin->position(), time_step); // Use the position of the manneqeuin to determine activity, rather than position of camera
	}

	// TO-FIX! (use a timer)
	if (!m_health->active())
	{

		m_player.set_health(m_player.health() + 1);
	}


	// Update physics
	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	// Update ballistics
	m_ballistic.on_update(time_step);

	// Update player & camera
	m_player.on_update(time_step);
	m_player.update_camera(m_3d_camera);
	LOG_CORE_ERROR("hp left :  '{}'.", m_hud->m_hearts_left);
	



	//Update enemies
	m_blob.on_update(time_step);


	check_bounce();
	// Update FX
	m_cross_fade->on_update(time_step);
	m_forcefield->on_update(time_step);
	m_smoke_trail.on_update(time_step);

	// Update HUD
	m_hud->on_update(time_step);
}

void example_layer::on_render() 
{
    engine::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f}); 
    engine::render_command::clear();

    //const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
    //engine::renderer::begin_scene(m_3d_camera, textured_shader);

	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position()); // Use camera position as light source

	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(textured_lighting_shader, m_terrain);


	// Render in trees on either side of entrance
	std::stack<glm::mat4> tree_stack;
	for (int i = 0;i < 5;++i)
	{
		// Left Side
		tree_stack.push(glm::mat4(1.0f));
		tree_stack.top() = glm::translate(tree_stack.top(), glm::vec3(-5.f, 0.5f, -float(i)*5.f));
		tree_stack.top() = glm::scale(tree_stack.top(), glm::vec3(0.2f));
		engine::renderer::submit(textured_lighting_shader, tree_stack.top(), m_tree);
		tree_stack.pop();

		// Right side
		tree_stack.push(glm::mat4(1.0f));
		tree_stack.top() = glm::translate(tree_stack.top(), glm::vec3(5.f, 0.5f, -float(i) * 5.f));
		tree_stack.top() = glm::scale(tree_stack.top(), glm::vec3(0.2f));
		engine::renderer::submit(textured_lighting_shader, tree_stack.top(), m_tree);
		tree_stack.pop();


	}

	// Angle between cow and camera
	glm::vec3 p = glm::vec3(m_cow->position());
	glm::vec3 v = m_mannequin->position() - p;
	//Note atan2 works for all quadrants
	float theta = atan2(v.x, v.z);

	glm::vec3 p_2 = glm::vec3(m_bow->position());
	glm::vec3 v_2 = m_3d_camera.front_vector() - p_2;
	//Note atan2 works for all quadrants
	float theta_2 = atan2(v.x, v.z);


	glm::mat4 cow_transform(1.0f);
	cow_transform = glm::translate(cow_transform, p);
	cow_transform = glm::rotate(cow_transform,theta,glm::vec3(0.f,1.f,0.f));
	cow_transform = glm::scale(cow_transform, m_cow->scale());
	engine::renderer::submit(textured_lighting_shader, cow_transform, m_cow);

	//Render Jeep mesh
	glm::mat4 jeep_transform(1.0f);
	jeep_transform = glm::translate(m_jeep->position());
	jeep_transform = glm::rotate(jeep_transform, -0.5f*glm::pi<float>(), glm::vec3(1.f, 0.f, 0.f));
	jeep_transform = glm::scale(jeep_transform, m_jeep->scale());
	engine::renderer::submit(textured_lighting_shader, jeep_transform, m_jeep);

	//Render npc
	glm::mat4 npc_transform(1.0f);
	npc_transform = glm::translate(m_npc->position());
	npc_transform = glm::rotate(npc_transform, -1.f * glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f));
	npc_transform = glm::scale(npc_transform, m_npc->scale());
	engine::renderer::submit(textured_lighting_shader, npc_transform, m_npc);



		// Render weapon
	m_bow->textures().at(0)->bind(); // did nothing?
	glm::mat4 bow_transform(1.0f);
	bow_transform = glm::translate(m_bow->position()); // Move to where model is
	bow_transform = glm::translate(bow_transform, glm::vec3(0.f, 0.3f * sin(m_bow->rotation_amount()), 0.f));
	bow_transform = glm::rotate(bow_transform, m_bow->rotation_amount(), m_bow->rotation_axis());
	bow_transform = glm::rotate(bow_transform, -0.5f * glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f)); // Rotate so it is isnt sideways
	bow_transform = glm::scale(bow_transform, 0.5f *m_bow->scale());
	if (m_bow->active())
	{
		engine::renderer::submit(textured_lighting_shader, bow_transform,
			m_bow);
	}

	else
	{
		bow_transform = glm::translate(m_mannequin->position()+glm::vec3(0.1f,0.6f,0.f)); // Move to where model is
		bow_transform = glm::rotate(bow_transform, theta_2, glm::vec3(0.f,1.f,0.f));
		bow_transform = glm::scale(bow_transform, 0.2f * m_bow->scale());
		engine::renderer::submit(textured_lighting_shader, bow_transform,
			m_bow);
	}

	//Render Key
	engine::renderer::submit(textured_lighting_shader, m_key);



	//for (auto weapon : m_weapons)
	//{Process equip(), and stuff...}

	// Render health pickup
	m_health->textures().at(0)->bind();
	glm::mat4 pickup_transform(1.0f);
	pickup_transform = glm::translate(pickup_transform, m_health->position());
	pickup_transform = glm::rotate(pickup_transform, m_health->rotation_amount(),
		m_health-> rotation_axis());
	if (m_health->active())
	{
		engine::renderer::submit(textured_lighting_shader, m_health->meshes().at(0),
			pickup_transform);
	}

	m_random->textures().at(0)->bind();
	glm::mat4 q_pickup_transform(1.0f);
	q_pickup_transform = glm::translate(q_pickup_transform, m_random->position());
	q_pickup_transform = glm::rotate(q_pickup_transform, m_random->rotation_amount(), m_random->rotation_axis());
	q_pickup_transform = glm::scale(q_pickup_transform, glm::vec3(0.5f));
	if (m_random->active())
	{
		engine::renderer::submit(textured_lighting_shader, m_random->meshes().at(0),
			q_pickup_transform);
	}

	// Smoke
	m_smoke_trail.on_render(m_3d_camera, textured_lighting_shader);

    engine::renderer::end_scene();

	// Set up material shader. (does not render textures, renders materials instead)
	const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
	engine::renderer::begin_scene(m_3d_camera, material_shader);



	// Render House
	m_house.on_render(material_shader);


	m_mountain.on_render(material_shader);

	m_material->submit(material_shader); //Pass sphere material to renderer

	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position()); // 

	// Render the sphere using material shader
	engine::renderer::submit(material_shader, m_ball);

	//Render door
	m_door.on_render(material_shader);

	// Render ballistic using same shader as for sphere
	m_arrow.on_render(material_shader);
	m_ballistic.on_render(material_shader);

	m_blob.on_render(material_shader); //BLOB



	
	// Render forcefield
	m_forcefield->on_render(material_shader);


	m_material->submit(material_shader); //Pass tetrahedron material to renderer

	//Only render circles if weapon not picked up **Need to check if 2nd weapon is picked up too
	if (m_bow->active())
	{
		glm::mat4 circle_transform(1.f);
		engine::renderer::submit(material_shader, m_circle);
		circle_transform = glm::translate(circle_transform, glm::vec3(2.f, 0.6f, 2.f));
		engine::renderer::submit(material_shader, circle_transform, m_circle);
	}

	std::stack<glm::mat4> platform_stack;

	for (int i = 0;i < 3;++i)
	{
		platform_stack.push(glm::mat4(1.f));
		platform_stack.top() = glm::translate(platform_stack.top(), m_platform->position() + glm::vec3(0.f, 1.f +float(i), -float(4*i)));
		platform_stack.top() = glm::rotate(platform_stack.top(), glm::pi<float>(), glm::vec3(1.f, 0.f, 0.f));
		
		
		engine::renderer::submit(material_shader, platform_stack.top(), m_platform);

	}

	

	engine::renderer::end_scene();

	const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
	engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	// Render FX

	glm::mat4 aniTransform = glm::mat4(1.0f);

	engine::renderer::submit(animated_mesh_shader, m_mannequin);

	engine::renderer::end_scene();

	// Render text
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	// **TO-DO**
	// Turn this next bit into a class!

	// Score?

	m_text_manager->render_text(text_shader,"Score: " + std::to_string(m_score) , 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, "Health:  " + std::to_string(m_player.health()) , (float)engine::application::window().width() - 130.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.f, 0.f, 1.f));

	//Render crosshairs?

	std::pair<float, float> centre = std::make_pair(((float)engine::application::window().width() - engine::application::window().width() / 2.f),
		((float)engine::application::window().height() - (float)engine::application::window().height() / 2.f)); // The centre of the window
	m_text_manager->render_text(text_shader, "|", centre.first + 1.f, centre.second + 15.f, 0.2f, glm::vec4(1.f, 1.f, 1.f, 1.f)); //Top
	m_text_manager->render_text(text_shader, "|", centre.first + 1.f, centre.second - 15.f, 0.2f, glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_text_manager->render_text(text_shader, "_", centre.first - 14.f, centre.second + 3.f, 0.2f, glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_text_manager->render_text(text_shader, "_", centre.first + 14.f, centre.second + 3.f, 0.2f, glm::vec4(1.f, 1.f, 1.f, 1.f));


	// Render Game Over text
	if (m_hud->m_hearts_left == 0)
	{
		m_text_manager->render_text(text_shader,  "GAME OVER! ",
			(float)engine::application::window().height() / 3.f, (float)engine::application::window().height() / 2.f, 2.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));

		m_text_manager->render_text(text_shader, "Press escape to exit... ",
			(float)engine::application::window().height() / 3.f, (float)engine::application::window().height() / 3.f, 1.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));
	}

	//Render 2D

	engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);
	m_cross_fade->on_render(textured_lighting_shader);

	//Render HUD
	m_hud->on_render(textured_lighting_shader);
	

} 


// In-Game events 
void example_layer::on_event(engine::event& event) 
{ 
    if(event.event_type() == engine::event_type_e::key_pressed) 
    { 
        auto& e = dynamic_cast<engine::key_pressed_event&>(event); 
        if(e.key_code() == engine::key_codes::KEY_TAB) 
        { 
            engine::render_command::toggle_wireframe();
        }

		if (e.key_code() == engine::key_codes::KEY_C)
		{
			m_player.toggle_fps();
		}

		// **TO-DO Change to when player gets hit
		if (e.key_code() == engine::key_codes::KEY_1)
		{
			m_cross_fade->activate();
			//m_3d_camera->shake();
		}


		// **TO-DO Change to when player gets hit
		if (e.key_code() == engine::key_codes::KEY_P)
		{
			m_hud->toggle();
			//m_3d_camera->shake();
		}

		if (e.key_code() == engine::key_codes::KEY_2)
		{
			m_hud->m_hearts_left -= 1;
			//m_3d_camera->shake();
		}

		if (e.key_code() == engine::key_codes::KEY_G)
		{
			m_smoke_trail.initialise(m_mannequin->position() + glm::vec3(0.f,1.f,0.f), -m_mannequin->forward(), 5, 0.25f, 0.25f); //TIMER
		}


		// If player tries to move into forcefield before defeating all the enemies
		if (glm::length(m_mannequin->position() - m_key->position()) < 3.f) // Event should only play if not activated yet
		{
			
			m_forcefield->activate(2.f, m_key->position());
			m_cross_fade->activate();
			m_player.set_health(m_player.health()-10);
			// Render soome text?
			// Add a collision

		}

		// If player tries to move to blobbefore defeating all the enemies
		if (glm::length(m_mannequin->position() - m_blob.position() ) < 3.f && !m_blob.dead()) // Event should only play if blob is not dead
		{
			m_cross_fade->activate();
			m_blob.set_health(m_blob.health() - 2);
			m_player.set_health(m_player.health() - 10);
			// Render soome text?
			// Add a collision

		}


    }

	// Events involving the mouse
	if (event.event_type() == engine::event_type_e::mouse_button_pressed)
	{
		auto& e = dynamic_cast<engine::mouse_button_pressed_event&>(event);
		if (e.mouse_button() == engine::mouse_button_codes::MOUSE_BUTTON_LEFT)// && m_arrow_pouch.size() > 0) //Left click
		{
			m_ballistic.fire(m_mannequin, 25.0f); // Fire from player's position
			//m_arrow_pouch.pop_back();
		}

	}


}

void example_layer::check_bounce()
{
	//if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
		//m_audio_manager->play("bounce");
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}

