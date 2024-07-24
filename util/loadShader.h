#pragma once

#include "loadShader.h"

#include <SDL.h>
#include <glad/glad.h>
#include <string>

inline GLuint loadShader(std::string& VertexShaderCode, std::string& FragmentShaderCode)
{
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  GLint Result = GL_FALSE;
  int InfoLogLength;

  printf("Compiling shader : %s\n", "vertex");
  char const* VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::string VertexShaderErrorMessage("", InfoLogLength + 1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, VertexShaderErrorMessage.data());
    printf("%s\n", VertexShaderErrorMessage.c_str());
  }

  printf("Compiling shader : %s\n", "fragment");
  char const* FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::string FragmentShaderErrorMessage("", InfoLogLength + 1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, FragmentShaderErrorMessage.data());
    printf("%s\n", FragmentShaderErrorMessage.c_str());
  }

  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::string ProgramErrorMessage("", InfoLogLength + 1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage.data());
    printf("%s\n", ProgramErrorMessage.c_str());
  }

  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  if (!ProgramID) {
    SDL_Log("loadShaders() == 0");
    std::printf("loading vert(%s) frag(%s)", "vertex", "fragment");
    return EXIT_FAILURE;
  }

  return ProgramID;
}
