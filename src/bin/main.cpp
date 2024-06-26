#include <master.hpp>
#include <utility.hpp>

int main() {
    GUI gui;
    Window &window = gui.create_window(600, 500, "めいん");
    World &main_world = window.create_world();

    // バーテックスシェーダのコンパイル
    auto vsh_string = load_string("assets/shaders/default.vsh");
    auto vsh_id = create_shader(GL_VERTEX_SHADER, vsh_string);

    // フラグメントシェーダのコンパイル
    auto fsh_string = load_string("assets/shaders/default.fsh");
    auto fsh_id = create_shader(GL_FRAGMENT_SHADER, fsh_string);

    auto main_shader = ProgramObject{vsh_id, fsh_id};

    auto &camera = main_world.append_child<MobileNormalCamera>();
    camera.position = {0, 0, 1000};
    camera.rotate = ANGLE_Y(M_PIf);
    camera.scale = 2000; // 速度調整
    camera.set_active();

    Font migmix_font;
    auto &sample_text = main_world.append_child<Text>(migmix_font, "This is sample text 123456789", RGBA{0.5, 0.8, 0.2, 0.4});
    auto &credit_text = main_world.append_child<Text>(migmix_font, "(C) LearnOpenGL.com", RGBA{0.3, 0.7, 0.9, 0.4});

    sample_text.set_position({-200, 50, 200});
    credit_text.set_position({200, 400, 1});

    GLuint cat_texture = loadTexture("assets/images/cat.raw");
    Material &cat_material = MaterialBuilder().texture(cat_texture).build(window);

    auto &gon = new_mesh(window, GL_TRIANGLE_FAN, {{0.9f, 0.9f, 0.4}, {0.5f, 0.f, 0}, {0.f, 0.f, 0.2}, {0.f, 0.5f, 0}},
                         {
                             {0.9, 0.3, 0, 1},
                             {0.1, 0.2, 0.7, 0.3},
                             {0.9, 0.2, 0.7, 0.3},
                             {0.3, 0.7, 0.5f, 0.5},
                         },
                         {{1, 0}, {0, 0}, {0, 1}, {1, 1}});

    auto &gon2 = new_mesh(window, GL_TRIANGLE_FAN, {{-0.8, -0.3, -0.1}, {-0.2, 0.7, 0}, {0.5, -0.5, 0}},
                          {
                              {0.9, 0.3, 0, 1},
                              {0.9, 0.2, 0.7, 0.3},
                              {0.3, 0.7, 0.5f, 0.5},
                          });

    auto &gon3 = new_mesh(window, GL_TRIANGLE_FAN, {{0.4, 0.3, 0}, {0.5, 0.3, 0}, {0.5, 0.4, 0}, {0.45, 0.35, 0}, {0.4, 0.4, 0}},
                          {
                              {0.9, 0.3, 0, 1},
                              {0.9, 0.2, 0.7, 0.3},
                              {0.9, 0.2, 0.7, 0.3},
                              {0.9, 0.2, 0.7, 0.3},
                              {0.3, 0.7, 0.5f, 0.5},
                          });

    auto &poly = new_mesh(window, GL_TRIANGLE_FAN, {{-50, -50, 0}, {-50, 50, 0}, {50, 50, 0}, {50, -50, 0}}, {{0.3, 0.7, 0.1, 0.5}});

    auto &ins3 = main_world.append_child<MeshObject>(gon3);
    /* auto &ins = */ ins3.append_child<MeshObject>(gon, &cat_material);
    auto &ins2 = main_world.append_child<MeshObject>(gon2);
    auto &inspoly = main_world.append_child<MeshObject>(poly);
    ins3.scale = 1000;
    ins2.scale = 1500;
    inspoly.set_position({0, 100, 100});

    for (int i = -500; i <= 500; i += 100) {
        auto &instance = main_world.append_child<MeshObject>(poly);
        instance.set_position({-200, -50, i});
    }
    for (int i = -300; i <= 300; i += 100) {
        auto &instance = main_world.append_child<MeshObject>(poly);
        instance.set_position({-0, 300, i});
    }

    // main_world.timer.task(1, [&] {
    //     debug(ins3.get_absolute_transform());
    // });

    const vector<glm::vec3> vertices = {
        glm::vec3(0, -900, 500),
        glm::vec3(500, 0, 0),
        glm::vec3(0, 500, 0),
        glm::vec3(-500, 0, 0),
    };
    const vector<RGBA> colors = {
        {0.1, 0.4, 0.7, 0.5},
        {0.1, 0.4, 0.7, 1},
        {0.1, 0.4, 0.7, 0.5},
        {0.1, 0.4, 0.7, 1},
    };

    auto &cube = new_mesh(main_world);
    auto &mesh = cube.mesh;
    // mesh.vertices.xs = {1, 0, 0, 8};
    // mesh.vertices.ys = {1, 2, 3, -1};
    mesh.indices = {0, 1, 2, 3, 7, 1, 5, 0, 4, 2, 6, 7, 4, 5};
    mesh.vertices.coords = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}};
    // mesh.vertices.coords = {{0, 0, 0}, {1, 0, 0}, {0, 0, 1}, {1, 0, 1}, {0, 1, 0}, {1, 1, 0}, {0, 1, 1}, {1, 1, 1}};
    mesh.vertices.colors = std::vector(8, RGBA{0.46, 0.85, 0.84, 0.9});
    mesh.draw_mode = GL_LINE_STRIP;
    mesh.draw_mode = GL_TRIANGLE_STRIP;
    cube.material.line_width = 10;
    cube.scale = 200;
    cube.position = {100, 100, 500};

    auto &grid = new_line(main_world);
    // grid.indices = {0, 1, 2, 3, 4, 5};
    for (int i = -10; i <= 10; i++) {
        constexpr RGBA grid_color = {0.1, 0.1, 0.1, 1};
        grid.mesh.vertices.push_back(InterleavedVertexInfo{{i, 0, -10}, grid_color});
        grid.mesh.vertices.push_back(InterleavedVertexInfo{{i, 0, 10}, grid_color});
        grid.mesh.vertices.push_back(InterleavedVertexInfo{{-10, 0, i}, grid_color});
        grid.mesh.vertices.push_back(InterleavedVertexInfo{{10, 0, i}, grid_color});
    }
    grid.mesh.draw_mode = GL_LINES;
    grid.material.line_width = 6;
    grid.scale = 100;

    // レンダリングループ
    gui.mainloop();
}
