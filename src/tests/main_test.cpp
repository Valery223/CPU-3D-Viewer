#include <gtest/gtest.h>

#include <Eigen/Dense>

#include "../backend/transform/transform.h"
using namespace s21;
TEST(TransformTest, IdentityTransform) {
  Transform transform{};
  Vertex v(1, 2, 3, 1);
  EXPECT_EQ(transform.apply(v), v);
}

TEST(TransformTest, Translation) {
  Transform transform;
  transform.translate(1, 2, 3);
  Vertex v(1, 1, 1, 1);
  Vertex result = transform.apply(v);
  EXPECT_FLOAT_EQ(result.x(), 2);
  EXPECT_FLOAT_EQ(result.y(), 3);
  EXPECT_FLOAT_EQ(result.z(), 4);
  EXPECT_FLOAT_EQ(result.w(), 1);
}

TEST(TransformTest, Scaling) {
  Transform transform;
  transform.scale(2, 3, 4);
  Vertex v(1, 1, 1, 1);
  Vertex result = transform.apply(v);
  EXPECT_FLOAT_EQ(result.x(), 2);
  EXPECT_FLOAT_EQ(result.y(), 3);
  EXPECT_FLOAT_EQ(result.z(), 4);
  EXPECT_FLOAT_EQ(result.w(), 1);
}

TEST(TransformTest, RotationX) {
  Transform transform;
  transform.rotate(Eigen::Vector3f(M_PI / 2, 0, 0));
  Vertex v(0, 1, 0, 1);
  Vertex result = transform.apply(v);
  EXPECT_NEAR(result.x(), 0, 1e-5);
  EXPECT_NEAR(result.y(), 0, 1e-5);
  EXPECT_NEAR(result.z(), 1, 1e-5);
}

TEST(TransformTest, RotationY) {
  Transform transform;
  transform.rotate(Eigen::Vector3f(0, M_PI / 2, 0));
  Vertex v(1, 0, 0, 1);
  Vertex result = transform.apply(v);
  EXPECT_NEAR(result.x(), 0, 1e-5);
  EXPECT_NEAR(result.y(), 0, 1e-5);
  EXPECT_NEAR(result.z(), -1, 1e-5);
}

TEST(TransformTest, RotationZ) {
  Transform transform;
  transform.rotate(Eigen::Vector3f(0, 0, M_PI / 2));
  Vertex v(1, 0, 0, 1);
  Vertex result = transform.apply(v);
  EXPECT_NEAR(result.x(), 0, 1e-5);
  EXPECT_NEAR(result.y(), 1, 1e-5);
  EXPECT_NEAR(result.z(), 0, 1e-5);
}

TEST(TransformTest, ApplyToNormal) {
  Transform transform;
  transform.scale(2, 2, 2);
  Normal n(0, 1, 0);
  Normal result = transform.applyToNormal(n);
  EXPECT_FLOAT_EQ(result.x(), 0);
  EXPECT_FLOAT_EQ(result.y(), 1);
  EXPECT_FLOAT_EQ(result.z(), 0);
}