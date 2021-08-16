#include "pch.h"
#include "hud.h"
#include "quad.h"
#include "arrow.h"

hud::hud(const std::string& path, float max_time, float width, float height)
{
	m_max_time = max_time;
	m_heart_texture = engine::texture_2d::create("assets/textures/heart.png", true);
	m_background_texture = engine::texture_2d::create("assets/textures/terrain.bmp", true);

	m_width = width;
	m_height = height;
	m_transparency = 0.5f;


	m_heart = quad::create(glm::vec2(width, height));
	m_background = quad::create(glm::vec2(width, height));



	m_hearts_left = 3;
	s_active = false;
}

hud::~hud()
{}

void hud::on_update(const engine::timestep& time_step)
{
	if (!s_active)
		return;


}

void hud::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;

	//transform = glm::translate(transform, glm::vec3(0.f, -m_width /2.f, -m_width /2.f));

	// Render hearts
	m_heart_texture->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.f);
	std::stack<glm::mat4> hp_stack;
	for (int i = 0;i < m_hearts_left;++i)
	{
		hp_stack.push(glm::mat4(1.f));
		hp_stack.top() = glm::translate(hp_stack.top(), glm::vec3(-m_width/1.1f, -m_height / 1.1f, 0.f));
		hp_stack.top() = glm::translate(hp_stack.top(), glm::vec3(float(0.2*i),0.f,0.f));
		hp_stack.top() = glm::scale(hp_stack.top(), glm::vec3(0.05f));
		engine::renderer::submit(shader, m_heart->mesh(), hp_stack.top());
		hp_stack.pop();
	}


	// render background
	m_background_texture->bind();
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.1f));
	std	::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transparency);
	engine::renderer::submit(shader, m_background->mesh(), transform);



	//engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);


}

void hud::toggle()
{
	s_active = !s_active;
}

engine::ref<hud> hud::create(const std::string& path, float max_time, float width, float height)
{
	return std::make_shared<hud>(path, max_time, width, height);
}
