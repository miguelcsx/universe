/**
 * @path src/scene/scene.cpp
 * @file scene.cpp
*/

#include "../include/scene/scene.hpp"

Scene::Scene(int width, int height) :
    camera(width, height) {
        simulate();
}

void Scene::update(float delta_time) {
    camera.update(delta_time);
    if (paused){
        return;
    }

    n_body->update(delta_time);

}

void Scene::render() {
    n_body->render(camera.get_view_matrix(), camera.get_projection_matrix());
}

void Scene::update_projection_matrix(int width, int height) {
    camera.update_projection_matrix(width, height);
}

void Scene::toggle_pause() {
    paused = !paused;
    n_body->set_paused(paused);
}

void Scene::reset() {
    camera.reset();
    n_body->reset();
}

bool Scene::is_paused() const {
    return paused;
}

void Scene::simulate() {
    if (n_body != nullptr) {
        delete n_body;
    }
    n_body = new BarnesHut(10000);
}
