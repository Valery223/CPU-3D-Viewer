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
   * @struct FaceVertex
   * @brief Один угол грани: 0-based индексы вершины/UV/нормали.
   *        Значение -1 означает, что компонента отсутствует в файле.
   */
  struct FaceVertex {
    int v;
    int vt;
    int vn;
  };

  /**
   * @brief Разбирает один токен грани (v, v/vt, v//vn, v/vt/vn).
   *        Поддерживает относительные (отрицательные) индексы OBJ.
   * @param token Токен угла грани.
   * @param mesh Меш (нужен для разрешения относительных индексов).
   * @return Разобранный угол с 0-based индексами.
   */
  static FaceVertex parseFaceVertex(const std::string& token, const Mesh& mesh);

  /**
   * @brief Триангулирует полигон веером и добавляет грани в меш,
   *        генерируя нормали и UV там, где их нет в файле.
   * @param mesh Меш, в который добавляются грани.
   * @param poly Углы полигона (>= 3).
   * @param materialIndex Индекс материала.
   */
  static void addPolygon(Mesh& mesh, const std::vector<FaceVertex>& poly,
                         uint32_t materialIndex);
};
}  // namespace s21
#endif  // OBJECT_LOADER_H
