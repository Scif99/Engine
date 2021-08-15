#pragma once
namespace engine
{
	class mesh;
	/// \brief Class creating a cone object with a mesh of a specified size
	class cone
	{
	public:
		/// \brief Constructor
		cone(float radius, float height, int stacks);
		/// \brief Destructor
		~cone();
		/// \brief Getter methods
		float radius() const { return m_radius; }
		float height() const { return m_height; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<cone> create(float radius, float height, int stacks);
	private:
		/// \brief Fields
		// vertices of the circle specified by a vector of positions.
		glm::vec3 m_centre;
		ref<engine::mesh> m_mesh;
		std::vector<glm::vec3> m_vertices{};
		float m_radius;
		float m_height;
		int m_stacks;
	};
}
