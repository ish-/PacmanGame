#include <glm/gtx/string_cast.hpp>  // debug
#include <iostream>

#include "game/App.hpp"
#include "game/Game.hpp"
#include "game/common.hpp"

#undef main
int main(int argc, char* argv[])
{
  App& app = App::get();
  Game& game = Game::get();

  app.loop([&game]() { game.onLoop(); });
  return 0;
}
