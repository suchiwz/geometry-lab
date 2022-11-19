#pragma once

#ifndef GEOMETRY_LAB_CORE_TRIMESH_HPP_
#define GEOMETRY_LAB_CORE_TRIMESH_HPP_

#include <cassert>
#include <functional>
#include <queue>
#include <string>

#include <Eigen/Core>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>

namespace geometry_lab {

/**
 * @brief Triangle mesh inherited from OpenMesh, where some
 *  additional operators are implemented.
*/
class TriMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
 public:
  /**
   * @brief Boundary structure in mesh, which stores a boundary
   *  loop.
  */
  class Boundary {
   public:
    /**
     * @brief Visit one boundary loop and do sth.
     * @param fun[in] - What we want to do, can be an anonymous 
     *                  function.
    */
    void Visit(std::function<void(const OpenMesh::SmartHalfedgeHandle&)> fun) {
      OpenMesh::SmartHalfedgeHandle it = start_;
      do {
        fun(it);
        it = it.next();
      } while (it != start_);
    }
    /**
     * @brief Overided '<' operator comparing number of edges.
     * @param a[in] - LHS value
     * @param b[in] - RHS value 
     * @retval TRUE - if 'a' has less number of edges than 'b'
     * @retval FALSE - Otherwise
    */
    friend bool operator<(const Boundary& a, const Boundary& b) {
      return a.length_ < b.length_;
    }
    /**
     * @brief Only set the @c start_ variable and does nothing.
    */
    Boundary(OpenMesh::SmartHalfedgeHandle start) : start_(start) {}
    /// The first halfedge in the loop.
    OpenMesh::SmartHalfedgeHandle start_;
    /// Number of edges on the boundary.
    size_t length_ = 0;
  };

  /// Halfedge difference property label
  static constexpr std::string_view kPropHalfedgeDiff = "HalfedgeDiff";
  /// Face area property label
  static constexpr std::string_view kPropFaceArea = "FaceArea";

  /**
   * @brief Load the mesh from an .obj file. 
   * 
   *  Default vertices normals would be computed by faces for  
   *  further requirements.
   * 
   * @param path[in] - File path
   * @param normalize[in] - Should the positions be normalized?
   * @retval FALSE - when some error occur
   * @retval TRUE - when the mesh is load successfully
  */
  bool LoadFromFile(const std::string& path, bool normalize = true);
  /**
   * @brief Update vertices normals with precomputed face normals.
  */
  void ComputeVertexNormalWithFace();
  /**
   * @brief Normalize the positions of vertices into a bounding cube.
   * @param a[in] - The scale of normalization. i.e. The bounding 
   *                cube would be scaled to (-a,-a,-a)->(a,a,a).
  */
  void NormalizePositions(float a);
  /**
   * @brief Find all the boundary loops of the mesh.
   * @return The boundary loops sorted by number of edges (longest
   *  to shortest)
  */
  std::priority_queue<Boundary> ComputeBoundaries();
  /**
   * @brief Initialize the property @c HalfedgeDiff and @c FaceArea.
  */
  void ComputeHalfedgeDifferenceAndFaceArea();

  TriMesh() {}
  /**
   * @brief Get halfedge difference property. 
   * 
   *  One should first call @c ComputeHalfedgeDifferenceAndFaceArea()
   *  to ensure the return value of @c has_halfedge_difference()
   *  returns true.
   * 
   * @return A Halfedge property manager, one can use [] operator to
   *  visit the difference vector on the halfedge.
  */
  const OpenMesh::HProp<Eigen::Vector2d> prop_halfedge_diff() {
    return OpenMesh::getProperty<OpenMesh::HalfedgeHandle, Eigen::Vector2d>(
        *this, kPropHalfedgeDiff.data());
  }
  /**
   * @brief Get face area property. 
   * 
   *  One should first call @c ComputeHalfedgeDifferenceAndFaceArea()
   *   to ensure the return value of @c has_face_area() returns true.
   * 
   * @return A Face property manager, one can use [] operator to visit
   *   the face area on the halfedge.
  */
  const OpenMesh::FProp<double> prop_face_area() {
    return OpenMesh::getProperty<OpenMesh::FaceHandle, double>(
        *this, kPropFaceArea.data());
  }
  /**
   * @brief Get the 3d position from index
   * @param id[in] - index of the vertex 
   * @return 3d position of the vertex
  */
  const Point& id2position(const size_t id) const {
    return point(vertex_handle(static_cast<uint>(id)));
  }
  /**
   * @brief Get the normal from index
   * @param id[in] - index of the vertex 
   * @return Normal of the vertex
  */
  const Normal& id2normal(const size_t id) const {
    return normal(vertex_handle(static_cast<uint>(id)));
  }
  /**
   * @return Does the Halfedge difference property exist?
  */
  bool has_halfedge_difference() const {
    return OpenMesh::hasProperty<OpenMesh::HalfedgeHandle, Eigen::Vector2d>(
        *this, kPropHalfedgeDiff.data());
  }
  /**
   * @return Does the Face area property exist?
  */
  bool has_face_area() const {
    return OpenMesh::hasProperty<OpenMesh::FaceHandle, double>(
        *this, kPropFaceArea.data());
  }
};

}  // namespace geometry_lab

#endif  // !GEOMETRY_LAB_CORE_TRIMESH_HPP_