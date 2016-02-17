#pragma once

#include "../Common/scene.h"
#include "../Common/camera.h"

#include <memory>
#include <QOpenGLWidget.h>
#include <QOpenGLShaderProgram.h>


struct point_light 
{
    GLfloat a, b, c, d; //Lissajous parameters
    GLfloat x_r, y_r, z_r; //Lissajous radius
    GLfloat mid_x, mid_y, mid_z; //Lissajous center
    //glm::vec3 position;
    glm::vec3 color;
};

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

public slots:
    void set_draw_spheres(bool);
    void set_light_count(int);
    void set_normal_mapping(bool);
    void set_directional_lighting(bool);

private:
    std::unique_ptr<Model> scene_;
    std::unique_ptr<Model> sphere_;
    std::unique_ptr<Model> plane_;


    QOpenGLShaderProgram buffer_shader_;
    QOpenGLShaderProgram direct_lighting_shader_;
    QOpenGLShaderProgram point_lighting_shader_;
    QOpenGLShaderProgram normal_shader_;

    GLuint buffer_;
    GLuint position_tex_, normal_tex_, albedo_tex_;
    GLuint normal_map_;

    struct dir_light
    {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    dir_light dir_light_;
    std::vector<point_light> point_lights_;

    Camera camera_;

    GLfloat t_;
    bool draw_spheres_;
    bool use_normal_mapping_;
    bool use_directional_lighting_;
    size_t lights_count_;

    QPoint last_mouse_pos;
    QPoint mouse_dif_;
    bool w_pressed;
    bool s_pressed;
    bool d_pressed;
    bool a_pressed;
    bool esc_pressed_;
    GLuint cubemap_;

    const size_t max_lights_ = 100;
};