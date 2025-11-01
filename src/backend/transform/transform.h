#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "backend/types.h"

namespace s21 {
class Transform {
 public:
  Transform();
  void translate(float x, float y, float z);

  void rotate(const Eigen::Vector3f& axis);

  void scale(float sx, float sy, float sz);

  Vertex apply(const Vertex& localVertex) const;

  Normal applyToNormal(const Normal& localNormal) const;

 private:
  Matrix4x4 matrix4x4;
};
}  // namespace s21
#endif  // TRANSFORM_H