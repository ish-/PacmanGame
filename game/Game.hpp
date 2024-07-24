#pragma once

#include "App.hpp"
#include "Cube.hpp"
#include "GameMap.hpp"
#include "common.hpp"

#include "myogl/Camera.hpp"
#include "myogl/Mesh.hpp"
#include "myogl/RectGeo.hpp"
#include "myogl/Scene.hpp"
#include "myogl/SpotLight.hpp"

#include <algorithm>
#include <list>
#include <random>

class Game {
private:
  Game()
  {
    std::mt19937 rdGen(rd());

    // Scene initialization
    spotLight = new SpotLight();
    gameMap = new GameMap();

    sceneCenter = glm::vec3(gameMap->size.x / 2., gameMap->size.y / 2., 0.);
    spotLight->setPosition(glm::vec3(sceneCenter.x, sceneCenter.y, 3.0));
    camera = new Camera(W_FOV, float(W_W) / float(W_H), .1, 1000.);
    float camZ = 11.;
    camera->setPosition(glm::vec3(-sceneCenter.x, -sceneCenter.y, -camZ));
    scene = new Scene(camera, spotLight);

    cubeGeo = new BoxGeo();
    Cube::setGeo(cubeGeo);

    for (int i = 0; i < std::size(gameMap->grid); i++) {
      float x = i % gameMap->size.x;
      float y = int(i / gameMap->size.x);
      CubeType cubeType = gameMap->grid.at(i);
      Cube* cube = new Cube(cubeType);

      glm::vec3 startPos = glm::vec3(x + .5, y + .5, 0.);
      cube->setPosition(startPos);

      if (cubeType == PLAYER)
        player = cube;
      else if (cubeType == GHOST) {
        // if (ghosts.size() < 1) {
        cube->startPos = startPos;
        cube->color = GHOSTS_COLOR[ghosts.size()];
        cube->dir = COMPASS[rdGen() % 4];
        ghosts.push_back(cube);
        // }
      } else {
        staticCubes.push_back(cube);
      }
      if (cubeType == FOOD) {
        remainingFood++;
      }

      if (cube->dynamic)
        dynamicCubes.push_back(cube);

      scene->push_back(cube);
    }

    floorGeo = new RectGeo(gameMap->size.x);
    floor = new Mesh(floorGeo, glm::vec3(.0, .1, .3));
    floor->setScale(2.);
    floor->setPosition(glm::vec3(gameMap->size.x / 2. - .5, gameMap->size.x / 2. - .5, -.5));
    scene->push_back(floor);
  }

  ~Game()
  {
    _log("~Game()");
    delete cubeGeo;
    delete floorGeo;
    delete camera;
    delete spotLight;
    delete scene;
  }

public:
  static bool checkCollision(Cube* one, Cube* two)
  {
    CubeBBox oneBb = one->getBBox();
    CubeBBox twoBb = two->getBBox();
    bool collisionX = oneBb.pos.x + oneBb.size.x >= twoBb.pos.x && twoBb.pos.x + twoBb.size.x >= oneBb.pos.x;
    bool collisionY = oneBb.pos.y + oneBb.size.y >= twoBb.pos.y && twoBb.pos.y + twoBb.size.y >= oneBb.pos.y;
    return collisionX && collisionY;
  }
  static Dir quantizeDir(glm::vec2& dir)
  {
    float angle = atan2(dir.y, dir.x);
    if (-PI / 4 <= angle && angle < PI / 4)
      return LEFT;
    else if (PI / 4 <= angle && angle < 3 * PI / 4)
      return UP;
    else if (-3 * PI / 4 <= angle && angle < -PI / 4)
      return DOWN;
    else
      return RIGHT;
  }
  static Game& get()
  {
    static Game instance;
    return instance;
  }

  GameMap* gameMap;
  Scene* scene;
  Camera* camera;
  SpotLight* spotLight;
  glm::vec3 sceneCenter;
  Cube* player;

  RectGeo* floorGeo;
  BoxGeo* cubeGeo;

  std::list<Cube*> dynamicCubes;
  std::list<Cube*> staticCubes;
  std::vector<Cube*> ghosts;

  Mesh* floor;

  int remainingFood = 0;
  float frightenedTime = false;

  std::random_device rd;
  std::mt19937 rdGen;

