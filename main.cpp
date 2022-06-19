#include<iostream>
#include<vector>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<GL/glew.h>
#include<glm/gtc/matrix_transform.hpp>

#include "canvas.hpp"
#include "shader.hpp"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow("Canvas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        std::cout << glewGetErrorString(err) << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG);

    Shader shader;
    shader.load("shaders/vertex_shader.glsl", (argc > 1) ? argv[1] : "shaders/fragment_shader.glsl");
    Canvas canvas;

    SDL_GL_SetSwapInterval(1);
    SDL_ShowCursor(SDL_ENABLE);

    glEnable(GL_DEPTH_TEST); // So triangles don't "overlap"

    bool quit = false;
    while(!quit)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                quit = true;
        }
        glClearColor(1, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.start();

        shader.set_uniform_variable(glm::vec2(1280, 720), "iResolution");
        shader.set_uniform_variable(SDL_GetTicks()/1000.0f, "iTime");

        canvas.render();

        shader.stop();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
