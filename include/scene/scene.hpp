/**
 * @path include/scene/scene.hpp
 * @file scene.hpp
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include "camera.hpp"
#include "../include/simulation/n_body.hpp"
#include "../include/simulation/barnes_hut.hpp"

class Scene {
    public:
        Camera camera;
        NBody *n_body = nullptr;

        Scene(int width, int height);
        void update(float delta_time);
        void render();

        void update_projection_matrix(int width, int height);
        void toggle_pause() ;
        void reset();

        [[nodiscard]] bool is_paused() const;

        void simulate();

    private:
        bool paused = false;
};

#endif // SCENE_HPP