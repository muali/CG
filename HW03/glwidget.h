#pragma once

#include "../Common/scene.h"
#include "../Common/camera.h"

#include <memory>
#include <QOpenGLWidget.h>
#include <QOpenGLShaderProgram.h>


struct glwidget : QOpenGLWidget
{
    Q_OBJECT;

public:

    glwidget(QWidget* parent = nullptr);

    void initializeGL() override;
    //   void resizeGL    (int width, int height)  override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent * e) override;
    void keyReleaseEvent(QKeyEvent * e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void do_turn(float dt);

public slots:
    void nearest_toogled(bool);
    void linear_toogled(bool);
    void mipmap_toogled(bool);
    void static_toogled(bool);
    void dynamic_toogled(bool);

private:

    std::unique_ptr<Model> model_;
    std::vector<glm::vec3> cam_vertices_;
    QOpenGLShaderProgram shader_;
    QOpenGLShaderProgram buffer_shader_;
    QOpenGLShaderProgram camera_shader_;

    GLuint cam_vao_;
    GLuint cam_vbo_;

    Camera camera_;
    Camera static_proj_;

    GLuint static_text_;
    GLuint frame_buffer_;
    GLuint dyn_tex_;

    bool use_static_;
    GLuint filter_;

    QPoint last_mouse_pos;
    QPoint mouse_dif_;
    bool w_pressed;
    bool s_pressed;
    bool d_pressed;
    bool a_pressed;
    GLfloat angle_;

    const float cam_near_ = 0.1f;
    const float cam_far_ = 1000.f;
    const GLuint dyn_tex_size_ = 1024;
};