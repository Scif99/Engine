#pragma once
namespace engine
{
	class mesh;
	/// \brief Class creating a circle object with a mesh of a specified size
	class circle
	{
	public:
		/// \brief Constructor
		circle(glm::vec3 centre);
		/// \brief Destructor
		~circle();
		/// \brief Getter methods
		glm::vec3 centre() const { return m_centre; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<circle> create(glm::vec3 centre);
	private:
		/// \brief Fields
		// vertices of the circle specified by a vector of positions.
		glm::vec3 m_centre;
		ref<engine::mesh> m_mesh;
	};
}
