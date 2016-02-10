#pragma once

#include "scene.h"
#include "camera.h"
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
    bool esc_pressed();

private:
    scene scene_;
    QOpenGLShaderProgram cubemap_shader_;
    Camera camera_;

    QPoint last_mouse_pos;
    QPoint mouse_dif_;
    bool w_pressed;
    bool s_pressed;
    bool d_pressed;
    bool a_pressed;
    bool esc_pressed_;
};