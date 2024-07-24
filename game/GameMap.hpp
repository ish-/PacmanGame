#include "Cube.hpp"
#include "common.hpp"

#include <algorithm>
#include <string>
#include <vector>

// 1000000001000000001
// 1011011101011101101
const std::string gridTxt = R"(
1111111111111111111
1000000000000000001
1011410111114101101
1000010001000100001
1111011101011101111
3331013388833101333
1111013133313101111
3333333138313333333
1111013111113101111
3331013333333101333
1111010111110101111
1000000001000000001
1011011101011101101
1001000009000001001
1101010111110101011
1000010001000100001
1011111101011111101
1004000001000004001
1111111111111111111
)";

class GameMap {
public:
  struct Size {
    int x;
    int y;
  } size;
  std::vector<CubeType> grid;

  GameMap()
  {
    int lines = 0;
    int maxHSize = 0;
    int curHSize = 0;
    bool newLine = true;
    for (char c: gridTxt) {
      if (c >= '0' && c <= '9') {
        if (newLine) {
          newLine = false;
          lines++;
        }
        // int v = c - 48;
        CubeType CubeType = MAP_TO_CUBE[c];
        // std::printf("%d \n", v);
        grid.push_back(CubeType);
        curHSize++;
      } else if (c == '\n') {
        newLine = true;
        maxHSize = curHSize > maxHSize ? curHSize : maxHSize;
        curHSize = 0;
      }
    }
    size = {maxHSize, lines};
    std::reverse(grid.begin(), grid.end());
    std::printf("GameMap.size: [%d, %d]", size.x, size.y);
    std::cout << "\n";
  }

  std::vector<Dir> getFreeDirs(glm::vec2 pos)
  {
    std::vector<Dir> dirs;
    for (int i = 0; i < COMPASS.size(); i++) {
      Dir dir = COMPASS[i];
      // CubeType cell = getCell(pos + dir + glm::vec2(1., 0.));
      CubeType cell = getCell(pos + dir);
      if (cell != WALL)
        dirs.push_back(dir);
    }
    return dirs;
  }

  // std::vector<CubeType> getNeighbors(glm::vec2 pos)
  // {
  //   glm::ivec2 ipos(int(pos.x), int(pos.y));
  //   int x = int(pos.x);
  //   int y = int(pos.y);
  //   std::vector<CubeType> kernel{getCell(x - 1, y + 1), getCell(x, y + 1),    getCell(x + 1, y + 1),
  //                                getCell(x - 1, y),     getCell(x + 1, y),    getCell(x - 1, y - 1),
  //                                getCell(x, y - 1),     getCell(x + 1, y - 1)};
  //   return kernel;
  // }

  CubeType getCell(glm::vec2 pos)
  {
    // int i = fmin(ipos.y * size.x + ipos.x, std::size(grid) - 1);
    int i = floor(pos.y) * size.x + floor(pos.x);
    // _log(grid.at(i), glm::to_string(pos));
    // _log(floor(pos.x), floor(pos.y));
    if (i < 0 || i > std::size(grid) - 1) {
      std::cout << "i: " << i << " ";
      _log("getCell().at(>size())", glm::to_string(pos));
      return WALL;
    }
    return grid.at(i);
  }
};
