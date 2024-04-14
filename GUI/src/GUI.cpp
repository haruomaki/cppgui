#include "GUI.hpp"
#include "Window.hpp"

Window &GUI::create_window(int width, int height, const std::string &title) {
    auto window = std::make_unique<Window>(*this, width, height, title.c_str());
    this->windows_.push_back(std::move(window));
    return *this->windows_.back();
}

void GUI::mainloop() {
    if (looping_) {
        throw std::runtime_error("すでにメインループが始まっています");
    }
    looping_ = true;

    // 描画のループ
    while (!this->windows_.empty()) {
        tick++;

        // auto vvv = this->windows_[0];
        // this->windows_.clear();
        // this->windows_.push_back(vvv);

        // 閉じるべきウィンドウを見つけてvectorから削除
        print("d1");
        auto remove_begin = std::remove_if(this->windows_.begin(), this->windows_.end(), [](const auto &window) {
            return glfwWindowShouldClose(window->gwin_) != 0;
        });
        this->windows_.erase(remove_begin, this->windows_.end());
        print("d2");

        // 生きている各ウィンドウに対して更新および描画
        for (const auto &window : this->windows_) {
            print("p1");
            window->update_routine();
            print("p2");
            window->draw_routine();
            print("p3");
        }

        // 受け取ったイベント（キーボードやマウス入力）を処理する
        glfwPollEvents();
    }

    looping_ = false;
}
