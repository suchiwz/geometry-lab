#include "core/trimesh.hpp"

#include <OpenMesh/Core/IO/MeshIO.hh>

namespace geometry_lab {

bool TriMesh::LoadFromFile(const std::string& path, bool normalize) {
  OpenMesh::IO::Options opt;
  // Read the file
  if (!OpenMesh::IO::read_mesh(*this, path, opt)) {
    printf("Error::TriMesh::Failed to load mesh: %s\n\n", path.c_str());
    return false;
  }
  printf("TriMesh::Successfully loaded: %s\n\n", path.c_str());
  // Generate default vertex normal
  request_vertex_normals();
  ComputeVertexNormalWithFace();
  // Nomalize positions
  if (normalize)
    NormalizePositions(1.0f);
  return true;
}
void TriMesh::ComputeVertexNormalWithFace() {
  if (!has_vertex_normals())
    request_vertex_normals();
  request_face_normals();
  update_normals();
  release_face_normals();
}
void TriMesh::NormalizePositions(float a) {
  using vecf3 = OpenMesh::VectorT<float, 3>;
  // Find current bounding box
  vecf3 max, min;
  max = min = point(*vertices_begin());
  for (const auto& v : vertices()) {
    const auto& p = point(v);
    for (int i = 0; i < 3; ++i) {
      if (p[i] > max[i])
        max[i] = p[i];
      if (p[i] < min[i])
        min[i] = p[i];
    }
  }
  // Find contre and scale
  vecf3 translate = -(max + min) / 2.0f;
  vecf3 scale_vec = (max - min) / 2.0f;
  float scale = std::max<float>({scale_vec[0], scale_vec[1], scale_vec[2]});
  // Transform
  for (const auto& v : vertices()) {
    const vecf3& p = point(v);
    set_point(v, (p + translate) / scale);
  }
}

std::priority_queue<TriMesh::Boundary> TriMesh::ComputeBoundaries() {
  std::priority_queue<TriMesh::Boundary> rst;
  auto visited = OpenMesh::HProp<bool>(false, *this);
  for (const auto& hh : halfedges()) {
    // Visit the boundary loop
    if (hh.is_boundary() && !visited[hh]) {
      Boundary bdr(hh);
      bdr.Visit([&](const OpenMesh::SmartHalfedgeHandle& it) {
        visited[it] = true;
        bdr.length_ += 1;
      });
      rst.push(bdr);
    }
  }
  return rst;
}

void TriMesh::ComputeHalfedgeDifferenceAndFaceArea() {
  if (has_halfedge_difference() && has_face_area())
    return;
  using vecf3 = OpenMesh::VectorT<float, 3>;
  auto halfedge_diff =
      OpenMesh::HProp<Eigen::Vector2d>(*this, kPropHalfedgeDiff.data());
  auto face_area = OpenMesh::FProp<double>(*this, kPropFaceArea.data());
  vecf3 dx01, dx02;
  double l01, l02, cos0, sin0;
  for (const auto& fh : faces()) {
    // On each face, the three edges are in the order of
    //   fh.halfedge(), fh.halfedge().next(), fh.halfedge.to()
    const auto& hh01 = fh.halfedge();
    const auto& hh20 = hh01.next().next();
    dx01 = point(hh01.to()) - point(hh01.from());
    dx02 = point(hh20.from()) - point(hh20.to());
    l01 = dx01.norm();
    l02 = dx02.norm();
    cos0 = dx01.dot(dx02) / (l01 * l02);
    sin0 = sqrt(1.0 - cos0 * cos0);
    // Set the start point of fh.halfedge() to (0, 0)
    // and set the end point of fh.halfedge() to (l, 0)
    // Then we can locate the 2D flattenned point of other vertices in order
    halfedge_diff[hh01] << l01, 0;
    halfedge_diff[hh01.next()] << l02 * cos0 - l01, l02 * sin0;
    halfedge_diff[hh20] = -halfedge_diff[hh01] - halfedge_diff[hh01.next()];
    // The area of the triangle
    face_area[fh] = 0.5 * l01 * l02 * sin0;
  }
}

}  // namespace geometry_lab