#pragma once

#include <cppgui.hpp>

template <size_t N>
class CorePolygon : Draw {
    // GLuint vbo_;
    Window &window_;
    GLuint tex_id_ = 0;
    array<Point<GLfloat>, N> vertices_;

  public:
    array<RGBA, N> colors_;

    CorePolygon(Window &window, array<Point<float>, N> points, GLuint tex_id = 0, GLenum usage = GL_DYNAMIC_DRAW)
        : window_(window)
        , vertices_(points)
        , tex_id_(tex_id) {
        cout << "こんです" << endl;
        // 頂点バッファオブジェクト（VBO）の生成とデータの転送
        // glGenBuffers(1, &vbo_);
        // glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices_.size() * 2, vertices_.data(), usage); // WARNING: vertices_のアライメントによっては動作しない
    }

    void draw() const override {
        // 何番目のattribute変数か
        int positionLocation = glGetAttribLocation(window_.program_id_, "position");
        int uvLocation = glGetAttribLocation(window_.program_id_, "uv");
        int colorLocation = glGetAttribLocation(window_.program_id_, "color");

        int textureLocation = glGetUniformLocation(window_.program_id_, "texture");
        int is_texLocation = glGetUniformLocation(window_.program_id_, "is_tex");

        // テクスチャを持つ場合
        if (tex_id_ != 0) {
            // attribute属性を有効にする
            glEnableVertexAttribArray(positionLocation);
            glEnableVertexAttribArray(uvLocation);
            glEnableVertexAttribArray(colorLocation);

            // uniform属性を設定する
            glUniform1i(textureLocation, 0);
            glUniform1i(is_texLocation, int(true));

            const GLfloat vertex_uv[] = {1, 0, 0, 0, 0, 1, 1, 1};

            // attribute属性を登録
            glVertexAttribPointer(positionLocation, 2, GL_FLOAT, false, 0, vertices_.data());
            glVertexAttribPointer(uvLocation, 2, GL_FLOAT, false, 0, vertex_uv);
            glVertexAttribPointer(colorLocation, 4, GL_FLOAT, false, 0, colors_.data());

            // モデルの描画
            glBindTexture(GL_TEXTURE_2D, tex_id_);
            glDrawArrays(GL_TRIANGLE_FAN, 0, N);
            glBindTexture(GL_TEXTURE_2D, 0);
        } else {
            // attribute属性を有効にする
            glEnableVertexAttribArray(positionLocation);
            glEnableVertexAttribArray(colorLocation);

            // attribute属性を登録
            glVertexAttribPointer(positionLocation, 2, GL_FLOAT, false, 0, vertices_.data());
            glVertexAttribPointer(colorLocation, 4, GL_FLOAT, false, 0, colors_.data());

            // モデルの描画
            glUniform1i(is_texLocation, int(false));
            glDrawArrays(GL_TRIANGLE_FAN, 0, N);

            // // glDisableVertexAttribArray(0); // TODO: 不要？
        }
    }
};