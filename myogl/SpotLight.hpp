#pragma once
#include "./Object3D.hpp"

class SpotLight: public Object3D {
public:
  glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.31f);
  float power = 1.;
};