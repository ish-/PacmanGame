#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

inline std::string loadFile(const char* filePath)
{
  // Read the Vertex Shader code from the file
  std::string content;
  std::ifstream fileStream(filePath, std::ios::in);
  if (fileStream.is_open()) {
    std::stringstream sstr;
    sstr << fileStream.rdbuf();
    content = sstr.str();
    fileStream.close();
  } else {
    printf("Impossible to open %s. Are you in the right directory?\n", filePath);
    getchar();
    return "0";
  }
  return content;
}
