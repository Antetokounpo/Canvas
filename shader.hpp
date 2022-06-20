#pragma once

#include<string>
#include<map>
#include<vector>

#include<GL/glew.h>
#include<glm/glm.hpp>

class Shader
{
    public:
        Shader();
        ~Shader();

        void load(const std::string& vertex_shader_file, const std::string& fragment_shader_file);
        void start();
        void stop();
        GLuint get_id();

        void set_uniform_variable(const float& v, const std::string& uniform_variable);
        void set_uniform_variable(const glm::vec3& v, const std::string& uniform_varible);
        void set_uniform_variable(const glm::vec2& v, const std::string& uniform_varible);
        void set_uniform_variable(const glm::mat4& v, const std::string& uniform_varible);

        static std::vector<std::string> get_uniform_vars_from_shader(const std::string& shader_filename);
        const std::vector<std::string>& get_uniform_vars();
        void set_uniform_vars(const std::vector<std::string>& vars);
    private:
        std::string read_file(const std::string& filename);
        void compile_shader(const std::string& source, GLuint shader_id);
        void add_uniform_location(const std::string& uniform_variable);
        void init_uniform_vars();

        std::string prepend_shadertoy_code(std::string fragment_code);

        std::map<std::string, GLuint> uniform_var_dict;
        std::vector<std::string> uniform_vars;
        GLuint shader_program;
};