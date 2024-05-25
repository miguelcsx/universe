/*
 * @path src/graphics/launcher.cpp
 * @file launcher.cpp
*/

#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../include/ui/launcher.hpp"
#include "../include/graphics/manager.hpp"
#include "../include/scene/scene.hpp"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#include "imgui/libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

Launcher::Launcher() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Set OpenGL version
    #if defined(__EMSCRIPTEN__)
        const char* glsl_version = "#version 330 core";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #else
        const char* glsl_version = "#version 430";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create window
    #ifdef __EMSCRIPTEN__
        emscripten_get_canvas_element_size("#canvas", &window_width, &window_height);
    #else
        display_width = window_width;
        display_height = window_height;
    #endif

    std::string title = "N-Body Simulation";
    title += " (U - Toggle UI)";
    window = glfwCreateWindow(window_width, window_height, title.c_str(), nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, Manager::key_callback);

    #ifdef __EMSCRIPTEN__
        if (gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(emscripten_GetProcAddress)) == 0) {
            std::cerr << "Failed to initialize OpenGL ES" << std::endl;
            exit(EXIT_FAILURE);
        }
    #else
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize OpenGL context" << std::endl;
            exit(EXIT_FAILURE);
        }
    #endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // Enable keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    #ifdef __EMSCRIPTEN__
        emscripten_set_touchstart_callback("#canvas", this, true, Manager::touch_start_callback);
        emscripten_set_touchend_callback("#canvas", this, true, Manager::touch_end_callback);
        emscripten_set_touchmove_callback("#canvas", this, true, Manager::touch_move_callback);
    #endif

    #ifndef __EMSCRIPTEN__
        center_window();
    #endif

    // Initialize scene
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    #ifndef __EMSCRIPTEN__
        glPointSize(point_size);
    #endif

}

Launcher::~Launcher() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Launcher::start() {
    scene = std::make_unique<Scene>(window_width, window_height);

    std::chrono::high_resolution_clock::time_point last_time = std::chrono::high_resolution_clock::now();
    float delta_time = 0.0f;

    #ifdef __EMSCRIPTEN__
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.IniFilename = nullptr;
        EMSCRIPTEN_MAIN_LOOP_BEGIN
    #else
        while (glfwWindowShouldClose(window) == 0)
    #endif
    {
        std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
        delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - last_time).count();
        auto start_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());

        handle_input();
        update_game(delta_time);
        handle_ui(delta_time);
        update_screen();

        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
        auto delay = std::chrono::duration<float, std::chrono::seconds::period>(end_time - start_time).count();
        if (delay > 0.0f) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 1000.0f)));
        }

        last_time = current_time;
    }
    #ifdef __EMSCRIPTEN__
        EMSCRIPTEN_MAIN_LOOP_END
    #endif

    scene.reset();
}

void Launcher::handle_input() {
    glfwPollEvents();

    if (Manager::is_forward_pressed(window)) {
        scene->camera.move_forward();
    }

    if (Manager::is_backward_pressed(window)) {
        scene->camera.move_backward();
    }

    if (Manager::is_left_pressed(window)) {
        scene->camera.move_left();
    }

    if (Manager::is_right_pressed(window)) {
        scene->camera.move_right();
    }

    if (Manager::is_up_pressed(window)) {
        scene->camera.move_up();
    }

    if (Manager::is_down_pressed(window)) {
        scene->camera.move_down();
    }

    double mouse_x = 0.0f, mouse_y = 0.0f;

    #ifdef __EMSCRIPTEN__
    if (Manager::drag_data.is_dragging) {
        mouse_x = Manager::drag_data.x;
        mouse_y = Manager::drag_data.y;
    } else {
    #endif
        Manager::get_mouse_position(window, &mouse_x, &mouse_y);
    #ifdef __EMSCRIPTEN__
    }
    #endif

    double mouse_delta_x = 0.0f, mouse_delta_y = 0.0f;

    mouse_movement(mouse_x, mouse_y, mouse_delta_x, mouse_delta_y);

    // Read mouse movement
    if (Manager::is_key_mouse_pressed(window)) {
        scene->camera.mouse_move(static_cast<float>(mouse_delta_x), static_cast<float>(mouse_delta_y));
    }
}

void Launcher::handle_ui(float delta_time) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ui) {
        {
        ImGui::Begin("Controls", &ui);
        ImGui::Text("WASD - Move Camera");
        ImGui::Text("Mouse - Rotate Camera");
        ImGui::Text("Right Mouse - Drag Camera");
        ImGui::Text("Scroll Wheel - Zoom Camera");
        ImGui::Text("Space - Pause Simulation");
        ImGui::Text("R - Reset Simulation");
        ImGui::Text("F11 - Toggle Fullscreen");
        ImGui::End();
        }
    }
    ImGui::Render();
}

void Launcher::update_game(float delta_time) {
    scene->update(delta_time);
}

void Launcher::update_screen() {
    if (!is_minimized()) {
        update_viewport();
    }

    clear_screen();
    scene->render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    const ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(window);
}


void Launcher::reset() {
    scene->reset();
}

void Launcher::toggle_pause() {
    scene->toggle_pause();
}

void Launcher::update_viewport() {
    #ifdef __EMSCRIPTEN__
        emscripten_get_canvas_element_size("#canvas", &window_width, &window_height);
    #else
        glfwGetFramebufferSize(window, &window_width, &window_height);
        if (!fullscreen) {
            glfwGetWindowSize(window, &window_width, &window_height);
        }
    #endif
    scene->update_projection_matrix(window_width, window_height);
    glViewport(0, 0, window_width, window_height);
}

void Launcher::center_window() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window_pos_x = (mode->width - window_width) / 2;
    window_pos_y = (mode->height - window_height) / 2;
    glfwSetWindowPos(window, window_pos_x, window_pos_y);
}

void Launcher::toggle_fullscreen() {
    #ifndef __EMSCRIPTEN__
        if (fullscreen) {
            glfwSetWindowMonitor(window, nullptr, 0, 0, display_width, display_height, GLFW_DONT_CARE);
            display_width = window_width;
            display_height = window_height;
            glfwGetWindowPos(window, &window_pos_x, &window_pos_y);
            fullscreen = true;
        }
        else {
            glfwGetWindowPos(window, &window_pos_x, &window_pos_y);
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
        }
    #endif
}

void Launcher::toggle_ui() {
    ui = !ui;
}

void Launcher::clear_screen() const {
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Launcher::is_minimized() const {
    return glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0;
}

void Launcher::mouse_movement(const double& mouse_x, const double& mouse_y, double& move_x, double& move_y) {
    static double last_mouse_x = mouse_x;
    static double last_mouse_y = mouse_y;

    move_x = mouse_x - last_mouse_x;
    move_y = last_mouse_y - mouse_y;

    last_mouse_x = mouse_x;
    last_mouse_y = mouse_y;
}

std::string_view Launcher::get_gl_vendor() {
    return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}

std::string_view Launcher::get_gl_version() {
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}


std::string_view Launcher::get_glsl_version() {
    return reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

std::string_view Launcher::get_glfw_version() {
    return glfwGetVersionString();
}

std::string_view Launcher::get_glad_version() {
    return "0.1.36";
}

std::string_view Launcher::get_imgui_version() {
    return IMGUI_VERSION;
}
