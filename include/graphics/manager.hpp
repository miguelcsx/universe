/*
 * @path include/graphics/manager.hpp
 * @file manager.hpp
*/

#ifndef MANAGER_HPP
#define MANAGER_HPP

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

struct GLFWwindow;

class Manager {
    public:
        // Callback function for key events
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        // Functions to check if a key is pressed
        static bool is_forward_pressed(GLFWwindow* window);
        static bool is_backward_pressed(GLFWwindow* window);
        static bool is_left_pressed(GLFWwindow* window);
        static bool is_right_pressed(GLFWwindow* window);
        static bool is_up_pressed(GLFWwindow* window);
        static bool is_down_pressed(GLFWwindow* window);

        // Functions for mouse events
        static void get_mouse_position(GLFWwindow* window, double& x, double& y);
        static bool is_key_move_pressed(GLFWwindow* window);
        static bool is_key_set_pressed(GLFWwindow* window);

        #ifdef __EMSCRIPTEN__
        struct DragData {
            double x = 0.0f;
            double y = 0.0f;
            bool is_dragging = false;
        };
        static Manager::DragData drag_data;
        static EM_BOOL touch_end_callback(int event_type, const EmscriptenTouchEvent* touch_event, void* user_data);
        static EM_BOOL touch_start_callback(int event_type, const EmscriptenTouchEvent* touch_event, void* user_data);
        static EM_BOOL touch_move_callback(int event_type, const EmscriptenTouchEvent* touch_event, void* user_data);
        #endif
};

#endif // MANAGER_HPP
