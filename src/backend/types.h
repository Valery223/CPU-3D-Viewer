#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Dense>

namespace s21 {
/**
 * @typedef Vertex
 * @brief Определяет вершину как 4-мерный вектор.
 */
using Vertex = Eigen::Vector4f;

/**
 * @typedef Normal
 * @brief Определяет нормаль как 3-мерный вектор.
 */
using Normal = Eigen::Vector3f;

/**
 * @typedef Position3F
 * @brief Определяет позицию в 3D пространстве.
 */
using Position3F = Eigen::Vector3f;

/**
 * @typedef Vector3F
 * @brief Определяет 3-мерный вектор.
 */
using Vector3F = Eigen::Vector3f;

/**
 * @typedef Color
 * @brief Определяет цвет как 3-мерный вектор (RGB).
 */
using Color = Eigen::Vector3f;

/**
 * @typedef UVCoordinate
 * @brief Определяет координаты текстурирования как 2-мерный вектор.
 */
using UVCoordinate = Eigen::Vector2f;

/**
 * @typedef Matrix4x4
 * @brief Определяет 4x4 матрицу трансформаций.
 */
using Matrix4x4 = Eigen::Matrix4f;

/**
 * @class Face
 * @brief Класс, представляющий грань 3D-модели.
 */
class Face {
 public:
  uint32_t vertexIndex[3];  ///< Индексы вершин грани.
  uint32_t normalIndex[3];  ///< Индексы нормалей грани.
  uint32_t uvCoordinateIndex[3];  ///< Индексы текстурных координат.
  uint32_t materialIndex;  ///< Индекс материала, связанного с гранью.
};

/**
 * @struct Light
 * @brief Структура, представляющая источник света в 3D сцене.
 */
struct Light {
  Position3F position;  ///< Позиция источника света.
  Color color;          ///< Цвет света.

  /**
   * @brief Конструктор с параметрами.
   * @param position Позиция источника света.
   * @param color Цвет света.
   */
  Light(Position3F position, Color color) : position(position), color(color) {}

  /**
   * @brief Конструктор по умолчанию.
   * Создает источник света в позиции (0, 0, 200) с цветом (100, 100, 100).
   */
  Light() : position({0.0f, 0.0f, 200.0f}), color(100.0f, 100.0f, 100.0f) {}
};
}  // namespace s21
#endif
