#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

const mat4 I_MAT = mat4(1.f);

class Object3D {
public:
  mat4 model = I_MAT;

  vec3 position = vec3(0.);

  void updateMat()
  {
    model[3] = vec4(position, 1.0f);
  }

  void setScale(const vec3& scale)
  {
    model = glm::scale(glm::mat4(1.0f), scale) * glm::mat4(glm::mat3(model));
  }
  void setScale(float scale)
  {
    setScale(vec3(scale, scale, scale));
  }

  void setPosition(const vec3& _position)
  {
    model[3] = vec4(_position, 1.0f);
    position = _position;
  }

  vec3 getPosition()
  {
    return vec3(model[3]);
  }

  void translate(const glm::vec3& translation)
  {
    glm::mat4 translationMat = glm::translate(I_MAT, translation);
    model = translationMat * model;
    position = vec3(model[3]);
  }

  void translateLocal(const vec3& tranlateBy)
  {
    model = glm::translate(model, tranlateBy);
  }

  quat getRotation()
  {
    mat3 rotMat = mat3(model);
    return quat_cast(rotMat);
  }
  vec3 getRotationEul()
  {
    return eulerAngles(getRotation());
  }

  void setRotation(const glm::vec3& angles)
  {
    glm::mat4 rotationX = glm::rotate(I_MAT, angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(I_MAT, angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(I_MAT, angles.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;
    glm::vec3 translation = glm::vec3(model[3]);
    model = glm::translate(I_MAT, translation) * rotationMatrix;
  }
  void setRotationEul(const glm::vec3& eulerAngles)
  {
    return setRotation(radians(eulerAngles));
  }

  void setRotationLocal(const vec3& angles)
  {
    vec3 translation = vec3(model[3]);
    quat quater = quat(angles);
    mat4 rotMat = toMat4(quater);
    rotMat[3] = vec4(translation, 1.0f);
    model = rotMat;
  }
  void setRotationEulLocal(const vec3& eulerAngs)
  {
    return setRotationLocal(radians(eulerAngs));
  }

  void rotate(const vec3& angles)
  {
    mat4 rotationX = glm::rotate(I_MAT, angles.x, vec3(1.0f, 0.0f, 0.0f));
    mat4 rotationY = glm::rotate(I_MAT, angles.y, vec3(0.0f, 1.0f, 0.0f));
    mat4 rotationZ = glm::rotate(I_MAT, angles.z, vec3(0.0f, 0.0f, 1.0f));
    mat4 rotationMatrix = rotationZ * rotationY * rotationX;
    model = rotationMatrix * model;
  }
  void rotateEul(const vec3& eulerAngs)
  {
    rotate(radians(eulerAngs));
  }

  void rotateLocal(const vec3& angles)
  {
    quat quater = quat(angles);
    mat4 rotationMatrix = toMat4(quater);
    model = model * rotationMatrix;
  }
  void rotateEulLocal(const vec3& eulerAngs)
  {
    rotateLocal(radians(eulerAngs));
  }
};
