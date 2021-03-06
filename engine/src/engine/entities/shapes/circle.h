#pragma once
namespace engine
{
	class mesh;
	/// \brief Class creating a circle object with a mesh of a specified size
	class circle
	{
	public:
		/// \brief Constructor
		circle(float radius);
		/// \brief Destructor
		~circle();
		/// \brief Getter methods
		float radius() const { return m_radius; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<circle> create(float radius);
	private:
		/// \brief Fields
		// vertices of the circle specified by a vector of positions.
		glm::vec3 m_centre;
		ref<engine::mesh> m_mesh;
		float m_radius;
	};
}
