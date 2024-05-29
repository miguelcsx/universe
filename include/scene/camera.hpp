/**
 * @path include/scene/camera.hpp
 * @file camera.hpp
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        // Camera attributes
        float movement_speed = 3.0f;
        float rotation_speed = 0.1f;
        float field_of_view = 90.0f;
        float near_plane = 0.1f;
        float far_plane =100.0f;

        // Position and rotation of the camera
        float yaw;
        float initial_yaw = -90.0f;
        
        float pitch;
        float initial_pitch = 0.0f;
        bool constrain_pitch = true;
        
        glm::vec3 position;
        glm::vec3 initial_position = glm::vec3(0.0f, 0.0f, 1.0f);

        // Movement buffer to take into account the delta time
        glm::vec3 movement_buffer;
        glm::vec3 camera_front_buffer = glm::vec3(0.0f, 0.0f, -1.0f);

        Camera(int display_width, int display_height);
        void reset();
        void update(float delta_time);
        
        // Camera movement
        void move_forward();
        void move_backward();
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        void mouse_move(float x_offset, float y_offset);

        void zoom_in();
        void zoom_out();

        void update_projection_matrix(int display_width, int display_height);
        
        [[nodiscard]] glm::mat4 get_view_matrix() const;
        [[nodiscard]] glm::mat4 get_projection_matrix() const;
    
    private:
        // Matrix transformations
        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;
        glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

        // void update_camera_vectors();
        void update_view_matrix();

};

#endif // CAMERA_HPP