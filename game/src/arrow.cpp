#include "pch.h"
#include "arrow.h"
#include "ballistic.h"


arrow::arrow()
{}

arrow::~arrow()
{}

void arrow::initialise(float height, float width, float rotation, float x_pos, float z_pos, float thickness)
{
	m_height = height;
	m_width = width;
	m_rotation = rotation;
	m_position = glm::vec3(x_pos, m_height / 2.0f, z_pos);

	m_shaft = engine::cuboid::create(glm::vec3(thickness / 2.f, height / 2.f, thickness / 2.f), false);

	std::vector<glm::vec3> tetrahedron_vertices;
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.3f, 0.f));//0
	tetrahedron_vertices.push_back(glm::vec3(-0.1f, 0.f, 0.1f));//1
	tetrahedron_vertices.push_back(glm::vec3(-0.1f, 0.f, -0.1f)); //2
	tetrahedron_vertices.push_back(glm::vec3(0.1f, 0.f, -0.1f)); //3
	tetrahedron_vertices.push_back(glm::vec3(0.1f, 0.f, 0.1f)); //4
	m_head =engine::tetrahedron::create(tetrahedron_vertices);

	m_material = engine::material::create(0.5f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1.0f);

	m_vertices.push_back(glm::vec3(-width / 2.0f, -height / 2.0f, 0.f));
	m_vertices.push_back(glm::vec3(width / 2.0f, -height / 2.0f, 0.f));
	m_vertices.push_back(glm::vec3(width / 2.0f, height / 2.0f, 0.f));
	m_vertices.push_back(glm::vec3(-width / 2.0f, height / 2.0f, 0.f));

	glm::mat4 overall_transform(1.0f);
	overall_transform = glm::translate(overall_transform, m_position + glm::vec3(0.f,1.f,0.f));
	overall_transform = glm::rotate(overall_transform, -0.5f * glm::pi<float>(), glm::vec3(1.f, 0.f, 0.f));

	left_transform = glm::translate(overall_transform, glm::vec3(0.f, 0.f, 0.f));
	right_transform = glm::translate(overall_transform, glm::vec3(0.f, m_height/2, 0.f));


	for (uint32_t i = 0; i < m_vertices.size(); i++)
	{
		m_vertices.at(i) = overall_transform * glm::vec4(m_vertices.at(i), 1.f);
	}
}



void arrow::on_render(const engine::ref<engine::shader>& shader)
{
	m_material->submit(shader);

	engine::renderer::submit(shader, m_shaft->mesh(), left_transform);
	engine::renderer::submit(shader, m_head->mesh(), right_transform);
	

	//engine::renderer::submit(shader, );
}

//bool goal_post::check_collision(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 r0, glm::vec3 r1, glm::vec3& intersection, double& t)
//{
//	double u, v;
//	float EPSILON = 0.000000001f;
//
//	if (glm::length(r1 - r0) < EPSILON)
//		return false;
//
//	 triangle intersection routine
//	glm::vec3 edge1, edge2, tvec, pvec, qvec;
//	double det, inv_det;
//	glm::vec3 dir = r1 - r0;
//
//
//	 find vectors for two edges sharing vert0
//	edge1 = v1 - v0;
//	edge2 = v2 - v0;
//
//	 begin calculating determinant - also used to calculate U parameter
//	pvec = glm::cross(dir, edge2);
//
//	 if determinant is near zero, ray lies in plane of triangle
//	det = glm::dot(edge1, pvec);
//
//	if (det > -EPSILON && det < EPSILON)
//		return false;
//	inv_det = 1.0 / det;
//
//	 calculate distance from vert0 to ray origin
//	tvec = r0 - v0;
//
//	 calculate U parameter and test bounds
//	u = glm::dot(tvec, pvec) * inv_det;
//	if (u < 0.0 || u > 1.0)
//		return false;
//
//	 prepare to test V parameter
//	qvec = glm::cross(tvec, edge1);
//
//	 calculate V parameter and test bounds
//	v = glm::dot(dir, qvec) * inv_det;
//	if (v < 0.0 || u + v > 1.0)
//		return false;
//
//	 calculate t, ray intersects triangle
//	t = glm::dot(edge2, qvec) * inv_det;
//	if (t < 0 || t > 1)
//		return false;
//
//	intersection = r0 + (float)t * dir;
//	return true;
//}
