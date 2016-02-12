#include "glwidget.h"

#include <algorithm>
#include <ctime>
#include <climits>
#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <glm\gtc\type_ptr.hpp>


namespace
{

std::vector<glm::vec3> camera_vertices()
{
    float pf = 100.0f;
    float pn = 0.1f;
    glm::vec3 ftl = glm::vec3(-1, +1, pf); //far top left
    glm::vec3 fbr = glm::vec3(+1, -1, pf); //far bottom right
    glm::vec3 fbl = glm::vec3(-1, -1, pf); //far bottom left
    glm::vec3 ftr = glm::vec3(+1, +1, pf); //far top right
    glm::vec3 ntl = glm::vec3(-1, +1, pn); //near top left
    glm::vec3 nbr = glm::vec3(+1, -1, pn); //near bottom right
    glm::vec3 nbl = glm::vec3(-1, -1, pn); //near bottom left
    glm::vec3 ntr = glm::vec3(+1, +1, pn); //near top right

    return std::vector<glm::vec3> {
        // near
        ntl, nbl, ntr, // 1 triangle
        ntr, nbl, nbr,
        // right
        nbr, ftr, ntr,
        ftr, nbr, fbr,
        // left
        nbl, ftl, ntl,
        ftl, nbl, fbl,
        // far
        ftl, fbl, fbr,
        fbr, ftr, ftl,
        //bottom
        nbl, fbr, fbl,
        fbr, nbl, nbr,
        //top
        ntl, ftr, ftl,
        ftr, ntl, ntr
    };
}

} //unnamed


glwidget::glwidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , shader_(this)
    , camera_shader_(this)
    , buffer_shader_(this)
    , camera_(glm::vec3(-11.f, 30.f, 80.f))
    , w_pressed(false)
    , s_pressed(false)
    , a_pressed(false)
    , d_pressed(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(false);
}

void glwidget::initializeGL()
{
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/shader.vs");
    shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/shader.frag");
    shader_.link();
    std::string t = shader_.log().toStdString();

    model_ = std::make_unique<Model>("resource/house.obj");

    cam_vertices_ = camera_vertices();

}

void glwidget::paintGL()
{
    glClearColor(0.5, 0.5, 0.5, 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (!shader_.bind()) exit(1);
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
    model = glm::scale(model, glm::vec3(10.f, 10.f, 10.f));

    glm::mat4 view = camera_.GetViewMatrix();
    glm::mat4 proj = glm::perspective(camera_.Zoom, float(width()) / float(height()), 0.1f, 100.f);
    glUniformMatrix4fv(shader_.uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader_.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader_.uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniform3f(shader_.uniformLocation("cameraPos"),
        camera_.Position.x, camera_.Position.y, camera_.Position.z);

    model_->Draw();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    camera_shader_.bind();
    glUniformMatrix4fv(camera_shader_.uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(camera_shader_.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    model = glm::inverse(projectorProjection * projectorCamera.GetViewMatrix());
    glUniformMatrix4fv(camera_shader_.uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(camVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


}

void glwidget::do_turn(float dt)
{
    if (w_pressed) camera_.ProcessKeyboard(FORWARD, dt);
    if (s_pressed) camera_.ProcessKeyboard(BACKWARD, dt);
    if (d_pressed) camera_.ProcessKeyboard(RIGHT, dt);
    if (a_pressed) camera_.ProcessKeyboard(LEFT, dt);
    camera_.ProcessMouseMovement(mouse_dif_.x(), mouse_dif_.y());
    mouse_dif_ = QPoint();
    update();
}

void glwidget::keyPressEvent(QKeyEvent* e)
{
    if (e->text() == "w") w_pressed = true;
    if (e->text() == "s") s_pressed = true;
    if (e->text() == "a") a_pressed = true;
    if (e->text() == "d") d_pressed = true;
}

void glwidget::keyReleaseEvent(QKeyEvent* e)
{
    if (e->text() == "w") w_pressed = false;
    if (e->text() == "s") s_pressed = false;
    if (e->text() == "a") a_pressed = false;
    if (e->text() == "d") d_pressed = false;
}

void glwidget::mousePressEvent(QMouseEvent* e)
{
    last_mouse_pos = e->pos();
    setCursor(QCursor(Qt::BlankCursor));
}

void glwidget::mouseReleaseEvent(QMouseEvent* /*e*/)
{
    setCursor(QCursor(Qt::ArrowCursor));
}

void glwidget::mouseMoveEvent(QMouseEvent* e)
{
    QPoint cur_pos = e->pos();
    QPoint delta_pos = cur_pos - last_mouse_pos;
    delta_pos.setY(-delta_pos.y());
    mouse_dif_ += delta_pos;
    QCursor::setPos(mapToGlobal(last_mouse_pos));
}