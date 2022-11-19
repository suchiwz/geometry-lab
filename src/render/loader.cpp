#include "loader.hpp"

namespace geometry_lab {

void TriMeshLoader::GeneratePainter() {
  size_t n_v = mesh_->n_vertices();
  size_t n_f = mesh_->n_faces();
  painter_->vertices_.resize(n_v);
  painter_->indices_.resize(n_f);
  for (const auto& v : mesh_->vertices()) {
    const auto& p = mesh_->point(v);
    const auto& n = mesh_->normal(v);
    auto& tar = painter_->vertices_[v.idx()];
    tar.pos = {p[0], p[1], p[2]};
    tar.normal = {n[0], n[1], n[2]};
    tar.color = {1.0f, 0.9f, 0.8f};
  }
  for (const auto& f : mesh_->faces()) {
    const auto& fh0 = f.halfedge();
    auto& tar = painter_->indices_[f.idx()];
    tar[0] = fh0.from().idx();
    tar[1] = fh0.next().from().idx();
    tar[2] = fh0.next().next().from().idx();
  }
}

}  // namespace geometry_lab