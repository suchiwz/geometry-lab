#pragma once

#ifndef GEOMETRY_LAB_RENDER_SHADER_HPP_
#define GEOMETRY_LAB_RENDER_SHADER_HPP_

#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "render_config.hpp"

namespace geometry_lab {

/**
 * @brief APIs for shader
*/
class Shader {
 public:
  bool LoadFromFile(const char* vert_path, const char* frag_path,
                    const char* geom_path = nullptr) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      // open files
      vShaderFile.open(vert_path);
      fShaderFile.open(frag_path);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      // if geometry shader path is present, also load a geometry shader
      if (geom_path != nullptr) {
        gShaderFile.open(geom_path);
        std::stringstream gShaderStream;
        gShaderStream << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = gShaderStream.str();
      }
    } catch (std::ifstream::failure&) {
      printf("ERROR::Shader::File not successfully read.\n\n");
      return false;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if (geom_path != nullptr) {
      const char* gShaderCode = geometryCode.c_str();
      geometry = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geometry, 1, &gShaderCode, NULL);
      glCompileShader(geometry);
    }
    // shader Program
    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    if (geom_path != nullptr)
      glAttachShader(id_, geometry);
    glLinkProgram(id_);
    // delete shader
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geom_path != nullptr)
      glDeleteShader(geometry);
    return true;
  }
  void Use() const { glUseProgram(id_); }

  void set_bool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(id_, name), (int)value);
  }
  void set_int(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(id_, name), value);
  }
  void set_float(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(id_, name), value);
  }
  void set_vec2(const char* name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(id_, name), 1, &value[0]);
  }
  void set_vec3(const char* name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(id_, name), 1, &value[0]);
  }
  void set_vec4(const char* name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(id_, name), 1, &value[0]);
  }
  void set_mat2(const char* name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(id_, name), 1, GL_FALSE,
                       &mat[0][0]);
  }
  void set_mat3(const char* name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(id_, name), 1, GL_FALSE,
                       &mat[0][0]);
  }
  void set_mat4(const char* name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE,
                       &mat[0][0]);
  }

  Shader() {}
  GLuint id_ = 0;
};

class MeshFillShader : public Shader {
 public:
  MeshFillShader() {
    LoadFromFile(GEOMETRY_LAB_MESH_FILL_VERT, GEOMETRY_LAB_MESH_FILL_FRAG);
  }
  static std::shared_ptr<MeshFillShader> instance() {
    static auto ptr = std::make_shared<MeshFillShader>();
    return ptr;
  }
  void set_parameters(const glm::mat4& model, const glm::mat4& view,
                      const glm::mat4& proj, const glm::vec3& view_pos,
                      const glm::vec3& light_pos,
                      const glm::vec3& light_color) const {
    set_mat4("model", model);
    set_mat4("view", view);
    set_mat4("projection", proj);
    set_vec3("light_color", light_color);
    set_vec3("light_pos", light_pos);
    set_vec3("view_pos", view_pos);
  }
};

class MeshLineShader : public Shader {
 public:
  MeshLineShader() {
    LoadFromFile(GEOMETRY_LAB_MESH_LINE_VERT, GEOMETRY_LAB_MESH_LINE_FRAG);
  }
  static std::shared_ptr<MeshLineShader> instance() {
    static auto ptr = std::make_shared<MeshLineShader>();
    return ptr;
  }
  void set_parameters(const glm::mat4& model, const glm::mat4& view,
                      const glm::mat4& proj, const glm::vec3& view_pos,
                      float bias) const {
    set_mat4("model", model);
    set_mat4("view", view);
    set_mat4("projection", proj);
    set_vec3("view_pos", view_pos);
    set_float("bias", bias);
  }
};

}  // namespace geometry_lab

#endif  // !GEOMETRY_LAB_RENDER_SHADER_HPP_
