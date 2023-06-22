#include <master.hpp>

static float f(float x) {
    return 3 * sin(x);
}

int main() {
    constexpr int width = 600, height = 500;
    Window window(width, height);
    MaximumViewport viewport(window);

    World world;

    ProgramObject main_shader = {
        createShader(GL_VERTEX_SHADER, loadString("assets/shaders/shader.vsh")),
        createShader(GL_FRAGMENT_SHADER, loadString("assets/shaders/shader.fsh"))};

    OrthoCamera camera(viewport);
    // camera.setScale(0.01F);
    // camera.setScale(100);

    DynamicArray line(world, main_shader, {}, {}, GL_LINE_LOOP);
    line.setScale(100);

    for (auto &&x : linspace(-9, 9, 100)) {
        InterleavedVertexInfo2 ver;
        ver.coord_ = {x, f(x), 0};
        ver.color_ = {0.5, 0.2, 0.7, 1.0};
        line.vertices_.push_back(ver);
    }

    // 文字の表示
    Font migmix_font;
    Text sample_text(world, migmix_font, "This is sample text 123456789", {0.5, 0.8, 0.2, 0.4});
    Text credit_text(world, migmix_font, "(C) LearnOpenGL.com", {0.3, 0.7, 0.9, 0.4});

    // レンダリングループ
    window.mainloop([&] {
        for (auto &&ver : line.vertices_) {
            const float x = ver.coord_.x;
            const float y = f(x + float(window.tick_) / 100);
            ver.coord_ = {x, y, 0};
        }

        world.masterDraw(camera);
    });
}