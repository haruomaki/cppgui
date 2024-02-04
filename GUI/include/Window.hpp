#pragma once

#include "SizeCallback.hpp"
#include <graphical_base.hpp>

class SizeCallback;

// 一つのウィンドウを表すクラス
class Window {
    GLFWwindow *gwin_ = nullptr;
    std::set<std::unique_ptr<SizeCallback>> size_callbacks_;
    // std::set<std::function<void(int, int)> *> size_callbacks_;
    std::set<std::function<void(int, int)> *> key_callbacks_;
    std::set<std::function<void()> *> updates_;

    friend class GUI;
    friend class SizeCallback;
    friend class KeyCallback;
    friend class Update;

  public:
    Window(int width, int height, const char *title);
    // ~Window();

    // // コピーは禁止する
    // Window(const Window &) = default;
    // Window &operator=(const Window &) = default;

    // // ムーブコンストラクタはデフォルト、ムーブ代入は禁止
    // Window(Window &&) = default;
    // Window &operator=(Window &&) = default;

    [[nodiscard]] GLFWwindow *getGLFW() const;
    [[nodiscard]] pair<int, int> getWindowSize() const;
    [[nodiscard]] pair<int, int> getFrameBufferSize() const;
    [[nodiscard]] pair<float, float> getWindowContentScale() const;
    void close() const;

    // glfwGetKeyのラッパー。GLFW_PRESSのときtrue、GLFW_RELEASEのときfalse
    [[nodiscard]] bool getKey(int key) const;

    template <typename T>
    // TはSizeCallbackの派生クラス
    // Tはムーブコンストラクタを持つ
    T &registerSizeCallback(T &&size_callback) {
        print("はい");
        // size_callback.window_ = this;
        std::unique_ptr<SizeCallback> ptr(new T(size_callback)); // ヒープへムーブ
        (*ptr).window_ = this;
        // auto y = (ptr.get());
        auto [it, inserted] = this->size_callbacks_.insert(std::move(ptr));
        if (!inserted) {
            std::runtime_error("registerSizeCallbackに失敗");
        }
        auto ptr2 = static_cast<T *>(it->get());
        debug(ptr2);
        return *ptr2;
    }
};
