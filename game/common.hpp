#pragma once

#include <array>
#include <glm/glm.hpp>
#include <map>
#define PI 3.14159265358979323846f

using namespace glm;

static constexpr int W_W = 800;  // window
static constexpr int W_H = 800;
static const int W_MAX = fmax(W_W, W_H);
static const int W_MIN = fmin(W_W, W_H);
static constexpr float W_RATIO = float(W_W) / float(W_H);
static constexpr float W_FPS = 60;
static constexpr int W_FRAME_TIME = 1000. / W_FPS;
static constexpr float W_FOV = 80.;

typedef vec2 Dir;
Dir NODIR(0., 1.);
Dir UP(0., 1.);
Dir DOWN(0., -1.);
Dir LEFT(1., 0.);
Dir RIGHT(-1., 0.);

std::array<Dir, 4> COMPASS = {RIGHT, UP, LEFT, DOWN};

typedef vec3 Color;
typedef std::string CubeType;

struct CubeConf {
  CubeType type;
  Color color;
  float size;
  float dynamic;  // speeed
  bool obstacle;
};

const CubeType PLAYER = "PLAYER";
const CubeType GHOST = "GHOST";
const CubeType WALL = "WALL";
const CubeType EMPTY = "EMPTY";
const CubeType GATE = "GATE";
const CubeType FOOD = "FOOD";
const CubeType SFOOD = "SFOOD";

std::map<CubeType, CubeConf> CONF = {{WALL,
                                      {
                                          .type = WALL,
                                          .color = Color(0., 0., 1.),
                                          .size = .95,
                                          .dynamic = false,
                                          .obstacle = true,
                                      }},
                                     {PLAYER,
                                      {
                                          .type = PLAYER,
                                          .color = Color(1., 1., 0.),
                                          .size = .6,
                                          .dynamic = 1.,
                                          .obstacle = false,
                                      }},
                                     {GHOST,
                                      {
                                          .type = GHOST,
                                          .color = Color(1., 0., 0.),
                                          .size = .7,
                                          .dynamic = .9,
                                          .obstacle = false,
                                      }},
                                     {FOOD,
                                      {
                                          .type = FOOD,
                                          .color = Color(.1, .5, 0.),
                                          .size = .2,
                                          .dynamic = false,
                                          .obstacle = false,
                                      }},
                                     {SFOOD,
                                      {
                                          .type = SFOOD,
                                          .color = Color(.1, 1., 0.),
                                          .size = .4,
                                          .dynamic = false,
                                          .obstacle = false,
                                      }}};

std::map<char, CubeType> MAP_TO_CUBE = {
    {'0', FOOD}, {'4', SFOOD}, {'1', WALL}, {'2', GATE}, {'3', EMPTY}, {'8', GHOST}, {'9', PLAYER},
};

std::array<Color, 4> GHOSTS_COLOR = {Color(1., 0., 0.), Color(1, 0.722, 1), Color(1, 0.722, 0.318), Color(0.016, 1, 1)};

const float MOVE_SPEED = 2.5;
const float FRIGHTENED_TIMEOUT = 20.;

bool _LOG = true;
template <typename T1, typename T2 = const char*>
void _log(T1 str1, T2 str2 = "")
{
  if (_LOG) {
    std::cout << str1 << " " << str2 << "\n";
  }
}