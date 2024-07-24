#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "./Geo.hpp"

class RectGeo: public Geo {
public:
  glm::vec2 size;

  RectGeo (float _size = 1.) {
    size = glm::vec2(_size, _size);
    float w = size[0];
    float h = size[1];
    std::vector<GLfloat> vertices = {
      -w/2, h/2, 0,
       w/2, h/2, 0,
       w/2,-h/2, 0,
      -w/2,-h/2, 0,
    };

    std::vector<GLfloat> normals = {
      0.f, 0.f, -1.f,
      0.f, 0.f, -1.f,
      0.f, 0.f, -1.f,
      0.f, 0.f, -1.f,
    };

    std::vector<GLint> elements = {
      0, 1, 2, 2, 3, 0,
    };

    bindBuffers(vertices, elements, normals);
  }
};
