// ax_shader.cpp ----------------------------------------------------------------------------------------------------------------

#include "utils/ax_includes.h"
#include "abraxas/render/ax_shader.h"

 
//---------------------------------------------------------------------------------------------------------------------------------

uint32_t Ax_Shader::get_compiled_shader( uint32_t shader_type, const std::string& shader_source)
{
	uint32_t 		shader_id = glCreateShader(shader_type);

	const char		*c_source = shader_source.c_str();
	glShaderSource( shader_id, 1, &c_source, nullptr);
	glCompileShader( shader_id);

	GLint		 	result;
	glGetShaderiv( shader_id, GL_COMPILE_STATUS, &result);

	if ( result == GL_FALSE)
	{
		int 	length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

		GLchar	*strInfoLog = new GLchar[length + 1];
		glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

		fprintf(stderr, "Compile error in shader: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	return shader_id;
}

//---------------------------------------------------------------------------------------------------------------------------------

bool Ax_Shader::Load( const std::string &vertexshader_file, const std::string &fragmentshader_file)
{
	std::ifstream is_vs(vertexshader_file);
	const std::string 	f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

	std::ifstream is_fs( fragmentshader_file);
	const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

	m_ProgramId = glCreateProgram();

	uint32_t vs = get_compiled_shader(GL_VERTEX_SHADER, f_vs);
	uint32_t fs = get_compiled_shader(GL_FRAGMENT_SHADER, f_fs);

	glAttachShader(m_ProgramId, vs);
	glAttachShader(m_ProgramId, fs);

	glLinkProgram( m_ProgramId);
	glValidateProgram( m_ProgramId);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Shader::use()
{
	glUseProgram( m_ProgramId);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Shader::unload()
{
	glDeleteProgram( m_ProgramId);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Shader::set_mat4( const glm::mat4& mat4, const std::string& name)
{
	GLint myLoc = glGetUniformLocation( m_ProgramId, name.c_str());
	glUniformMatrix4fv(myLoc, 1, GL_FALSE, glm::value_ptr(mat4));
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Shader::set_i1( int v, const std::string& name)
{
	GLint myLoc = glGetUniformLocation( m_ProgramId, name.c_str());
	glUniform1i(myLoc, v);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Shader::set_f1( float v, const std::string& name)
{
	GLint myLoc = glGetUniformLocation( m_ProgramId, name.c_str());
	glUniform1f(myLoc, v);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Shader::set_f3( float a, float b, float c, const std::string& name)
{
	GLint myLoc = glGetUniformLocation( m_ProgramId, name.c_str());
	glUniform3f(myLoc, a, b, c);
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Shader::set_vec3( const glm::vec3& vec3, const std::string& name)
{
	GLint myLoc = glGetUniformLocation(  m_ProgramId, name.c_str());
	glProgramUniform3fv( m_ProgramId, myLoc, 1, glm::value_ptr(vec3));
}

//---------------------------------------------------------------------------------------------------------------------------------

void Ax_Shader::set_vec4( const glm::vec4& vec4, const std::string& name)
{
    GLint myLoc = glGetUniformLocation( m_ProgramId, name.c_str());
    glProgramUniform4fv( m_ProgramId, myLoc, 1, glm::value_ptr(vec4));
}

//---------------------------------------------------------------------------------------------------------------------------------
 

