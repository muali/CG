#pragma once

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
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;

public slots:
    void set_range(int slider_value);
    void set_max_iter(int slider_value);

private:
    static const int texture_size = 256;

    QPoint last_mouse_pos_;
    std::pair<float, float> bot_left_;
    float zoom_;
    float range_;
    int max_iter_;

    QOpenGLShaderProgram shader_program_;
    QImage texture_;
};