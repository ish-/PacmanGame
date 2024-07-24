#pragma once

#include "myogl/Geo.hpp"
#include "myogl/Object3D.hpp"
#include "myogl/simpleShader.hpp"
#include "util/loadShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


class Mesh: public Object3D {
public:
  GLuint shaderId;
  glm::vec3 color = glm::vec3(1., 1., 1.);
  Geo* geo;
  bool hidden = false;

  Mesh(Geo* _geo, glm::vec3 _color = glm::vec3(1., 1., 1.))
  {
    geo = _geo;
    color = _color;
    shaderId = getSimpleShaderId();
    // Mesh();
  }

  // Mesh () {
  //   shaderId = loadShaders("shaders/pos-norm.vert.glsl", "shaders/const-norm.frag.glsl");
  // }

  void draw()
  {
    glBindVertexArray(geo->vertexArrayId);

    int colorLoc = glGetUniformLocation(shaderId, "diffuseCol");
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawElements(GL_TRIANGLES, geo->vertexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
  }

  ~Mesh()
  {
    glDeleteProgram(shaderId);
  }
};
