#include "object.h"

namespace s21 {
void Object::setMesh(const Mesh& mesh) { mesh_ = mesh; }

const Mesh& Object::getMesh() const { return mesh_; }

Mesh& Object::getMesh() { return mesh_; }

void Object::setTransform(const Transform& transform) {
  transform_ = transform;
}

const Transform& Object::getTransform() const { return transform_; }

Transform& Object::getTransform() { return transform_; }

void Object::translate(float x, float y, float z) {
  transform_.translate(x, y, z);
}

void Object::rotate(const Eigen::Vector3f& axis) { transform_.rotate(axis); }

void Object::scale(float sx, float sy, float sz) {
  transform_.scale(sx, sy, sz);
}
}  // namespace s21