  void onLoop()
  {
    App& app = App::get();

    player->dir = app.arrows;

    scene->render();

    // if (app.mousePress) {
    //   app.showWin();
    // }

    if (frightenedTime) {  // frightened mode
      int ghostIdx = 0;
      for (Cube* ghost: ghosts) {
        ghost->color = remainder(app.eT, .2) > 0. ? Color(1., 1., 0.) : CONF[GHOST].color;

        if (app.eT - frightenedTime > FRIGHTENED_TIMEOUT) {  // RESET FRIGHTENED
          frightenedTime = 0.;
          ghost->color = GHOSTS_COLOR[ghostIdx % 4];
        }
        ghostIdx++;
      }
    }

    for (Cube* dynamicCube: dynamicCubes) {
      dynamicCube->move(app.dT);
      loopMapBounds(dynamicCube);
    }

    doCollisionsWithStaticCubes(player);

    _LOG = false;
    for (int i = 0; i < std::size(ghosts); i++) {
      Cube* ghost = ghosts[i];
      doCollisionsWithStaticCubes(ghost, i);
    }
    _LOG = true;

    doPlayerCollisionsWithGhosts();
  }

  void doPlayerCollisionsWithGhosts()
  {
    for (Cube* ghost: ghosts) {
      bool collision = Game::checkCollision(player, ghost);
      if (collision) {
        if (frightenedTime) {
          ghost->setPosition(glm::vec3(ghost->startPos, 0.));
        } else {
          App::get().lose();
        }
      }
    }
  }

  void doCollisionsWithStaticCubes(Cube* dynamicCube, int i = 0)
  {
    for (Cube* staticCube: staticCubes) {
      if (staticCube->hidden)
        continue;
      bool collision = Game::checkCollision(dynamicCube, staticCube);
      // staticCube->color = collision ? glm::vec3(1., 0., 0.) : CONF[WALL].color;

      if (collision) {
        if (dynamicCube->type == PLAYER) {  // PLAYER
          if (staticCube->type == WALL) {
            player->model = player->prevTransformMat;
          } else if (staticCube->type == FOOD) {  // PLAYER - FOOR
            staticCube->hidden = true;
            remainingFood--;
            if (remainingFood < 1) {
              App::get().showWin();
              return;
            }
          } else if (staticCube->type == SFOOD) {  // PLAYER - FOOR
            // scene->remove(staticCube);
            // staticCubes.remove(staticCube);
            staticCube->hidden = true;
            frightenedTime = App::get().eT;
          }
        } else if (dynamicCube->type == GHOST) {  // GHOST
          if (staticCube->type == WALL) {
            dynamicCube->model = dynamicCube->prevTransformMat;
            // dynamicCube->dir = glm::vec2(0.);
            changeGhostDir(dynamicCube, i);
          }
        }
      }
    }
  }

  void changeGhostDir(Cube* ghost, int ghostIdx = 0)
  {
    _log("<\\\\\\ changeGhostDir", ghostIdx);
    if (!ghost || !gameMap || !player)
      throw std::runtime_error("Game: changeGhostDir(): !ghost || !gameMap || !player");

    std::vector<Dir> freeDirs = gameMap->getFreeDirs(ghost->position);
    if (freeDirs.empty()) {
      _log("freeDirs.empty(); pos = ", glm::to_string(ghost->position));
      return;
    }

    _log("freeDirs.size() == ", freeDirs.size());
    if (freeDirs.size() > 1) {  // exclude dir that is from ghost came
      _log("my->dir", glm::to_string(-ghost->dir));
      auto it = std::find(freeDirs.begin(), freeDirs.end(), -ghost->dir);

      if (it != freeDirs.end()) {
        freeDirs.erase(it);
      }
    }

    Dir dir;
    bool toChase = (rdGen() % 10) > ((1. + ghostIdx) / 5) * 10;
    if (toChase) {
      glm::vec2 dirToPlayer = glm::normalize(glm::vec2(player->position - ghost->position));
      if (frightenedTime)  // invert in frightened mode (SFOOD)
        dirToPlayer = -dirToPlayer;
      dir = Game::quantizeDir(dirToPlayer);

      // get random if dirToPlayer is the WALL
      bool dirIsObstacle = find(freeDirs.begin(), freeDirs.end(), dir) == freeDirs.end();
      if (dirIsObstacle) {
        dir = freeDirs.at(rdGen() % freeDirs.size());
      }
    } else {
      dir = freeDirs.at(rdGen() % freeDirs.size());
    }

    _log("dir = ", glm::to_string(dir));
    _log("</// changeGhostDir");

    ghost->dir = dir;
  }

  void loopMapBounds(Cube* cube)
  {
    float x = cube->getPosition().x;
    if (x < 0.)
      cube->position.x = float(gameMap->size.x) - .1;
    if (x > (gameMap->size.x))
      cube->position.x = .1;
    cube->updateMat();
  }
};
