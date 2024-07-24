#pragma once

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <cmath>
#include <glm/glm.hpp>

#include "common.hpp"

#include <memory>

class App {
public:
  static App& get()
  {
    static App instance;
    return instance;
  }

  SDL_Window* window;
  SDL_GLContext context;

  Dir arrows = Dir(0, 0);

  bool quit = false;
  bool pause = false;
  SDL_Event mouse;
  Uint32 lastTime = SDL_GetTicks();
  Uint32 startTime = lastTime;
  int frame = 0;
  glm::vec2 mouseMove;
  glm::vec2 mousePos;
  bool mousePress;
  float dT;
  float eT;
  Uint32 now;
  float win = 0.;

  template <class T>
  void loop(T callback)
  {
    while (!quit) {
      now = SDL_GetTicks();
      dT = float(now - lastTime) / 1000.;  // delta time
      eT = float(now - startTime) / 1000.;  // elapsed time

      SDL_Event event;
      mouse.motion.xrel = 0.;
      mouse.motion.yrel = 0.;
      mousePress = 0;

      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
              case SDLK_RIGHTBRACKET:
                // next frame
                break;
              case SDLK_ESCAPE:
              case SDLK_q:
                quit = true;
                break;
              case SDLK_p:
                pause = !pause;
                break;
              case SDLK_w:
                arrows = UP;
                break;
              case SDLK_s:
                arrows = DOWN;
                break;
              case SDLK_d:
                arrows = LEFT;
                break;
              case SDLK_a:
                arrows = RIGHT;
                break;
            }
            break;

          case SDL_MOUSEMOTION:
          case SDL_MOUSEBUTTONDOWN:
            mousePress = event.button.state == SDL_PRESSED;
            mouse = event;
            // break;
            // _log("button", event.button.button);
            // _log("state", (event.button.state == SDL_PRESSED));
            break;

          case SDL_QUIT:
            quit = true;
            break;
        }
      }

      mouseMove = {(float)mouse.motion.xrel / (float)W_W, (float)mouse.motion.yrel / (float)W_H};
      mousePos = {(float)mouse.motion.x / (float)W_W, 1. - (float)mouse.motion.y / (float)W_H};

      if (!pause && !win) {  // stop GL redrawing
        glViewport(0, 0, W_W, W_H);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        callback();
        SDL_GL_SwapWindow(window);
      }

      if (win > 0.) {  // count down timeout before quit after win
        win -= dT;
        if (win <= 0.)
          quit = true;
      }

      Uint32 loopTime = SDL_GetTicks() - now;
      if (loopTime < W_FRAME_TIME) {
        float delay = W_FRAME_TIME - loopTime;
        SDL_Delay(delay);
      }
      lastTime = now;
      frame++;
    }
  }

  void showWin()
  {
    win = 2.;

    SDL_Surface* img = SDL_LoadBMP("../game/victory.bmp");
    if (img == NULL) {
      printf("Unable to load image! SDL Error: %s\n", SDL_GetError());
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* tex1 = SDL_CreateTextureFromSurface(renderer, img);

    SDL_RenderCopy(renderer, tex1, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_UpdateWindowSurface(window);

    // SDL_DestroyTexture(tex1);
    // SDL_DestroyRenderer(renderer);
    // SDL_FreeSurface(img);
    _log("Victory");
  }

  void lose()
  {
    _log("Lose");
    quit = true;
  }

private:
  App()
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::printf("SDL failed initialization. %s", SDL_GetError());
      // throw std::exception("Something Bad happened here");
      throw std::runtime_error("SDL failed initialization.");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    window = SDL_CreateWindow("PacmanGame", 0, 0, W_W, W_H, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  ~App()
  {
    _log("~App()");
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
};
