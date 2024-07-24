#pragma once
#include "Geo.hpp"

#include <glm/glm.hpp>
#include <vector>

class BoxGeo: public Geo {
public:
  BoxGeo(float _size = 1.)
  {
    size = glm::vec2(_size, _size);
    float s = _size / 2.;

    // clang-format off
    std::vector<GLfloat> vertices = {
      // -z
      -s, s, -s,
      s, s, -s,
      s, -s, -s,
      -s, -s, -s,
      // z
      -s, s, s,
      s, s, s,
      s, -s, s,
      -s, -s, s,
      // x
      s, -s, s,
      s, s, s,
      s, s, -s,
      s, -s, -s,
      // -x
      -s, -s, s,
      -s, s, s,
      -s, s, -s,
      -s, -s, -s,
      // y
        s, s, -s,
        s, s, s,
      -s, s, s,
      -s, s, -s,

      // -y
        s, -s, -s,
        s, -s, s,
      -s, -s, s,
      -s, -s, -s,
    };

    std::vector<GLfloat> normals = {
      0.f, 0.f, 1.f,
      0.f, 0.f, 1.f,
      0.f, 0.f, 1.f,
      0.f, 0.f, 1.f,

      0.f, 0.f, -1.f,
      0.f, 0.f, -1.f,
      0.f, 0.f, -1.f,
      0.f, 0.f, -1.f,

      -1.f, 0.f, 0.f,
      -1.f, 0.f, 0.f,
      -1.f, 0.f, 0.f,
      -1.f, 0.f, 0.f,

      1.f, 0.f, 0.f,
      1.f, 0.f, 0.f,
      1.f, 0.f, 0.f,
      1.f, 0.f, 0.f,

      0.f, -1.f, 0.f,
      0.f, -1.f, 0.f,
      0.f, -1.f, 0.f,
      0.f, -1.f, 0.f,

      0.f, 1.f, 0.f,
      0.f, 1.f, 0.f,
      0.f, 1.f, 0.f,
      0.f, 1.f, 0.f,
    };

    std::vector<GLint> elements = {
      0, 1, 2, 2, 3, 0,
      4, 5, 6, 6, 7, 4,
      8, 9, 10, 10, 11, 8,
      12, 13, 14, 14, 15, 12,
      16, 17, 18, 18, 19, 16,
      20, 21, 22, 22, 23, 20,
    };
    // clang-format on

    bindBuffers(vertices, elements, normals);
  }
};
