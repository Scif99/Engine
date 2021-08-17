#include "pch.h"
#include "smoke_trail.h"

smoke_trail::smoke_trail()
{
	s_active = false;
}


smoke_trail::~smoke_trail()
{}

void smoke_trail::load_texture(char* filename)
{
	// Load the texture
	m_texture = engine::texture_2d::create((char*)filename, false); // Loads an image.  Make sure the image is in a 32 bit format (tga) with transparency

	//create a quad
	// Determine the coordinates of the quad	   	 
	std::vector<engine::mesh::vertex> quad_vertices
	{
		//			position					normal					  tex coord       
		{ glm::vec3(-1.f, -1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),		{ 0.f, 0.f } },
		{ glm::vec3(-1.f, 1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),		{ 0.f, 1.f } },
		{ glm::vec3(1.f, 1.f, 0.f),			glm::vec3(0.f, 0.f, -1.f),		{ 1.f, 1.f } },
		{ glm::vec3(1.f, -1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),		{ 1.f, 0.f } },
	};

	const std::vector<uint32_t> quad_indices
	{
		 0,  1,  2,
		 0,  2,  3,
		 0,  2,  1,
		 0,  3,  2,
	};

	m_quad = engine::mesh::create(quad_vertices, quad_indices);
}

void smoke_trail::initialise(glm::vec3 smoke_trail_start, glm::vec3 smoke_trail_direction, int num_total_particles, float width, float height)
{
	// We're going to launch a set of particles along a ray that starts at smokeTrailStart and is directed along smokeTrailDirection
	m_start = smoke_trail_start;
	m_direction = smoke_trail_direction;
	glm::normalize(m_direction);
	m_direction *= 2.0f;

	m_width = width;
	m_height = height;

	m_num_total_particles = num_total_particles;
	s_active = true;
	m_particles.clear();
}

void smoke_trail::clear()
{
	m_particles.clear();
	s_active = false;
	accumulatedTime = 0.f;
}

void smoke_trail::on_render(const engine::perspective_camera& camera, engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;

	m_texture->bind();				// Binds the texture for use

	// Render each particle
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		if (m_particles[i].is_alive())
		{
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", true);
			engine::renderer_api::disable_depth_mask();
			m_texture->bind();
			glm::mat4 transform = m_particles[i].on_render(camera.front_vector(), camera.up_vector(), shader);
			engine::renderer::submit(shader, m_quad, transform);
			engine::renderer_api::enable_depth_mask();
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", false);
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);
		}
	}
}

void smoke_trail::on_update(const engine::timestep& timestep)
{
	m_num_alive = 0;
	for (unsigned int i = 0; i < m_particles.size(); i++) {
		m_particles[i].on_update(timestep);

		// Count the number of alive particles
		if (m_particles[i].is_alive())
			m_num_alive++;
	}

	accumulatedTime += (float)timestep;

	// Check if there are no more alive particles -- if so, clear
	if (s_active && m_particles.size() > 0 && m_num_alive == 0) {
		clear();
		return;
	}

	// Add a particle every 10 ms, up to the total number of particles to be added
	if (accumulatedTime > 0.05f && m_particles.size() < (unsigned int)m_num_total_particles) {
		add_particle();
		accumulatedTime -= 0.05f;
	}
}

// Add another particle 
void smoke_trail::add_particle()
{
	engine::particle particle;
	glm::vec3 p = m_start + (float)m_particles.size() * m_width * m_direction;
	particle.initialise(p, m_width, m_height, glm::vec3(0.f), glm::vec3(1.f), 15);
	m_particles.push_back(particle);
}
