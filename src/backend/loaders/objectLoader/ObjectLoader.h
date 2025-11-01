#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include "backend/material_manager/material_manager.h"
#include "backend/mesh/mesh.h"
#include "backend/types.h"

namespace s21 {
/**
 * @class ObjectLoader
 * @brief Класс для загрузки 3D-объектов из файлов .obj.
 */
class ObjectLoader {
 public:
  /**
   * @brief Загружает 3D-объект из файла .obj.
   * @param filepath Путь к файлу .obj.
   * @param mesh Объект Mesh, в который загружается геометрия.
   * @param materialManager Менеджер материалов, используемый для загрузки
   * материалов.
   */
  static void loadObj(const std::string& filepath, Mesh& mesh,
                      MaterialManager& materialManager);

 private:
  /**
   * @brief Закрытый конструктор, чтобы запретить создание экземпляров класса.
   */
  ObjectLoader(){};

  /**
   * @brief Разбирает вершину из потока.
   * @param iss Поток входных данных.
   * @return Разобранная вершина.
   */
  static Vertex parseVertex(std::istringstream& iss);

  /**
   * @brief Разбирает нормаль из потока.
   * @param iss Поток входных данных.
   * @return Разобранная нормаль.
   */
  static Normal parseNormal(std::istringstream& iss);

  /**
   * @brief Разбирает координаты текстурирования из потока.
   * @param iss Поток входных данных.
   * @return Разобранные координаты UV.
   */
  static UVCoordinate parseUVcoordiantes(std::istringstream& iss);

  /**
   * @brief Разбирает грань из потока.
   * @param iss Поток входных данных.
   * @param materialIndex Индекс материала для данной грани.
   * @return Разобранная грань.
   */
  static Face parseFace(std::istringstream& iss, uint32_t materialIndex);
};
}  // namespace s21
#endif  // OBJECT_LOADER_H
