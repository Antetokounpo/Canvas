#include "canvas.hpp"

#include<string>

#include<GL/glew.h>

Canvas::Canvas()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);

    load();
}

Canvas::~Canvas()
{
    glBindVertexArray(0); // Unbind any vao
    glDeleteVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for(GLuint& vbo: vbos)
        glDeleteBuffers(1, &vbo);
}

void Canvas::create_EBO(int indices[], size_t s_indices)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_indices, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind buffer
}

void Canvas::create_VBO(float data[], size_t s_data, unsigned int index, unsigned int coord_format)
{
    vbos.push_back(0); // Declare new vbo
    GLuint& vbo = vbos.back();

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, s_data, data, GL_STATIC_DRAW);
    glVertexAttribPointer(index, coord_format, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer
}

void Canvas::load()
{
    glBindVertexArray(vao);

    int indices[] = {
        3, 1, 2,
        3, 1, 0
    };

    float vertices[] = {
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,
        1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0
    };

    create_EBO(indices, 6*sizeof(int));
    create_VBO(vertices, 12*sizeof(float), 0, 3);

    glBindVertexArray(0); // Unbind VAO and associated VBOs

    vertex_count = 6;
}

void Canvas::render()
{
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO
    glDisableVertexAttribArray(0);
    glBindVertexArray(0); // Unbind VAO
}
