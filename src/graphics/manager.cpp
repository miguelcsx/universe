/*
 * @path src/graphics/manager.cpp
 * @file manager.cpp 
*/

#include <GLFW/glfw3.h>
#include "graphics/manager.hpp"
#include "ui/launcher.hpp"

void Manager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    Launcher* engine = static_cast<Launcher*>(glfwGetWindowUserPointer(window));

    switch(key) {
        case GLFW_KEY_ESCAPE:
            if(action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            break;

        case GLFW_KEY_U:
            if(action == GLFW_PRESS) {
                engine->toggle_ui();
            }
            break;

        case GLFW_KEY_R:
            if(action == GLFW_PRESS) {
                engine->reset();
            }
            break;

        case GLFW_KEY_SPACE:
            if(action == GLFW_PRESS) {
                engine->toggle_pause();
            }
            break;

        case GLFW_KEY_F11:
            if(action == GLFW_PRESS) {
                engine->toggle_fullscreen();
            }
            break;

    }
}

bool Manager::is_forward_pressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS;
}

bool Manager::is_backward_pressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS;
}

bool Manager::is_left_pressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
}

bool Manager::is_right_pressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
}

bool Manager::is_up_pressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
}

bool Manager::is_down_pressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
}

bool Manager::is_key_move_pressed(GLFWwindow* window) {
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

bool Manager::is_key_set_pressed(GLFWwindow* window) {
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void Manager::get_mouse_position(GLFWwindow* window, double& x_pos, double& y_pos) {
    glfwGetCursorPos(window, &x_pos, &y_pos);
}

#ifdef __EMSCRIPTEN__
Manager::DragData Manager::drag_data = {0.0f, 0.0f, false};

// Emscripten touch event callbacks
EM_BOOL Manager::touch_end_callback(int event_type, const EmscriptenTouchEvent* touch_event, void* user_data) {
    DragData* drag_buffer = static_cast<DragData*>(user_data);
    drag_buffer->is_dragging = false;
    return EM_TRUE;
}

EM_BOOL Manager::touch_start_callback(int event_type, const EmscriptenTouchEvent* touch_event, void* user_data) {
    DragData* drag_buffer = static_cast<DragData*>(user_data);
    drag_buffer->is_dragging = true;
    drag_buffer->x = touchEvent->touches[0].targetX;
    drag_buffer->y = touchEvent->touches[0].targetY;
    return EM_TRUE;
}

EM_BOOL Manager::touch_move_callback(int event_type, const EmscriptenTouchEvent* touch_event, void* user_data) {
    DragData* drag_buffer = static_cast<DragData*>(user_data);
    drag_buffer->x = touchEvent->touches[0].targetX;
    drag_buffer->y = touchEvent->touches[0].targetY;
    return EM_TRUE;
}

#endif
