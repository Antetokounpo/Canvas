#include "shader.hpp"

#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include<map>

#include<GL/glew.h>
#include<glm/glm.hpp>

Shader::Shader()
{
    shader_program = glCreateProgram();
}

Shader::~Shader()
{
    glDeleteProgram(shader_program);
}

std::string Shader::read_file(const std::string& filename)
{
    std::ifstream t(filename);
    if(t.is_open())
    {
        std::stringstream buffer;
        buffer << t.rdbuf();
        t.close();
        return buffer.str();
    }

    std::cout << "Can't open file: " << filename << std::endl;
    return "";
}

void Shader::compile_shader(const std::string& source, GLuint shader_id)
{
    GLint result = GL_FALSE;
    int info_length;

    const char* source_str = source.c_str();

    glShaderSource(shader_id, 1, &source_str, NULL);
    glCompileShader(shader_id);

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_length);
    if(info_length > 0)
    {
        char error_msg[info_length+1];
        glGetShaderInfoLog(shader_id, info_length, NULL, error_msg);
        std::cout << "Error compiling shader: " << error_msg << std::endl;
    }
}

void Shader::add_uniform_location(const std::string& uniform_variable)
{
    uniform_var_dict[uniform_variable] = glGetUniformLocation(shader_program, uniform_variable.c_str());
}

void Shader::init_uniform_vars()
{
    add_uniform_location("iResolution");
    add_uniform_location("iTime");
}

std::string Shader::prepend_shadertoy_code(std::string shader_code)
{
    std::string header_code = "#version 330 core\nuniform vec2 iResolution;\nuniform float iTime;\n";
    std::string footer_code = "void main()\n{\nmainImage(gl_FragColor, gl_FragCoord.xy);\n}";
    return header_code+shader_code+footer_code;
}

void Shader::load(const std::string& vertex_shader_file, const std::string& fragment_shader_file)
{
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    compile_shader(read_file(vertex_shader_file), vertex_shader);
    compile_shader(prepend_shadertoy_code(read_file(fragment_shader_file)), fragment_shader);

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    GLint result;
    int info_length;

    glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_length);
    if(info_length > 0)
    {
        char error_msg[info_length+1];
        glGetProgramInfoLog(shader_program, info_length, NULL, error_msg);
        std::cout << "Error linking shader program: " << error_msg << std::endl;
    }

    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    init_uniform_vars(); // Uniform variables must be initialize after linking
}

void Shader::start()
{
    glUseProgram(shader_program);
}

void Shader::stop()
{
    glUseProgram(0);
}

GLuint Shader::get_id()
{
    return shader_program;
}

void Shader::set_uniform_variable(const float& v, const std::string& uniform_variable)
{
    glUniform1fv(uniform_var_dict[uniform_variable], 1, &v);
}

void Shader::set_uniform_variable(const glm::vec3& v, const std::string& uniform_variable)
{
    glUniform3fv(uniform_var_dict[uniform_variable], 1, &v[0]);
}

void Shader::set_uniform_variable(const glm::vec2& v, const std::string& uniform_variable)
{
    glUniform2fv(uniform_var_dict[uniform_variable], 1, &v[0]);
}

void Shader::set_uniform_variable(const glm::mat4& m, const std::string& uniform_variable)
{
    glUniformMatrix4fv(uniform_var_dict[uniform_variable], 1, GL_FALSE, &m[0][0]);
}
