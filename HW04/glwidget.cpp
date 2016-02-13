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
// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
} //unnamed

glwidget::glwidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , buffer_shader_(this)
    , direct_lighting_shader_(this)
    , point_lighting_shader_(this)
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

    buffer_shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/buffer_shader.vs");
    buffer_shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/buffer_shader.frag");
    buffer_shader_.link();
    
    glGenFramebuffers(1, &buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer_);

    glGenTextures(1, &position_tex_);
    glBindTexture(GL_TEXTURE_2D, position_tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width(), height(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // - Normal color buffer
    glGenTextures(1, &normal_tex_);
    glBindTexture(GL_TEXTURE_2D, normal_tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width(), height(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // - Color + Specular color buffer
    glGenTextures(1, &albedo_tex_);
    glBindTexture(GL_TEXTURE_2D, albedo_tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // - Create and attach depth buffer (renderbuffer)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width(), height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_tex_, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_tex_, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo_tex_, 0);
    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());


    direct_lighting_shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/direct_lighting.vs");
    direct_lighting_shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/direct_lighting.frag");
    direct_lighting_shader_.link();

    direct_lighting_shader_.bind();

    direct_lighting_shader_.setUniformValue("light.direction", 1.f, -3.f, -8.f);
    direct_lighting_shader_.setUniformValue("light.ambient", 0.05f, 0.05f, 0.05f);
    direct_lighting_shader_.setUniformValue("light.diffuse", 0.4f, 0.4f, 0.4f);
    direct_lighting_shader_.setUniformValue("light.specular", 0.5f, 0.5f, 0.5f);
    direct_lighting_shader_.setUniformValue("gPosition", 0);
    direct_lighting_shader_.setUniformValue("gNormal", 1);
    direct_lighting_shader_.setUniformValue("gAlbedoSpec", 2);

    point_lighting_shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/point_light.vs");
    point_lighting_shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/point_light.frag");

    point_lighting_shader_.bind();
    point_lighting_shader_.setUniformValue("gPosition", 0);
    point_lighting_shader_.setUniformValue("gNormal", 1);
    point_lighting_shader_.setUniformValue("gAlbedoSpec", 2);

    scene_ = std::make_unique<Model>("resources/house.obj");
    sphere_ = std::make_unique<Model>("resources/sphere.obj");

    point_lights_.push_back(point_light{ glm::vec3(-44.f, 17.f, 41.f), glm::vec3(1.f, 0.f, 0.f) });
    point_lights_.push_back(point_light{ glm::vec3(-35.f, 16.5f, 23.7f), glm::vec3(0.f, 1.f, 0.f) });
    point_lights_.push_back(point_light{ glm::vec3(-30.f, 16.5f, 23.7f), glm::vec3(0.f, 0.f, 1.f) });
    point_lights_.push_back(point_light{ glm::vec3(-35.f, 16.5f, 40.f), glm::vec3(0.f, 1.f, 1.f) });
    point_lights_.push_back(point_light{ glm::vec3(-40.f, 16.5f, 20.f), glm::vec3(1.f, 1.f, 1.f) });
}

void glwidget::paintGL()
{
    glClearColor(0., 0., 0., 0.);
    // 1. Geometry Pass: render scene's geometry/color data into gbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, buffer_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projection = glm::perspective(camera_.Zoom, (float)width() / height(), 0.1f, 100.0f);
    glm::mat4 view = camera_.GetViewMatrix();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
    model = glm::scale(model, glm::vec3(10.f, 10.f, 10.f));
    buffer_shader_.bind();
    glUniformMatrix4fv(buffer_shader_.uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(buffer_shader_.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(buffer_shader_.uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    scene_->Draw(buffer_shader_);
   
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    // 2. Lighting Pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
    glClear(GL_COLOR_BUFFER_BIT);

    glDepthMask(false);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    direct_lighting_shader_.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, position_tex_);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal_tex_);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, albedo_tex_);
    // Also send light relevant uniforms
    direct_lighting_shader_.setUniformValue("viewPos", camera_.Position.x, camera_.Position.y, camera_.Position.z);
    // Finally render quad
    RenderQuad();

    point_lighting_shader_.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, position_tex_);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal_tex_);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, albedo_tex_);
    for (auto& light : point_lights_)
    {
        GLfloat constant = 1.0;
        GLfloat linear = 0.22;
        GLfloat quadratic = 0.20;
        GLfloat lightMax = std::fmaxf(std::fmaxf(light.color.r, light.color.g), light.color.b);
        GLfloat radius =
            (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
            / (2 * quadratic);

        model = glm::translate(glm::mat4(), light.position);
        model = glm::scale(model, glm::vec3(radius));
        glm::mat4 transform = projection * view * model;
        
        glUniformMatrix4fv(point_lighting_shader_.uniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(transform));
        point_lighting_shader_.setUniformValue("light.Position", light.position.x, light.position.y, light.position.z);
        point_lighting_shader_.setUniformValue("light.Color", light.color.r, light.color.g, light.color.b);
        point_lighting_shader_.setUniformValue("light.Linear", linear);
        point_lighting_shader_.setUniformValue("light.Quadratic", quadratic);
        point_lighting_shader_.setUniformValue("light.Radius", radius);
        point_lighting_shader_.setUniformValue("viewPos", camera_.Position.x, camera_.Position.y, camera_.Position.z);
        point_lighting_shader_.setUniformValue("screen", (float)width(), (float)height());

        sphere_->Draw(point_lighting_shader_);
    }

    glDepthMask(true);
    glDisable(GL_BLEND);

    std::string t = point_lighting_shader_.log().toStdString();
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