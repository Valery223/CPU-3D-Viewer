#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>

#include "backend/types.h"

namespace s21 {
/**
 * @class Camera
 * @brief Класс, представляющий камеру в 3D пространстве.
 */
class Camera {
 public:
  /** @brief Позиция камеры в 3D пространстве. */
  Eigen::Vector3f position;
  /** @brief Точка, на которую смотрит камера. */
  Eigen::Vector3f target;
  /** @brief Вектор "вверх" камеры. */
  Eigen::Vector3f up;

  /** @brief Угол обзора (в радианах). */
  float fov;
  /** @brief Соотношение сторон экрана. */
  float aspect_ratio;
  /** @brief Ближняя плоскость отсечения. */
  float near_plane;
  /** @brief Дальняя плоскость отсечения. */
  float far_plane;

  /** @brief Матрица вида камеры. */
  Matrix4x4 view_matrix;
  /** @brief Матрица проекции камеры. */
  Matrix4x4 projection_matrix;

  /**
   * @brief Конструктор по умолчанию.
   * Инициализирует камеру в позиции (0,0,150), направленную в (0,0,0) с FOV 10
   * градусов.
   */
  Camera()
      : position(0.0f, 0.0f, 150.0f),
        target(0.0f, 0.0f, 0.0f),
        up(0.0f, 1.0f, 0.0f),
        fov(10.0f * (M_PI / 180.0f)),
        aspect_ratio(4.0f / 3.0f),
        near_plane(1.0f),
        far_plane(10000.0f) {
    updateViewMatrix();
    updateProjectionMatrix();
  }

  /**
   * @brief Конструктор с параметрами.
   * @param pos Позиция камеры.
   * @param tgt Точка, на которую смотрит камера.
   * @param up_dir Вектор "вверх".
   * @param fov_deg Угол обзора (в градусах).
   * @param aspect Соотношение сторон экрана.
   * @param near_p Ближняя плоскость отсечения.
   * @param far_p Дальняя плоскость отсечения.
   */
  Camera(const Position3F& pos, const Position3F& tgt, const Position3F& up_dir,
         float fov_deg, float aspect, float near_p, float far_p)
      : position(pos),
        target(tgt),
        up(up_dir),
        fov(fov_deg * (M_PI / 180.0f)),
        aspect_ratio(aspect),
        near_plane(near_p),
        far_plane(far_p) {
    updateViewMatrix();
    updateProjectionMatrix();
  }

  /**
   * @brief Обновляет матрицу вида камеры.
   */
  void updateViewMatrix() {
    Eigen::Vector3f forward = (target - position).normalized();
    Eigen::Vector3f right = forward.cross(up).normalized();
    Eigen::Vector3f up_corrected = right.cross(forward).normalized();

    view_matrix = Matrix4x4::Identity();
    view_matrix.block<3, 1>(0, 0) = right;
    view_matrix.block<3, 1>(0, 1) = up_corrected;
    view_matrix.block<3, 1>(0, 2) = -forward;

    view_matrix(0, 3) = -right.dot(position);
    view_matrix(1, 3) = -up_corrected.dot(position);
    view_matrix(2, 3) = forward.dot(position);
  }

  /**
   * @brief Обновляет матрицу проекции камеры.
   */
  void updateProjectionMatrix() {
    float tan_half_fov = tan(fov * 0.5f);
    projection_matrix = Matrix4x4::Zero();
    projection_matrix(0, 0) = 1 / (aspect_ratio * tan_half_fov);
    projection_matrix(1, 1) = 1 / tan_half_fov;
    projection_matrix(2, 2) =
        -(far_plane + near_plane) / (far_plane - near_plane);
    projection_matrix(2, 3) = -1;
    projection_matrix(3, 2) =
        -(2 * far_plane * near_plane) / (far_plane - near_plane);
    projection_matrix(3, 3) = 0;
  }

  /**
   * @brief Устанавливает новое соотношение сторон и обновляет матрицу проекции.
   * @param new_aspect_ratio Новое соотношение сторон.
   */
  void setAspectRatio(float new_aspect_ratio) {
    aspect_ratio = new_aspect_ratio;
    updateProjectionMatrix();
  }
};
}  // namespace s21
#endif  // CAMERA_H
