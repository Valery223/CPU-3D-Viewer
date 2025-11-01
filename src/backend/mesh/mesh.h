#ifndef MESH_H
#define MESH_H

#include <cstdint>
#include <iostream>

#include "backend/types.h"

namespace s21 {
/**
 * @class Mesh
 * @brief Класс, представляющий 3D-модель с вершинами, нормалями и текстурными
 * координатами.
 */
class Mesh {
 public:
  /**
   * @brief Добавляет вершину в меш.
   * @param v Вершина, добавляемая в меш.
   */
  void addVertex(Vertex v);

  /**
   * @brief Добавляет нормаль в меш.
   * @param n Нормаль, добавляемая в меш.
   */
  void addNormal(Normal n);

  /**
   * @brief Добавляет координаты UV в меш.
   * @param uv Координаты UV.
   * @param normalize Флаг нормализации координат (по умолчанию false).
   */
  void addUVCoordinate(UVCoordinate uv, bool normalize = false);

  /**
   * @brief Добавляет грань (face) в меш.
   * @param face Грань, добавляемая в меш.
   */
  void addFace(Face face);

 public:
  std::vector<Vertex> vertices_;  ///< Вектор вершин меша.
  std::vector<Normal> normals_;   ///< Вектор нормалей меша.
  std::vector<UVCoordinate> uvCoordinates_;  ///< Вектор координат UV меша.
  std::vector<Face> faces_;  ///< Вектор граней меша.
};
}  // namespace s21
#endif  // MESH_H