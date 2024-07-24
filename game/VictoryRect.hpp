#pragma once

#include "conf.hpp"
#include "myogl/Mesh.hpp"
#include "myogl/RectGeo.hpp"

class VictoryRect: public Mesh {
public:
  GLuint texId;

  VictoryRect(): Mesh(new RectGeo(12.), Color(1., 0., 0.))
  {
    SDL_Surface* img = SDL_LoadBMP("../game/victory.bmp");
    if (img == NULL) {
      printf("Unable to load image! SDL Error: %s\n", SDL_GetError());
    }
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_, img->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(img);
    _log("Victory tex is binded");
  }

  void beforeDraw() override
  {
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(glGetUniformLocation(shaderId, "tex1"), GL_TEXTURE0 + 0);

    // Mesh::draw();
  }
};
