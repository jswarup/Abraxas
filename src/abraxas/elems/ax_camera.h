// ax_camera.h  -------------------------------------------------------------------------------------------------------------------------
#pragma once 

#define GLM_ENABLE_EXPERIMENTAL
#include 	<glm/gtx/quaternion.hpp>

#include 	"abraxas/elems/ax_userinput.h" 
#include 	"abraxas/render/ax_shader.h"
 
//---------------------------------------------------------------------------------------------------------------------------------

class Ax_Camera 
{
	glm::mat4 	m_ViewMatrix;
	glm::mat4 	m_Projection;
	glm::vec3 	m_Position;

	glm::vec3 	m_Focus;

	float 		m_Distance;
	float 		m_Aspect;
	float 		m_FOV;
	float 		m_Near;
	float 		m_Far;

	float 		m_Pitch;
	float 		m_Yaw;

	glm::vec2 	m_CurrentPos2d = { 0.0f, 0.0f };

	const glm::vec3 cRight = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 cUp = { 0.0f, 1.0f, 0.0f };
	const glm::vec3 cForward = { 0.0f, 0.0f, -1.0f }; 
	const float cRotationSpeed = 2.0f;

public:
	Ax_Camera( void)
	{}
	
	void 	Init( const glm::vec3& position, float fov, float aspect, float nearDist, float farDist)
	{
		m_Position = position;
		m_Aspect = aspect;
		m_Near = nearDist;
		m_Far = farDist;
		m_FOV = fov;

		m_Projection = glm::mat4{ 1.0f };		
		m_Focus = { 0.0f, 0.0f, 0.0f };
		m_Distance = 5.0f;
		m_Pitch = 0.0f;
		m_Yaw = 0.0f;
		m_CurrentPos2d = { 0.0f, 0.0f };

		set_aspect( m_Aspect);

		update_view_matrix();
	}

	void 	Update( Ax_Shader *shader) 
	{
		glm::mat4 			model{ 1.0f };
		shader->set_mat4( model, "model");
		shader->set_mat4( m_ViewMatrix, "view");
		shader->set_mat4( get_projection(), "projection");
		shader->set_vec3( m_Position, "camPos");
	}

	void 	set_aspect( float aspect)
	{
		m_Projection = glm::perspective( m_FOV, aspect, m_Near, m_Far);
	}

	void 	set_distance( float offset)
	{
		m_Distance += offset;
		update_view_matrix();
	}

	const glm::mat4		&get_projection( void) const { return m_Projection; }

	glm::mat4 			get_view_projection( void) const { return m_Projection * get_view_matrix(); }

	glm::quat 			get_direction( void) const { return glm::quat( glm::vec3( -m_Pitch, -m_Yaw, 0.0f)); }

	glm::vec3 			get_up( void) const { return glm::rotate( get_direction(), cUp); }

	glm::vec3 			get_right(  void) const { return glm::rotate( get_direction(), cRight); }

	glm::vec3 			get_forward( void) const { return glm::rotate(get_direction(), cForward); }

	const glm::mat4 	&get_view_matrix() const { return m_ViewMatrix; }

	void 		on_mouse_wheel( double delta)
	{
		set_distance( float( delta * 0.5f));
		update_view_matrix();
	}

	void 		reset( void)
	{
		m_Focus = { 0.0f, 0.0f, 0.0f };
		//mDistance = 5.0f;
		update_view_matrix();
	}

	void 		on_mouse_move( double x, double y, Ax_UserInput::ButtonPress button)
	{
		glm::vec2 pos2d{ x, y };

		if ( button == Ax_UserInput::Right)
		{
			glm::vec2 	delta = ( pos2d - m_CurrentPos2d) * 0.004f;
			float 		sign = get_up().y < 0 ? -1.0f : 1.0f;
			m_Yaw += sign * delta.x * cRotationSpeed;
			m_Pitch += delta.y * cRotationSpeed;

			update_view_matrix();
		} else if (button == Ax_UserInput::Middle)
		{
			// TODO: Adjust pan speed for distance
			glm::vec2 	delta = (pos2d - m_CurrentPos2d) * 0.003f;

			m_Focus += -get_right() * delta.x * m_Distance;
			m_Focus += get_up() * delta.y * m_Distance;
			update_view_matrix();
		}
		m_CurrentPos2d = pos2d;
	}

	void 	update_view_matrix( void)
	{
		m_Position =  m_Focus - get_forward() * m_Distance;
		glm::quat 		orientation = get_direction();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4( orientation);
		m_ViewMatrix = glm::inverse( m_ViewMatrix);
	}
};

//---------------------------------------------------------------------------------------------------------------------------------
 

