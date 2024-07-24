#pragma once
#include <vector>

class Geo {
public:
  GLuint vertexCount;
  GLuint vertexArrayId;
  GLuint vertexBuffer;
  GLuint normalBuffer;
  GLuint elementBuffer;
  glm::vec2 size = glm::vec2(0, 0);

  void bindBuffers(const std::vector<GLfloat>& vertices,
                   const std::vector<GLint>& elements,
                   const std::vector<GLfloat>& normals)
  {
    vertexCount = elements.size();

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLint), elements.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
  }

  ~Geo()
  {
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &elementBuffer);
    glDeleteVertexArrays(1, &vertexArrayId);
  }
};
