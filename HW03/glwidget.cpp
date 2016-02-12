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

std::vector<glm::vec3> camera_vertices(float c_near, float c_far)
{
    c_far *= 100;
	glm::vec3 ftl = glm::vec3(-1, +1, c_far); //far top left
	glm::vec3 fbr = glm::vec3(+1, -1, c_far); //far bottom right
	glm::vec3 fbl = glm::vec3(-1, -1, c_far); //far bottom left
	glm::vec3 ftr = glm::vec3(+1, +1, c_far); //far top right
	glm::vec3 ntl = glm::vec3(-1, +1, c_near); //near top left
	glm::vec3 nbr = glm::vec3(+1, -1, c_near); //near bottom right
	glm::vec3 nbl = glm::vec3(-1, -1, c_near); //near bottom left
	glm::vec3 ntr = glm::vec3(+1, +1, c_near); //near top right

	return vector<glm::vec3> {
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
    , static_proj_(glm::vec3(1.f, 19.f, 45.f), glm::vec3(0.f, 1.f, 0.f), -124, -18)
    , w_pressed(false)
    , s_pressed(false)
    , a_pressed(false)
    , d_pressed(false)
{
 //   static_proj_.Front = glm::vec3(-0.238461, -0.304864, -0.922060);
	//static_proj_.Up = glm::vec3(-0.076332, 0.952396, -0.295154);
	//static_proj_.Right = glm::vec3(0.968148, 0.000000, -0.250380);
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
    
    camera_shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/camera.vs");
    camera_shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/camera.frag");
    std::string t = camera_shader_.log().toStdString();

    static_text_ = TextureFromFile("smile.png", "resource");

    model_ = std::make_unique<Model>("resource/house.obj");

    cam_vertices_ = camera_vertices(cam_near_, cam_far_);
    
    glGenVertexArrays(1, &cam_vao_);
    glGenBuffers(1, &cam_vbo_);
    glBindVertexArray(cam_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, cam_vbo_);
    glBufferData(GL_ARRAY_BUFFER, cam_vertices_.size() * 3 * sizeof(GLfloat), cam_vertices_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
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
    glm::mat4 proj = glm::perspective(camera_.Zoom, float(width()) / float(height()), cam_near_, cam_far_);
    glm::mat4 static_view = static_proj_.GetViewMatrix();
    glm::mat4 static_proj = glm::perspective(static_proj_.Zoom, 0.5f, cam_near_, cam_far_);

    glm::mat4 transform = proj * view * model;
    glm::mat4 proj_transform = static_proj * static_view * model;

    glUniformMatrix4fv(shader_.uniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniformMatrix4fv(shader_.uniformLocation("proj_transform"), 1, GL_FALSE, glm::value_ptr(proj_transform));

    glUniform1i(shader_.uniformLocation("projectiveTexture"), 10);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, static_text_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    model_->Draw(shader_);                                       
    draw_cameras(view, proj, model);
}

void glwidget::draw_cameras(glm::mat4 const &view, glm::mat4 const &proj, glm::mat4 const &model)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    camera_shader_.bind();
    
    glm::mat4 static_view = static_proj_.GetViewMatrix();
    glm::mat4 static_proj = glm::perspective(static_proj_.Zoom, 1.f, cam_near_, cam_far_);

    glUniformMatrix4fv(camera_shader_.uniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(proj * view));
    glUniformMatrix4fv(camera_shader_.uniformLocation("inv_proj_transform"), 1, GL_FALSE, glm::value_ptr(glm::inverse(static_proj * static_view)));

    glBindVertexArray(cam_vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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