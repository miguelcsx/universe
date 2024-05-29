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

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif


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
        const char* glsl_version = "#version 300 es";
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

    std::string title = TITLE.data();
    title = title + " - " + VERSION.data();
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
        if (gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
            std::cerr << "Failed to initialize OpenGL ES" << std::endl;
            exit(EXIT_FAILURE);
        }
    #else
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
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
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    #ifdef __EMSCRIPTEN__
        emscripten_set_touchstart_callback("#canvas", (void*)&Manager::drag_data, true, Manager::touch_start_callback);
        emscripten_set_touchend_callback("#canvas", (void*)&Manager::drag_data, true, Manager::touch_end_callback);
        emscripten_set_touchmove_callback("#canvas", (void*)&Manager::drag_data, true, Manager::touch_move_callback);
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

    std::cout << "GL Vendor: " << get_gl_vendor() << std::endl;
    std::cout << "GL Version: " << get_gl_version() << std::endl;
    std::cout << "GLSL Version: " << get_glsl_version() << std::endl;
    std::cout << "GLFW Version: " << get_glfw_version() << std::endl;
    std::cout << "GLAD Version: " << get_glad_version() << std::endl;
    std::cout << "ImGui Version: " << get_imgui_version() << std::endl;

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
        delta_time = std::chrono::duration<float>(current_time - last_time).count();
        auto start_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());

        handle_input();
        update_game(delta_time);
        handle_ui(delta_time);
        update_screen();

        auto  end_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
        auto delay = delta_time - std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time).count();
        if (delay > 0.0f) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 1000.0f)));
        }

        last_time = current_time;
    }
    #ifdef __EMSCRIPTEN__
        EMSCRIPTEN_MAIN_LOOP_END;
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
        Manager::get_mouse_position(window, mouse_x, mouse_y);
    #ifdef __EMSCRIPTEN__
    }
    #endif

    double mouse_delta_x = 0.0f, mouse_delta_y = 0.0f;

    mouse_movement(mouse_x, mouse_y, mouse_delta_x, mouse_delta_y);

    // Read mouse movement
    if (Manager::is_key_move_pressed(window)) {
        scene->camera.mouse_move(static_cast<float>(mouse_delta_x), static_cast<float>(mouse_delta_y));
    }
}

