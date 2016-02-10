#pragma once

#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <gl\glew.h>

struct vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct mesh
{
    mesh(std::vector<vertex> vertices, std::vector<GLuint> indices)
        : vertices_(vertices)
        , indices_(indices)
    {
    }

    void draw()
    {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void load()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex), vertices_.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), indices_.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, position));
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));

        glBindVertexArray(0);
    }

private:
    GLuint vao, vbo, ebo;

    std::vector<vertex> vertices_;
    std::vector<GLuint> indices_;
};
