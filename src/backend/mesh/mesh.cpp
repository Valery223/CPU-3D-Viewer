#include "mesh.h"

namespace s21 {
void Mesh::addVertex(Vertex v) { vertices_.push_back(v); }

void Mesh::addNormal(Normal n) { normals_.push_back(n); }

void Mesh::addUVCoordinate(UVCoordinate uv, bool normalize) {
  if (normalize) {
    uv.x() = uv.x() - std::floor(uv.x());
    uv.y() = uv.y() - std::floor(uv.y());
  }

  uvCoordinates_.push_back(uv);
}

void Mesh::addFace(Face face) { faces_.push_back(face); }
}  // namespace s21