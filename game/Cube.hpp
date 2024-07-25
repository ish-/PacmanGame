#pragma once

#include "common.hpp"
#include "myogl/BoxGeo.hpp"
#include "myogl/Geo.hpp"
#include "myogl/Mesh.hpp"

#include <glm/glm.hpp>

struct CubeBBox {
  glm::vec2 pos;
  glm::vec2 size;
};

// struct PathDir {
//   std::vector<Dir> dirs;
//   float lastTime;
// };

std::shared_ptr<Geo> boxGeo;

class Cube: public Mesh {
public:
  static void setGeo(std::shared_ptr<Geo> _boxGeo)
  {
    boxGeo = _boxGeo;
  }
  static void unsetGeo()
  {
    boxGeo.reset();
  }

  float size = 1.;
  CubeType type = EMPTY;
  bool obstacle = false;
  float dynamic = false;
  // PathDir pathDir{{NODIR}, 0.};
  glm::vec2 startPos;

  mat4 prevTransformMat;
  glm::vec2 dir = glm::vec2(0., 0.);

  Cube(CubeType cubeType): Mesh(boxGeo)
  {
    CubeConf conf = CONF[cubeType];
    this->size = conf.size;
    this->type = conf.type;
    this->color = conf.color;
    this->obstacle = conf.obstacle;
    this->dynamic = conf.dynamic;
    setScale(conf.size);
    prevTransformMat = model;
  }

  CubeBBox getBBox()
  {
    glm::vec2 size(this->size);
    // glm::vec2 size (.9);
    glm::vec2 pos = glm::vec2(this->getPosition()) - (size / 2.f);
    return {pos, size};
  }

  void move(float dT)
  {
    if (dynamic) {
      prevTransformMat = model;
      translate(glm::vec3(dir, 0.) * dT * MOVE_SPEED * dynamic);
    }
  }
};
