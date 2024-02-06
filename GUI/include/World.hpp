#pragma once

#include <Camera.hpp>

class World : public WorldObject {
    set<function<void(const Camera &)> *> draws_;
    FunctionSet<void()> updates_;
    Camera *active_camera_ = nullptr;

    friend class DrawableWorldObject;
    friend class Update;

    friend class Window;
    int draw_priority_ = 0;

  public:
    Window &window;

    World(Window &window, int draw_priority)
        : WorldObject(*this) // Worldにのみ許されたプライベートコンストラクタ
        , draw_priority_(draw_priority)
        , window(window) {}

    ~World() override {
        print("Worldのデストラクタ");
        this->children_.clear(); // draws_やupdates_が消える前にUpdate等のデストラクタを呼ぶ
    }

    World(const World &) = delete;
    World &operator=(const World &) const = delete;
    World(World &&) = delete;
    World &operator=(World &&) const = delete;

    void master_draw() {
        if (this->active_camera_ == nullptr) {
            print("警告: アクティブなカメラが存在しません");
        }
        for (auto *draw : draws_) {
            (*draw)(*this->active_camera_);
        }
    }

    void master_update() {
        for (const auto &[id, update] : this->updates_) {
            update();
        }
    }

    void set_active_camera(Camera &camera) {
        this->active_camera_ = &camera;
    }

    void set_draw_priority(int priority) {
        this->draw_priority_ = priority;
        this->window.refresh_world_order();
    }
};
