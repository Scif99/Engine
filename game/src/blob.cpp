#include "pch.h"
#include "blob.h"



blob::blob()
{}

blob::~blob()
{}

void blob::initialise(float x_pos, float z_pos, float health)
{
	m_health = health;
	m_is_dead = false;
	m_top = engine::sphere::create(10, 20, 0.3f);
	m_bottom = engine::sphere::create(10, 20, 0.5f);

	m_position = glm::vec3(x_pos, 0.5f + m_bottom->radius(), z_pos);

	m_material = engine::material::create(0.5f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1.0f);



	glm::mat4 overall_transform(1.0f);
	overall_transform = glm::translate(overall_transform, m_position);

	top_transform = glm::translate(overall_transform, glm::vec3(0.f, m_bottom->radius()+ m_top->radius(), 0.f));
	bottom_transform = glm::translate(overall_transform, glm::vec3(0.f));

	for (uint32_t i = 0; i < m_vertices.size(); i++)
	{
		m_vertices.at(i) = overall_transform * glm::vec4(m_vertices.at(i), 1.f);
	}
}

void::blob::on_update(float dt)
{
	if (m_health < 0)
	{
		m_is_dead = true;
	}
}


void blob::on_render(const engine::ref<engine::shader>& shader)
{
	if (m_health > 0)
	{
		m_material->submit(shader);

		engine::renderer::submit(shader, m_top->mesh(), top_transform);
		engine::renderer::submit(shader, m_bottom->mesh(), bottom_transform);

	}
	//engine::renderer::submit(shader, );
}

void blob::attack(float dt)
{
	//attack
}

bool blob::check_collision(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 r0, glm::vec3 r1, glm::vec3& intersection, double& t)
{
	double u, v;
	float EPSILON = 0.000000001f;

	if (glm::length(r1 - r0) < EPSILON)
		return false;

	// triangle intersection routine
	glm::vec3 edge1, edge2, tvec, pvec, qvec;
	double det, inv_det;
	glm::vec3 dir = r1 - r0;


	// find vectors for two edges sharing vert0
	edge1 = v1 - v0;
	edge2 = v2 - v0;

	// begin calculating determinant - also used to calculate U parameter
	pvec = glm::cross(dir, edge2);

	// if determinant is near zero, ray lies in plane of triangle
	det = glm::dot(edge1, pvec);

	if (det > -EPSILON && det < EPSILON)
		return false;
	inv_det = 1.0 / det;

	// calculate distance from vert0 to ray origin
	tvec = r0 - v0;

	// calculate U parameter and test bounds
	u = glm::dot(tvec, pvec) * inv_det;
	if (u < 0.0 || u > 1.0)
		return false;

	// prepare to test V parameter
	qvec = glm::cross(tvec, edge1);

	// calculate V parameter and test bounds
	v = glm::dot(dir, qvec) * inv_det;
	if (v < 0.0 || u + v > 1.0)
		return false;

	// calculate t, ray intersects triangle
	t = glm::dot(edge2, qvec) * inv_det;
	if (t < 0 || t > 1)
		return false;

	intersection = r0 + (float)t * dir;
	return true;
}
