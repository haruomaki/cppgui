#pragma once

#include <Draw.hpp>
#include <GUI.hpp>
#include <KeyCallback.hpp>
#include <Shader.hpp>
#include <Timer.hpp>
#include <Update.hpp>

template <typename T>
struct Rect {
    T left_, right_, bottom_, top_;
};

class WorldObject {
    Point<float> pos_;
    Point<float> abspos_;
    WorldObject *parent_ = nullptr;
    set<WorldObject *> children_;

    void refreshAbsolutePosition() {
        if (parent_ != nullptr) {
            abspos_ = parent_->abspos_ + pos_;
        }
        for (auto *child : children_) {
            child->refreshAbsolutePosition();
        }
    }

  public:
    WorldObject()
        : pos_{0, 0}
        , abspos_({0, 0}) {}

    ~WorldObject() {
        cout << "フリー！ " << this << endl;
        if (parent_ != nullptr) {
            parent_->children_.erase(this);
        }
        for (auto *child : children_) {
            child->parent_ = nullptr;
        }
    }

    // コピー禁止
    WorldObject(const WorldObject &) = delete;

    WorldObject &operator=(WorldObject &&other) noexcept {
        pos_ = other.pos_;
        abspos_ = other.abspos_;
        parent_ = other.parent_;
        children_ = other.children_;

        // 親の子情報を更新
        if (parent_ != nullptr) {
            parent_->children_.erase(&other);
            parent_->children_.insert(this);
        }

        // 子たちの親情報を更新
        for (auto *child : children_) {
            child->parent_ = this;
        }

        return *this;
    }

    WorldObject(WorldObject &&other) noexcept {
        *this = std::move(other);
    }

    void append(WorldObject *child) {
        child->parent_ = this;
        this->children_.insert(child);
        this->refreshAbsolutePosition();
    }

    void deleteRecursively() {
        for (auto *child : children_) {
            child->deleteRecursively();
        }
        delete this;
    }

    void showAbsolutePositionRecursively(int depth) const {
        // cout << "showです。" << endl;
        cout << std::string(depth, ' ') << this->abspos_ << endl;
        for (auto *child : children_) {
            child->showAbsolutePositionRecursively(depth + 1);
        }
    }

    [[nodiscard]] Point<float> getPosition() const {
        return pos_;
    }

    [[nodiscard]] Point<float> getAbsolutePosition() const {
        return abspos_;
    }

    void setPosition(Point<float> pos) {
        pos_ = pos;
        this->refreshAbsolutePosition();
    }

    WorldObject *getParent() {
        return parent_;
    }

    friend class Window;
};

class Window {
    GLFWwindow *gwin_;
    glm::mat4 view_matrix_ = glm::mat4(1);
    glm::vec3 camera_pos_;
    float camera_zoom_ = 1;
    static constexpr float default_camera_zoom = 400;
    bool looping_ = false;

  public:
    WorldObject world_object_root_;
    int tick_ = 0;
    ProgramObject shader_;

    Window(int width, int height);
    ~Window();
    void mainloop(const std::function<void()> &callback);
    pair<int, int> getWindowSize();
    pair<int, int> getFrameBufferSize();
    pair<float, float> getWindowContentScale();

    [[nodiscard]] const glm::mat4 &getViewMatrix() const;
    void setCamera(Point<float> pos, float zoom);
};

GLuint loadTexture(const string &filename);

#include <Polygon.hpp>