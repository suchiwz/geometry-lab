#pragma once

#ifndef GEOMETRY_LAB_RENDER_LOADER_HPP_
#define GEOMETRY_LAB_RENDER_LOADER_HPP_

#include <memory>
#include <string>

#include "core/trimesh.hpp"
#include "render/mesh_painter.hpp"

namespace geometry_lab {

/**
 * @brief Bind TriMesh (for computing) and MeshPainter (for 
 *  rendering). If one want to render a mesh, create this 
 *  object and process the mesh to GL.
*/
class TriMeshLoader {
 public:
  /**
   * @brief Send the mesh to GL and generate the @c painter_.
  */
  void GeneratePainter();
  /**
   * @brief InitGlBuffers()
  */
  void InitBuffers() { painter_->InitGlBuffers(); }
  /**
   * @brief LoadElementBuffer(),LoadVertexBuffer().
  */
  void LoadBuffers() {
    painter_->LoadElementBuffer();
    painter_->LoadVertexBuffer();
  }
  /**
   * @brief Initialize the Loader with a label and source mesh.
   * @param label[in] - The name of the mesh in the renderer.
   * @param mesh[in] - The source mesh.
  */
  TriMeshLoader(const std::string& label, std::shared_ptr<TriMesh> mesh)
      : label_(label), mesh_(mesh) {
    painter_ =
        std::make_shared<MeshPainter>(mesh_->n_vertices(), mesh_->n_faces());
  }
  /// The name of the mesh in the renderer.
  std::string label_;
  /// Source mesh for computing.
  std::shared_ptr<TriMesh> mesh_;
  /// Render of the mesh.
  std::shared_ptr<MeshPainter> painter_;
};

}  // namespace geometry_lab

#endif  // !GEOMETRY_LAB_RENDER_LOADER_HPP_
