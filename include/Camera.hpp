#pragma once

#include <Window.hpp>

class Camera : public WorldObject {
  protected:
    const Window &window_;
    float aspect_ratio_ = 1;

  public:
    Camera(const Window &window)
        : window_(window) {}
    // glm::mat4 view_matrix_ = glm::mat4(1);
    // glm::mat4 projection_matrix_ = glm::perspective(glm::radians(90.0F), 1.f, 0.1F, 1000.f);
    glm::mat4 projection_matrix_ = glm::ortho<float>(-500, 500, -400, 400, 10, -10);
    // glm::mat4 projection_matrix_ = glm::ortho(-1, 1, -1, 1, 1, -1);

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        // return glm::inverse(getAbsoluteTransform());
        return projection_matrix_ * glm::inverse(getAbsoluteTransform());
    }
};

class NormalCamera : public Camera, SizeCallback {
  public:
    NormalCamera(const Window &window)
        : Camera(window) {
        auto pos = window.getFrameBufferSize();
        int width = pos.first;
        int height = pos.second;
        sizeCallback(width, height);
    }

    void sizeCallback(int width, int height) override {
        aspect_ratio_ = width / height;
        projection_matrix_ = glm::ortho<float>(-width / 2, width / 2, -height / 2, height / 2, 1, -1);
        // debug(projection_matrix_);
    }
};
