#pragma once
namespace engine
{
	class mesh;
	/// \brief Class creating a circle object with a mesh of a specified size
	class triangle
	{
	public:
		/// \brief Constructor
		triangle(std::vector<glm::vec3> vertices);
		/// \brief Destructor
		~triangle();
		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<triangle> create(std::vector<glm::vec3> vertices);
	private:
		/// \brief Fields
		// vertices of the triangle specified by a vector of coordinates.
		std::vector<glm::vec3> m_vertices;
		ref<engine::mesh> m_mesh;
		float m_radius;
	};
}
