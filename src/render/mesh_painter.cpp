#include "mesh_painter.hpp"
#include "shader.hpp"

namespace geometry_lab {
void MeshPainter::Draw() const {
  const glm::mat4 M = model_.GetModel();
  const glm::mat4 V = camera_.GetView();
  const glm::mat4 P = camera_.GetProjection();
  glBindVertexArray(vao_);
  switch (fill_mode_) {
    case FillMode::kLineAndFill: {
      // Firstly, draw faces
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      MeshFillShader::instance()->Use();
      MeshFillShader::instance()->set_parameters(M, V, P, camera_.position_,
                                                 point_light_.position_,
                                                 point_light_.color_);
      glDrawElements(GL_TRIANGLES, 3 * static_cast<GLsizei>(indices_.size()),
                     GL_UNSIGNED_INT, 0);
      // The, draw lines on the two sides
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      MeshLineShader::instance()->Use();
      MeshLineShader::instance()->set_parameters(M, V, P, camera_.position_,
                                                 offset_);
      glDrawElements(GL_TRIANGLES, 3 * static_cast<GLsizei>(indices_.size()),
                     GL_UNSIGNED_INT, 0);
      MeshLineShader::instance()->set_parameters(M, V, P, camera_.position_,
                                                 -offset_);
      glDrawElements(GL_TRIANGLES, 3 * static_cast<GLsizei>(indices_.size()),
                     GL_UNSIGNED_INT, 0);

      break;
    }
    case FillMode::kLine: {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      MeshLineShader::instance()->Use();
      MeshLineShader::instance()->set_parameters(M, V, P, camera_.position_,
                                                 0.0f);
      glDrawElements(GL_TRIANGLES, 3 * static_cast<GLsizei>(indices_.size()),
                     GL_UNSIGNED_INT, 0);
      break;
    }
    case FillMode::kFill: {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      MeshFillShader::instance()->Use();
      MeshFillShader::instance()->set_parameters(M, V, P, camera_.position_,
                                                 point_light_.position_,
                                                 point_light_.color_);
      glDrawElements(GL_TRIANGLES, 3 * static_cast<GLsizei>(indices_.size()),
                     GL_UNSIGNED_INT, 0);
      break;
    }
    default:
      assert(false);
  }
  glBindVertexArray(0);
}
void MeshPainter::InitGlBuffers() {
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);
  glBindVertexArray(0);
}
void MeshPainter::LoadVertexBuffer() {
  assert(vertices_.size() > 0);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertices_[0]),
               vertices_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices_[0]),
                        (void*)0);
  glEnableVertexAttribArray(0);  // Vertex Position
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices_[0]),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);  // Vertex Normal
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices_[0]),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);  // Vertex Color
  glBindVertexArray(0);
}
void MeshPainter::LoadElementBuffer() {
  assert(indices_.size() > 0);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(indices_[0]),
               indices_.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
}
MeshPainter::~MeshPainter() {
  glDeleteBuffers(1, &ebo_);
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
}
}  // namespace geometry_lab