void Launcher::handle_ui(float delta_time) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ui) {
        {
            ImGui::Begin("Information");
            ImGui::Text("Author: %s", AUTHOR.data());
            ImGui::Text("Project: %s", TITLE.data());
            ImGui::Text("%.3f ms/frame (%1.f fps)", delta_time, 1.0f / delta_time);
            ImGui::Text("Window width: %d", display_width);
            ImGui::Text("Window height: %d", display_height);
            ImGui::Text("OpenGl Version: %s", get_gl_version().data());
            ImGui::End();
        }
       {
            ImGui::Begin("Camera settings");
            
            // Camera position
            ImGui::Text("Position: ");
            ImGui::DragFloat3("##position", reinterpret_cast<float*>(&scene->camera.position));
            // Reset camera position
            ImGui::Text("Reset position: ");
            ImGui::DragFloat3("##reset_position", reinterpret_cast<float*>(&scene->camera.initial_position));
            ImGui::Button("Reset");
            if (ImGui::IsItemClicked()) {
                scene->camera.reset();
            }
            ImGui::NewLine();

            // Pitch
            ImGui::Text("Pitch: ");
            ImGui::Checkbox("Pitch constrained", &scene->camera.constrain_pitch);
            ImGui::DragFloat("##pitch", &scene->camera.pitch);
            // Yaw
            ImGui::Text("Yaw: ");
            ImGui::DragFloat("##yaw", &scene->camera.yaw);

            ImGui::NewLine();

            // Field of view
            ImGui::Text("Field of view: ");
            ImGui::DragFloat("##fov", &scene->camera.field_of_view, 0.1f, 1.0f, 180.0f);

            ImGui::NewLine();

            // Near plane
            ImGui::Text("Near plane: ");
            ImGui::DragFloat("##near_plane", &scene->camera.near_plane, 0.1f);
            // Far plane
            ImGui::Text("Far plane: ");
            ImGui::DragFloat("##far_plane", &scene->camera.far_plane, 0.1f);
            
            ImGui::NewLine();

            // Camera speed
            ImGui::Text("Speed: ");
            ImGui::DragFloat("##speed", &scene->camera.movement_speed);

            // Sensitivity
            ImGui::Text("Sensitivity: ");
            ImGui::DragFloat("##sensitivity", &scene->camera.rotation_speed, 0.1f);

            ImGui::End();
       }
       {
            ImGui::Begin("Simulator settings");
            ImGui::Text("Start Simulation");
            ImGui::Button("Start##start_btn");
            if (ImGui::IsItemClicked()) {
                scene->simulate();
            }

            ImGui::NewLine();
            
            ImGui::Text("Body count: ");
            static int body_count = static_cast<int>(scene->n_body->get_body_count());
            ImGui::DragInt("##body_count", &body_count, 1, 1, MAX_BODY_COUNT);
            ImGui::Button("Validate##body_count_setter");
            if (ImGui::IsItemClicked()) {
                scene->n_body->set_body_count(body_count);
            }
            
            ImGui::NewLine();

            ImGui::Text("Reset Simulation");
            ImGui::Button("Reset##reset_btn");
            if (ImGui::IsItemClicked()) {
                reset();
            }

            ImGui::NewLine();

            ImGui::Text("Pause Simulation");
            ImGui::Button(scene->is_paused() ? "Resume##pause_btn" : "Pause##pause_btn");
            if (ImGui::IsItemClicked()) {
                toggle_pause();
            }

            ImGui::NewLine();

            #ifndef __EMSCRIPTEN__
            ImGui::Text("Point size: ");
            ImGui::DragFloat("##point_size", &point_size, 0.1f, 1.0f, 100.0f);
            ImGui::Button("Validate##point_size_setter");
            if (ImGui::IsItemClicked()) {
                glPointSize(point_size);
            }
            #endif

            ImGui::NewLine();

            ImGui::Text("Spawn position: ");
            ImGui::DragFloat3("##spawn_position", reinterpret_cast<float*>(&scene->n_body->position));

            ImGui::NewLine();

            ImGui::Text("Radius: ");
            ImGui::DragFloat("##radius", &scene->n_body->radius, 0.1f, 0.1f, 100.0f);

            ImGui::NewLine();

            ImGui::Text("Body mass: ");
            ImGui::DragFloat("##body_mass", &scene->n_body->mass, 0.1f, 0.1f, 100.0f);

            ImGui::NewLine();

            ImGui::Text("Gravity: ");
            ImGui::DragFloat("##gravity", &scene->n_body->gravity, 0.1f, 0.1f, 100.0f);

            ImGui::Text("Softening Factor: ");
            ImGui::DragFloat("##softening", &scene->n_body->softening_factor, 0.1f, 0.1f, 100.0f);

            ImGui::NewLine();

            ImGui::Text("Damping:");
            ImGui::DragFloat("##damping", &scene->n_body->damping, 0.0f, 0.0f, 0.1f);

            ImGui::Text("Interaction Percentage: ");
            ImGui::DragFloat("##interaction_percentage", &scene->n_body->interaction_percentage, 0.0f, 0.0f, 1.0f);

            ImGui::Text("Theta:");
            ImGui::DragFloat("##theta", &scene->n_body->theta, 0.0f, 0.0f, 1.0f);

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
            fullscreen = false;
        }
        else {
            glfwGetWindowPos(window, &window_pos_x, &window_pos_y);
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
            fullscreen = true;
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
    static double last_x = 0.0f;
    static double last_y = 0.0f;

    move_x = mouse_x - last_x;
    move_y = last_y - mouse_y;

    last_x = mouse_x;
    last_y = mouse_y;
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

