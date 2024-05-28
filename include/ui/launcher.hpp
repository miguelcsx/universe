/*
 * @path include/graphics/launcher.hpp
 * @file launcher.hpp
*/

#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <memory>
#include <string_view>
#include <glm/glm.hpp>

class Scene;
class GLFWwindow;

class Launcher {
    public:
        static constexpr std::string_view TITLE = "N-Body Simulation";
        static constexpr std::string_view VERSION = "1.0.0";
        static constexpr std::string_view PROJECT_URL = "https://github.com/miguelcsx/universe.git";
        static constexpr std::string_view AUTHOR = "Miguel Cárdenas";

        Launcher();
        Launcher(const Launcher&) = delete;
        Launcher(Launcher&&) = delete;
        Launcher& operator=(const Launcher&) = delete;
        Launcher& operator=(Launcher&&) = delete;
        ~Launcher();

        void start();
        void handle_input();
        void handle_ui(float delta_time);
        void update_game(float delta_time);
        void update_screen();
        void reset();
        void toggle_pause();
        void update_viewport();
        void center_window();
        void toggle_fullscreen();
        void toggle_ui();
        void clear_screen() const;

        [[nodiscard]] bool is_minimized() const;

    private:
        GLFWwindow* window;
        int window_width = 1280;
        int window_height = 720;

        int display_width, display_height;
        int window_pos_x, window_pos_y;

        float delta_time = 1.0f / 60.0f;

        std::unique_ptr<Scene> scene;

        struct {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
            float w = 1.0f;
        } clear_color;

        static constexpr int MAX_BODY_COUNT = 10000000;

        #ifndef __EMSCRIPTEN__
            float point_size = 2.0f;
            bool fullscreen = false;
            bool ui = true;
        #else
            bool ui = false;
        #endif

        static void mouse_movement(const double& mouse_x, const double& mouse_y, double& move_x, double& move_y);
        static std::string_view get_gl_vendor();
        static std::string_view get_gl_version();
        static std::string_view get_glsl_version();
        static std::string_view get_glfw_version();
        static std::string_view get_glad_version();
        static std::string_view get_imgui_version();
        // static std::string_view get_glm_version();
};

#endif // LAUNCHER_HPP