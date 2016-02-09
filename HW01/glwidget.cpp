#include "glwidget.h"

#include <algorithm>
#include <ctime>
#include <climits>
#include <QMouseEvent>
#include <QWheelEvent>

glwidget::glwidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , shader_program_(this)
    , bot_left_(-2.f, -2.f)
    , zoom_(0.01f)
    , range_(4)
    , max_iter_(50)
{
    setMouseTracking(false);
    texture_ = QImage("pal.ppm", "PPM");
}

void glwidget::initializeGL()
{
    shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, "mandelbrot.frag");
    shader_program_.log().toStdString();
    shader_program_.link();
    shader_program_.bind();

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage1D(GL_TEXTURE_1D, 0, 4, texture_.width(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texture_.bits());
}

void glwidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader_program_.setUniformValue("coord", bot_left_.first, bot_left_.second);
    shader_program_.setUniformValue("zoom", zoom_);
    shader_program_.setUniformValue("range", range_);
    shader_program_.setUniformValue("max_step", max_iter_);

    glRectf(-1, -1, 1, 1);
}

void glwidget::set_range(int slider_value)
{
    range_ = 8. * slider_value / 100;
    update();
}

void glwidget::set_max_iter(int slider_value)
{
    max_iter_ = slider_value;
    update();
}

void glwidget::mousePressEvent(QMouseEvent* e)
{
    last_mouse_pos_ = e->pos();
}

void glwidget::mouseMoveEvent(QMouseEvent* e)
{
    QPoint cur_pos = e->pos();
    QPoint delta_pos = cur_pos - last_mouse_pos_;
    bot_left_.first -= delta_pos.x() * zoom_;
    bot_left_.second += delta_pos.y() * zoom_;
    last_mouse_pos_ = cur_pos;
    update();
}

void glwidget::wheelEvent(QWheelEvent* e)
{
    float xgl = e->x();
    float ygl = height() - e->y();
    float x = bot_left_.first + xgl * zoom_;
    float y = bot_left_.second + ygl * zoom_;
    float dy_angle = e->angleDelta().y();

    if (dy_angle > 0)
        zoom_ *= 1.25;
    else
        zoom_ = std::max(1e-9, zoom_ * 0.75);

    bot_left_.first = x - xgl * zoom_;
    bot_left_.second = y - ygl * zoom_;
    update();
}