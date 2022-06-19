#pragma once

#include<vector>
#include<string>

#include<GL/glew.h>

class Canvas
{
    public:
        Canvas();
        ~Canvas();
        void render();
    private:
        void create_VBO(float data[], size_t s_data, unsigned int index, unsigned int coord_format);
        void create_EBO(int indices[], size_t s_indices);
        void load();

        GLuint vao;
        GLuint ebo;
        std::vector<GLuint> vbos; 
        int vertex_count;
};
