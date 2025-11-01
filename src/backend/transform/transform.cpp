#include "transform.h"

namespace s21 {
Transform::Transform() { matrix4x4 = Eigen::Matrix4f::Identity(); }

void Transform::translate(float x, float y, float z) {
  Matrix4x4 translationMatrix = Eigen::Matrix4f::Identity();
  translationMatrix(0, 3) = x;
  translationMatrix(1, 3) = y;
  translationMatrix(2, 3) = z;

  matrix4x4 = translationMatrix * matrix4x4;
}

void Transform::scale(float sx, float sy, float sz) {
  Matrix4x4 scaleMatrix = Eigen::Matrix4f::Identity();
  scaleMatrix(0, 0) = sx;
  scaleMatrix(1, 1) = sy;
  scaleMatrix(2, 2) = sz;

  matrix4x4 = matrix4x4 * scaleMatrix;
}

void Transform::rotate(const Eigen::Vector3f &axis) {
  Eigen::Matrix4f rotateX = Eigen::Matrix4f::Identity();
  Eigen::Matrix4f rotateY = Eigen::Matrix4f::Identity();
  Eigen::Matrix4f rotateZ = Eigen::Matrix4f::Identity();

  rotateX(1, 1) = std::cos(axis[0]);
  rotateX(1, 2) = -std::sin(axis[0]);
  rotateX(2, 1) = std::sin(axis[0]);
  rotateX(2, 2) = std::cos(axis[0]);

  rotateY(0, 0) = std::cos(axis[1]);
  rotateY(0, 2) = std::sin(axis[1]);
  rotateY(2, 0) = -std::sin(axis[1]);
  rotateY(2, 2) = std::cos(axis[1]);

  rotateZ(0, 0) = std::cos(axis[2]);
  rotateZ(0, 1) = -std::sin(axis[2]);
  rotateZ(1, 0) = std::sin(axis[2]);
  rotateZ(1, 1) = std::cos(axis[2]);

  Eigen::Matrix4f rotateMatrix = rotateZ * rotateY * rotateX;

  matrix4x4 = matrix4x4 * rotateMatrix;
}

Vertex Transform::apply(const Vertex &localVertex) const {
  return matrix4x4 * localVertex;
}

Normal Transform::applyToNormal(const Normal &localNormal) const {
  Eigen::Matrix3f normalMatrix =
      matrix4x4.block<3, 3>(0, 0).inverse().transpose();
  return (normalMatrix * localNormal).normalized();
}
}  // namespace s21