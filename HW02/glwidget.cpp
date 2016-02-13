#include "glwidget.h"
#include "cubemap.h"

#include <algorithm>
#include <ctime>
#include <climits>
#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <glm\gtc\type_ptr.hpp>

glwidget::glwidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , cubemap_shader_(this)
    , camera_(glm::vec3(0.f, 0.f, 3.f))
    , skybox_(this)
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
    cubemap_shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, "cubemap.vs");
    cubemap_shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, "cubemap.frag");
    cubemap_shader_.link();
    std::string t = cubemap_shader_.log().toStdString();

    skybox_.load();

    std::vector<std::string> skybox_paths = {
        "skybox/ThickCloudsWaterRight2048.png",
        "skybox/ThickCloudsWaterLeft2048.png",
        "skybox/ThickCloudsWaterUp2048.png",
        "skybox/ThickCloudsWaterDown2048.png",
        "skybox/ThickCloudsWaterBack2048.png",
        "skybox/ThickCloudsWaterFront2048.png"
    };

    cubemap_ = load_cubemap(skybox_paths);

    model_ = std::make_unique<Model>("bunny_with_normals.obj");
}

void glwidget::paintGL()
{
    glClearColor(0.5, 0.5, 0.5, 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (!cubemap_shader_.bind()) exit(1);
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
    model = glm::scale(model, glm::vec3(10.f, 10.f, 10.f));

    glm::mat4 view = camera_.GetViewMatrix();
    glm::mat4 proj = glm::perspective(camera_.Zoom, float(width()) / float(height()), 0.1f, 100.f);
    glUniformMatrix4fv(cubemap_shader_.uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(cubemap_shader_.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(cubemap_shader_.uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniform3f(cubemap_shader_.uniformLocation("cameraPos"),
        camera_.Position.x, camera_.Position.y, camera_.Position.z);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_);
    model_->Draw(cubemap_shader_);

    skybox_.draw(cubemap_, view, proj);
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

void glwidget::mouseReleaseEvent(QMouseEvent* e)
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