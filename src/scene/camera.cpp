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
    movement_buffer = glm::vec3(0.0f);
}

void Camera::update(float delta_time) {
    position += movement_buffer * movement_speed * delta_time;
    update_view_matrix();
    movement_buffer = glm::vec3(0.0f);
}

void Camera::update_view_matrix() {
    view_matrix = glm::lookAt(position, position + glm::normalize(camera_front_buffer), camera_up);

}


void Camera::update_projection_matrix(int display_width, int display_height) {
    // Update the projection matrix with the new display size
    projection_matrix = glm::perspective(
        glm::radians(field_of_view * 0.5f),
        static_cast<float>(display_width) / static_cast<float>(display_height),
        near_plane,
        far_plane
    );
}

void Camera::move_forward() {
    movement_buffer += glm::normalize(camera_front_buffer);
}

void Camera::move_backward() {
    movement_buffer -= glm::normalize(camera_front_buffer);
}

void Camera::move_left() {
    movement_buffer -= glm::cross(camera_front_buffer, camera_up);
}

void Camera::move_right() {
    movement_buffer += glm::cross(camera_front_buffer, camera_up);
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

    // Clamp the pitch angle
    if (constrain_pitch) {
        pitch = std::max(std::min(pitch, 89.0f), -89.0f);
    }
    else {
        pitch = fmod(pitch, 360.0f);
    }

   // Wrap the yaw angle
   yaw = fmod(yaw, 360.0f);

    const float yaw_radians = glm::radians(yaw);
    const float pitch_radians = glm::radians(pitch);

    camera_front_buffer.x = cos(yaw_radians) * cos(pitch_radians);
    camera_front_buffer.y = sin(pitch_radians);
    camera_front_buffer.z = sin(yaw_radians) * cos(pitch_radians);

    camera_front_buffer = glm::normalize(camera_front_buffer);
}

glm::mat4 Camera::get_view_matrix() const {
    return view_matrix;
}

glm::mat4 Camera::get_projection_matrix() const {
    return projection_matrix;
}
