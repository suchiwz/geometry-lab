#pragma once

#ifndef GEOMETRY_LAB_RENDER_MESH_PAINTER
#define GEOMETRY_LAB_RENDER_MESH_PAINTER

#include <cassert>
#include <vector>

#include "painter.hpp"

namespace geometry_lab {

/**
 * @brief Load mesh info to the GL buffer for rendering, we
 *  seperate the rendering object and computing object in the 
 *  engine, and bind them in loader.hpp.
*/
class MeshPainter : public Painter {
 public:
  /**
   * @brief Package the properties of a vertex and send
   *  them to the buffer together.
  */
  struct VertInfo {
    glm::vec3 pos, normal, color;
  };
  /**
   * @brief Fill the triangles or draw the line sketch.
  */
  enum class FillMode {
    /// Draw twice, the sketch and the triangles.
    kLineAndFill = 0,
    /// Only the line sketch.
    kLine,
    /// Only fill the faces.
    kFill,
  };
  /**
   * @brief Call this function to draw!
  */
  void Draw() const;
  /**
   * @brief Update the position infomation in @c vertices_.
   * @param positions[in] - New positions for updating. 
  */
  void UpdatePositions(const std::vector<glm::vec3>& positions) {
    assert(positions.size() == vertices_.size());
    for (size_t i = 0; i < positions.size(); ++i) {
      vertices_[i].pos = positions[i];
    }
  }
  /**
   * @brief Update the normal infomation in @c vertices_.
   * @param normals[in] - New normals for updating. 
  */
  void UpdateNormals(const std::vector<glm::vec3>& normals) {
    assert(normals.size() == vertices_.size());
    for (size_t i = 0; i < normals.size(); ++i) {
      vertices_[i].normal = normals[i];
    }
  }
  /**
   * @brief Update the color infomation in @c vertices_.
   * @param colors[in] - New colors for updating. 
  */
  void UpdateColors(const std::vector<glm::vec3>& colors) {
    assert(colors.size() == vertices_.size());
    for (size_t i = 0; i < colors.size(); ++i) {
      vertices_[i].color = colors[i];
    }
  }
  /**
   * @brief Update the indices of faces, using std::move().
   * @param indices[in] - New indices for updating. 
  */
  void UpdateIndices(std::vector<glm::ivec3>& indices) {
    indices_ = std::move(indices);
  }
  /**
   * @brief Initialize the buffers, including VAO,VBO,and VEO.
  */
  void InitGlBuffers();
  /**
   * @brief Load vertex buffer. Send all the vertices information
   *  to the GPU.
  */
  void LoadVertexBuffer();
  /**
   * @brief Load element buffer. Send all the indices to the GPU.
  */
  void LoadElementBuffer();
  /**
   * @brief Initialize the painter with fixed number of vertices
   *  and faces.
   * @param n_v[in] - Number of vertices. 
   * @param n_f[in] - Number of faces. 
  */
  MeshPainter(size_t n_v, size_t n_f) {
    vertices_.resize(n_v);
    indices_.resize(n_f);
  }
  /**
   * @brief Release the buffers when deconstruct.
  */
  ~MeshPainter();
  /// GL Buffers
  GLuint vao_ = 0, vbo_ = 0, ebo_ = 0;
  /// Draw the sketch or fill the faces?
  FillMode fill_mode_ = FillMode::kLineAndFill;
  /// Properties of vertices
  std::vector<VertInfo> vertices_;
  /// Connectivity
  std::vector<glm::ivec3> indices_;
  /// When draw the lines at the same time of faces, we need to
  /// draw twice with a offset on both sides.
  float offset_ = 1e-4f;
};

}  // namespace geometry_lab

#endif  // !GEOMETRY_LAB_RENDER_MESH_PAINTER
