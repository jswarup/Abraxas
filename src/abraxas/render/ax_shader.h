// ax_shader.h ------------------------------------------------------------------------------------------------------------------
#pragma once
 
//---------------------------------------------------------------------------------------------------------------------------------
// Utitlity class to load, compile and attach a vertex- and fragment shader to a program

class Ax_Shader
{
	uint32_t 		m_ProgramId;

	uint32_t 		get_compiled_shader( uint32_t shader_type, const std::string& shader_source);

public:
	Ax_Shader( void)
	{}

	// Load a vertex and a fragment shader from file
	bool Load(const std::string& vertexshader_file, const std::string& fragmentshader_file);

	// Use the program
	void use();

	// Delete the program
	void unload();

	uint32_t 	get_program_id() { return m_ProgramId; }

	void set_mat4(const glm::mat4& mat4, const std::string& name);

	void set_i1( int v, const std::string& name);
	void set_f1( float v, const std::string& name);
	void set_f3( float a, float b, float c, const std::string& name);

	void set_vec3( const glm::vec3& vec3, const std::string& name);
	void set_vec4( const glm::vec4& vec4, const std::string& name);
};

//---------------------------------------------------------------------------------------------------------------------------------
 

