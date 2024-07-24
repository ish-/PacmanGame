#pragma once
#include "./Object3D.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera: public Object3D {
public:
  glm::mat4 projection;

  Camera(float fov, float ratio, float near, float far)
  {
    projection = glm::perspective(glm::radians(fov), ratio, near, far);
  }
};
