#pragma once

#include <Drawable.hpp>
#include <Shader.hpp>
#include <Update.hpp>
#include <core.hpp>

class DynamicArray : public DrawableWorldObject, Update {
    VertexArrayObject vao_;
    VertexBufferObject vbo_;
    const ProgramObject &shader_;
    size_t n_;
    size_t capacity_;
    static constexpr RGBA default_color{0.8, 0.8, 0.8, 1};

    void update() override {
        // VBOの更新
        vao_.bind([&] {
            if (capacity_ != vertices.capacity()) {
                // 空のVBOを生成
                vbo_ = VertexBufferObject::gen(sizeof(InterleavedVertexInfo) * vertices.capacity(), nullptr, GL_DYNAMIC_DRAW);

                // VAOを作成。頂点の座標と色を関連付ける
                vao_ = VertexArrayObject::gen();
                vao_.bind([&] {
                    vbo_.bind([&] {
                        shader_.setAttribute("position", 3, GL_FLOAT, GL_FALSE, sizeof(InterleavedVertexInfo), nullptr);                                  // 位置
                        shader_.setAttribute("color", 4, GL_FLOAT, GL_FALSE, sizeof(InterleavedVertexInfo), reinterpret_cast<void *>(sizeof(float) * 3)); // 色 offset=12
                    });
                    getErrors();
                });
            }
            capacity_ = vertices.capacity();
            n_ = vertices.size();
            vbo_.bind([&] {
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InterleavedVertexInfo) * capacity_, vertices.data());
            });
        });
    }

    void draw(const Camera &camera) const override {
        // シェーダを有効化
        const auto &shader = shader_;
        shader.use();

        // 点の大きさ・線の太さを設定
        switch (draw_mode) {
        case GL_POINTS:
            glPointSize(point_size);
            break;
        case GL_LINE_STRIP:
        case GL_LINE_LOOP:
            glLineWidth(line_width);
            break;
        }

        // ワールド座標変換
        glm::mat4 model_matrix = this->getAbsoluteTransform();

        // ビュー座標変換
        const glm::mat4 &view_matrix = camera.getViewMatrix();

        // 合成して、モデルビュー行列を得る
        glm::mat4 model_view_matrix = view_matrix * model_matrix;
        shader.setUniform("modelViewMatrix", model_view_matrix);

        // 射影変換行列
        const auto projection_matrix = camera.getProjectionMatrix();
        shader.setUniform("projectionMatrix", projection_matrix);

        // モデルの描画
        shader.setUniform("is_tex", GL_FALSE);
        vao_.bind([&] {
            glDrawArrays(draw_mode, 0, GLsizei(n_));
        });
    }

  public:
    InterleavedVertexInfoVector vertices;
    GLenum draw_mode = GL_LINE_STRIP;
    int point_size = 4;
    int line_width = 4;

    DynamicArray(Window &window, World &world, const ProgramObject &shader, vector<glm::vec3> coords = {}, vector<RGBA> colors = {})
        : DrawableWorldObject(world)
        , Update(window)
        , shader_(shader)
        , n_(coords.size())
        , capacity_(coords.capacity()) {

        vector<InterleavedVertexInfo> vers = {};
        for (size_t i = 0; i < n_; i++) {
            glm::vec3 coord = coords[i];
            RGBA color = (i < colors.size() ? colors[i] : default_color); // 色情報がないときは白色に
            vers.push_back({coord, color});
        }
        vertices = vers;

        // debug(MemoryView(reinterpret_cast<float *>(vers.data()), sizeof(InterleavedVertexInfo) / sizeof(float) * n_));
    }
};
