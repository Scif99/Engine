#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
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

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

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
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);
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
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);


	// Skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/SkyboxFront.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxRight.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxBack.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxLeft.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxTop.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxBottom.bmp", true)
		});

	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f/ glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(3.0f, 0.5f, -5.0f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	m_mannequin = engine::game_object::create(mannequin_props);
		
	m_player.initialise(m_mannequin);

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/terrain.bmp", false) };
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
	cow_props.position = { -4.f,0.5f, -5.f };
	cow_props.scale = glm::vec3(cow_scale);
	cow_props.bounding_shape = cow_model->size() / 2.f * cow_scale;
	m_cow = engine::game_object::create(cow_props);

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

	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.f, 5.f, -5.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.000001f;
	m_ball = engine::game_object::create(sphere_props);

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


	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_ball);
	//m_game_objects.push_back(m_cow);
	//m_game_objects.push_back(m_tree);
	//m_game_objects.push_back(m_pickup);


	// Vector to store the in-game collectibles
	m_collectibles.push_back(m_health);
	m_collectibles.push_back(m_random);

	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(1);
}

example_layer::~example_layer() {}


// Update various components
void example_layer::on_update(const engine::timestep& time_step)
{



	// Update all collectibles
	for (auto pickup : m_collectibles)
	{
		pickup->update(m_mannequin->position(), time_step); // Use the position of the manneqeuin to determine activity, rather than position of camera
	}


	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	// Update player & camera
	m_player.on_update(time_step);
	m_player.update_camera(m_3d_camera);




	check_bounce();
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
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

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




	
	// Anglae between cow and camera
	glm::vec3 p = glm::vec3(0.f, 0.5f, 5.f);
	glm::vec3 v = m_mannequin->position() - p;
	//Note atan2 works for all quadrants
	float theta = atan2(v.x, v.z);

	glm::mat4 cow_transform(1.0f);
	cow_transform = glm::translate(cow_transform, p);
	cow_transform = glm::rotate(cow_transform,theta,glm::vec3(0.f,1.f,0.f));
	cow_transform = glm::scale(cow_transform, m_cow->scale());
	engine::renderer::submit(textured_lighting_shader, cow_transform, m_cow);

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
	q_pickup_transform = glm::rotate(q_pickup_transform, m_random->rotation_amount(),
		m_random->rotation_axis());
	q_pickup_transform = glm::scale(q_pickup_transform, glm::vec3(0.5f));
	if (m_random->active())
	{
		engine::renderer::submit(textured_lighting_shader, m_random->meshes().at(0),
			q_pickup_transform);
	}


    engine::renderer::end_scene();

	// Set up material shader. (does not render textures, renders materials instead)
	const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
	engine::renderer::begin_scene(m_3d_camera, material_shader);

	m_material->submit(material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	engine::renderer::submit(material_shader, m_ball);

	engine::renderer::end_scene();

	const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
	engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	glm::mat4 aniTransform = glm::mat4(1.0f);

	engine::renderer::submit(animated_mesh_shader, m_mannequin);

	engine::renderer::end_scene();

	// Render text
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	// **TO-DO**
	// Turn this next bit into a class!

	// Score?
	int score = 0;
	std::string s = std::to_string(score);
	m_text_manager->render_text(text_shader,"Score: " + s , 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, "Health: 100 " , (float)engine::application::window().width() - 130.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.f, 0.f, 1.f));


	// TODO: Make this text float up.. Probably want a score update() function
	if (!m_health->active())
	{
		score += 100;
		m_text_manager->render_text(text_shader, " +100 ", 65.f, (float)engine::application::window().height() - 55.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		
	}

	// Display whether player is sprinting
	m_text_manager->render_text(text_shader, "Sprint: " + m_player.sprint_notif(), (float)engine::application::window().width() - 130.f, 25.f, 0.5f, glm::vec4(1.f, 0.f, 0.f, 1.f));

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



    } 
}

void example_layer::check_bounce()
{
	//if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
		//m_audio_manager->play("bounce");
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}

