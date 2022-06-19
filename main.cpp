/* STL includes */
#include<iostream>
#include<vector>

/* SDL/OpenGL/GLM includes */
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<GL/glew.h>
#include<glm/gtc/matrix_transform.hpp>

/* ImGui includes */
#include<imgui/imgui.h>
#include<imgui/imgui_impl_sdl.h>
#include<imgui/imgui_impl_opengl3.h>

/* Canvas includes */
#include "canvas.hpp"
#include "shader.hpp"

void init_imgui(SDL_Window* window, SDL_GLContext context)
{
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void imgui_prepare_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}


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

    // Init ImGui
    init_imgui(window, context);

    bool quit = false;
    Uint32 time_delta = SDL_GetTicks();
    while(!quit)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if(e.type == SDL_QUIT)
                quit = true;
        }
        glClearColor(1, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        imgui_prepare_frame();

        ImGui::Begin("Debug Info");
        ImGui::Text("FPS: %.1f", 1.0f/((SDL_GetTicks()-time_delta)/1000.0f));
        time_delta = SDL_GetTicks();

        ImGui::End();

        shader.start();

        shader.set_uniform_variable(glm::vec2(1280, 720), "iResolution");
        shader.set_uniform_variable(SDL_GetTicks()/1000.0f, "iTime");

        canvas.render();

        shader.stop();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
