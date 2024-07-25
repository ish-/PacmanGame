#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <memory>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "SpotLight.hpp"

void _render(std::shared_ptr<Camera> camera, std::shared_ptr<Mesh> mesh, std::shared_ptr<SpotLight> spotLight)
{
  GLuint shId = mesh->shaderId;
  glm::mat4 view = camera->model;
  glm::mat4 modelView = view * mesh->model;
  glm::mat4 modelViewProjection = camera->projection * modelView;
  glm::mat4 normalMat = glm::inverseTranspose(modelView);

  glUseProgram(shId);
  glUniformMatrix4fv(glGetUniformLocation(shId, "model"), 1, GL_FALSE, glm::value_ptr(mesh->model));
  glUniformMatrix4fv(glGetUniformLocation(shId, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(shId, "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));
  glUniformMatrix4fv(glGetUniformLocation(shId, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
  glUniformMatrix4fv(glGetUniformLocation(shId, "modelViewProjection"), 1, GL_FALSE,
                     glm::value_ptr(modelViewProjection));
  glUniformMatrix4fv(glGetUniformLocation(shId, "normalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));
  glUniform3fv(glGetUniformLocation(shId, "lightPos"), 1, glm::value_ptr(spotLight->model));

  mesh->draw();
}

class Scene: public std::list<std::shared_ptr<Mesh>> {
public:
  std::shared_ptr<Camera> camera;
  std::shared_ptr<SpotLight> light;

  Scene(std::shared_ptr<Camera> camera_, std::shared_ptr<SpotLight> light_): camera(camera_), light(light_)
  {
  }

  void render()
  {
    for (std::shared_ptr<Mesh> mesh: (*this)) {
      if (mesh->hidden)
        continue;
      _render(camera, mesh, light);
    }
  }
};
