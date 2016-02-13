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
	void paintGL() override;
    void keyPressEvent(QKeyEvent * e) override;
    void keyReleaseEvent(QKeyEvent * e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void do_turn(float dt);

private:
    std::unique_ptr<Model> scene_;
    std::unique_ptr<Model> sphere_;

    QOpenGLShaderProgram buffer_shader_;
    QOpenGLShaderProgram direct_lighting_shader_;
    QOpenGLShaderProgram point_lighting_shader_;

    GLuint buffer_;
    GLuint position_tex_, normal_tex_, albedo_tex_;

    struct dir_light
    {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct point_light 
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    dir_light dir_light_;
    std::vector<point_light> point_lights_;

    Camera camera_;

    QPoint last_mouse_pos;
    QPoint mouse_dif_;
    bool w_pressed;
    bool s_pressed;
    bool d_pressed;
    bool a_pressed;
    bool esc_pressed_;
    GLuint cubemap_;
};