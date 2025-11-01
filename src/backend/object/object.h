#ifndef OBJECT_H
#define OBJECT_H
#include "backend/mesh/mesh.h"
#include "backend/transform/transform.h"

namespace s21 {
/**
 * @class Object
 * @brief Класс, представляющий 3D-объект с геометрией и трансформациями.
 */
class Object {
 public:
  /**
   * @brief Конструктор объекта.
   * @param mesh Сетка (меш), связанная с объектом.
   */
  Object(Mesh& mesh) : mesh_(mesh), transform_{} {}

 public:
  /**
   * @brief Устанавливает новую сетку для объекта.
   * @param mesh Новая сетка.
   */
  void setMesh(const Mesh& mesh);

  /**
   * @brief Возвращает константную ссылку на сетку объекта.
   * @return Константная ссылка на Mesh.
   */
  const Mesh& getMesh() const;

  /**
   * @brief Возвращает ссылку на сетку объекта.
   * @return Ссылка на Mesh.
   */
  Mesh& getMesh();

  /**
   * @brief Устанавливает трансформацию объекта.
   * @param transform Новая трансформация.
   */
  void setTransform(const Transform& transform);

  /**
   * @brief Возвращает константную ссылку на трансформацию объекта.
   * @return Константная ссылка на Transform.
   */
  const Transform& getTransform() const;

  /**
   * @brief Возвращает ссылку на трансформацию объекта.
   * @return Ссылка на Transform.
   */
  Transform& getTransform();

 public:
  /**
   * @brief Перемещает объект в пространстве.
   * @param x Смещение по оси X.
   * @param y Смещение по оси Y.
   * @param z Смещение по оси Z.
   */
  void translate(float x, float y, float z);

  /**
   * @brief Поворачивает объект вокруг заданной оси.
   * @param axis Вектор оси вращения.
   */
  void rotate(const Eigen::Vector3f& axis);

  /**
   * @brief Масштабирует объект по каждой оси.
   * @param sx Коэффициент масштабирования по оси X.
   * @param sy Коэффициент масштабирования по оси Y.
   * @param sz Коэффициент масштабирования по оси Z.
   */
  void scale(float sx, float sy, float sz);

 private:
  Mesh mesh_;            ///< Сетка (меш) объекта.
  Transform transform_;  ///< Трансформация объекта.
};
}  // namespace s21
#endif  // OBJECT_H