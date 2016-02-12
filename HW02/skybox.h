#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <QOpenGLShaderProgram>


struct skybox
{
    skybox(QObject* ctx = nullptr)
        : shader_(ctx)
    {
        vertices = {
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
        };
    }

    void load();

    void draw(GLuint cubemap, glm::mat4 const& view, glm::mat4 const& proj)
    {
        glDepthFunc(GL_LEQUAL);
        if (!shader_.bind()) exit(0);

        glm::mat4 view2 = glm::mat4(glm::mat3(view));

        glUniformMatrix4fv(shader_.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view2));
        glUniformMatrix4fv(shader_.uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(proj));
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        glDrawArrays(GL_TRIANGLES, 0, vertices_cnt);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        std::string t = shader_.log().toStdString();
    }

private:
    GLuint vao, vbo;

    QOpenGLShaderProgram shader_;

    const size_t vertices_cnt = 36;
    std::vector<GLfloat> vertices;
};

inline void skybox::load()
{
    shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, "skybox.vs");
    shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, "skybox.frag");
    shader_.link();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glBindVertexArray(0);
}