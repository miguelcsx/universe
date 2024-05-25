/**
 * @path src/scene/camera.cpp
 * @file camera.cpp
*/

#include "../include/scene/camera.hpp"

Camera::Camera(int display_width, int display_height) {
    yaw = initial_yaw;
    pitch = initial_pitch;
    position = initial_position;
    update_view_matrix();
    update_projection_matrix(display_width, display_height);
}

void Camera::reset() {
    yaw = initial_yaw;
    pitch = initial_pitch;
    position = initial_position;
    mouse_move(0.0f, 0.0f);
}

void Camera::update(float delta_time) {
    // Update the camera front vector
    // camera_front = glm::normalize(camera_front_buffer);
    // Update the camera right and up vectors
    // camera_right = glm::normalize(glm::cross(camera_front, world_up));
    // camera_up = glm::normalize(glm::cross(camera_right, camera_front));
    // Update the view matrix
    // view_matrix = glm::lookAt(position, position + camera_front, camera_up);
    position += movement_buffer * movement_speed * delta_time;
    update_view_matrix();
    movement_buffer = glm::vec3(0.0f);
}

void Camera::update_view_matrix() {
    view_matrix = glm::lookAt(position, position + camera_front_buffer, camera_up);

}

void Camera::update_projection_matrix(int display_width, int display_height) {
    // Update the projection matrix with the new display size
    projection_matrix = glm::perspective(
        glm::radians(field_of_view / 2),
        static_cast<float>(display_width) / static_cast<float>(display_height),
        near_plane,
        far_plane
    );
}

void Camera::move_forward() {
    movement_buffer += camera_front;
}

void Camera::move_backward() {
    movement_buffer -= camera_front;
}

void Camera::move_left() {
    movement_buffer -= camera_right;
}

void Camera::move_right() {
    movement_buffer += camera_right;
}

void Camera::move_up() {
    movement_buffer += camera_up;
}

void Camera::move_down() {
    movement_buffer -= camera_up;
}

void Camera::mouse_move(float x_offset, float y_offset) {
    yaw += x_offset * rotation_speed;
    pitch += y_offset * rotation_speed;
    if (constrain_pitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }
    else {
        if (pitch > 360.0f) {
            pitch -= 360.0f;
        }
        if (pitch < -360.0f) {
            pitch += 360.0f;
        }
    }

    if (yaw > 360.0f) {
        yaw -= 360.0f;
    }
    if (yaw < -360.0f) {
        yaw += 360.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front_buffer = glm::normalize(front);
}

glm::mat4 Camera::get_view_matrix() const {
    return view_matrix;
}

glm::mat4 Camera::get_projection_matrix() const {
    return projection_matrix;
}
