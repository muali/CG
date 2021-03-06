#pragma once

#include "../Common/scene.h"
#include "../Common/camera.h"
#include "skybox.h"

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
    std::unique_ptr<Model> model_;
    QOpenGLShaderProgram cubemap_shader_;
    Camera camera_;
    skybox skybox_;

    QPoint last_mouse_pos;
    QPoint mouse_dif_;
    bool w_pressed;
    bool s_pressed;
    bool d_pressed;
    bool a_pressed;
    bool esc_pressed_;
    GLuint cubemap_;
};