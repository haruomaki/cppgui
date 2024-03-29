#pragma once

#include <graphical_base.hpp>

GLuint create_shader(GLenum shader_type, const string &source_code);

enum class StorageQualifier {
    Attribute,
    Uniform,
};

class ProgramObject {
    GLuint program_id_;

  public:
    ProgramObject() = delete; // glCreateProgram()をしないコンストラクタはナンセンス
    ProgramObject(std::initializer_list<GLuint> shader_ids);
    ~ProgramObject();
    ProgramObject(const ProgramObject &) = delete;
    ProgramObject &operator=(const ProgramObject &) const = delete;
    ProgramObject(ProgramObject &&shader) noexcept // TODO: ソースファイルに
        : program_id_(shader.program_id_) {
    }
    ProgramObject &operator=(ProgramObject &&shader) noexcept {
        this->program_id_ = shader.program_id_;
        return *this;
    }

    [[nodiscard]] GLuint get_program_id() const;
    void use() const;

    template <StorageQualifier q>
    [[nodiscard]] GLint get_location(const string &name) const;

    void set_attribute(const string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) const;

    void set_uniform(const string &name, GLint int_value) const;
    void set_uniform(const string &name, const glm::vec3 &vec3_value) const;
    void set_uniform(const string &name, const glm::vec4 &vec4_value) const;
    void set_uniform(const string &name, const glm::mat4 &mat4_value) const;
};

class VertexArrayObject {
    GLuint array_{};

  public:
    static inline VertexArrayObject gen() {
        VertexArrayObject vao;
        glGenVertexArrays(1, &vao.array_); // VAOの生成
        return vao;
    }

    template <typename F>
    inline void bind(const F &proc_in_bind) const {
        glBindVertexArray(array_); // VAOをバインド
        proc_in_bind();
        glBindVertexArray(0);
    }
};

template <GLenum target>
class BufferObject {
    GLuint buffer_{};

  public:
    // glGenBuffers() + glBufferData()
    static inline BufferObject gen(size_t size, const void *data, GLenum usage) {
        BufferObject xbo;
        glGenBuffers(1, &xbo.buffer_); // BOの生成
        xbo.bind([&] {
            glBufferData(target, GLsizeiptr(size), data, usage); // バインド中にデータを設定
        });
        return xbo;
    }

    // バッファ内容を更新する
    inline void subdata(GLintptr offset, size_t size, const void *data) {
        this->bind([&] {
            glBufferSubData(target, offset, GLsizeiptr(size), data);
        });
    }

    template <typename F>
    inline void bind(const F &proc_in_bind) const {
        glBindBuffer(target, buffer_);
        proc_in_bind();
        glBindBuffer(target, 0);
    }
};

using VertexBufferObject = BufferObject<GL_ARRAY_BUFFER>;
using ElementBufferObject = BufferObject<GL_ELEMENT_ARRAY_BUFFER>;
