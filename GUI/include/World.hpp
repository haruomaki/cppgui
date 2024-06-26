#pragma once

#include "Mesh.hpp"
#include "Timer.hpp"

class Camera;
class Rigidbody;

class World : public WorldObject {
    Camera *active_camera_ = nullptr;

    friend class Window;
    int draw_priority_ = 0;

    // メッシュ描画を一元管理するクラス
    friend class MeshObject;
    MeshDrawManager mesh_draw_manager_;
    friend class WorldObject; // mesh_draw_manager_にアクセスするため

  public:
    Window &window;
    Timer timer;
    BufferedSet<std::function<void(const Camera &)> *> draws;
    BufferedSet<std::function<void()> *> updates;
    BufferedSet<Rigidbody *> rigidbodies;

    World(Window &window, int draw_priority)
        : WorldObject(*this) // Worldにのみ許されたプライベートコンストラクタ
        , draw_priority_(draw_priority)
        , window(window) {}

    ~World() override {
        info("Worldのデストラクタ");
        this->children_.clear(); // drawsやupdatesが消える前にUpdate等のデストラクタを呼ぶ
    }

    World(const World &) = delete;
    World &operator=(const World &) const = delete;
    World(World &&) = delete;
    World &operator=(World &&) const = delete;

    void master_update() {
        this->updates.foreach_flush([&](const auto update) {
            (*update)();
        });

        this->timer.step(); // タイマーを進める
    }

    void master_physics();

    void master_draw() {
        if (this->active_camera_ == nullptr) {
            print("警告: アクティブなカメラが存在しません");
        }
        this->draws.foreach_flush([&](const auto *draw) {
            (*draw)(*this->active_camera_);
        });

        // メッシュを描画
        this->mesh_draw_manager_.step();
        this->mesh_draw_manager_.draw_all_registered_objects(*active_camera());
    }

    // void register_to_draw(const MeshObject &obj) {
    //     this->mesh_draw_manager_.register_to_draw(obj);
    // }

    Camera *&active_camera() {
        return this->active_camera_;
    }

    [[nodiscard]] int get_draw_priority() const {
        return this->draw_priority_;
    }
    void set_draw_priority(int priority) {
        this->draw_priority_ = priority;
        this->window.refresh_world_order();
    }
    PropertyGetSet<&World::get_draw_priority, &World::set_draw_priority> draw_priority{this};
};
